
#include "pch.h" 
#include "CellEntity.h"
#include "CellEntityAOIManager.h"
#include "../../Clients/Client/ClientInterface.h"
#include "../baseApp/BaseInternInterface.h"
#include "Apps/CellApp_BaseApp.h"
#include <EntityDef/EntityInfo.h>
#include <EntityDef/EntityType.h>


namespace Aurora
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	bool CellEntityAOIManager::SEntityInAOI::IsStampChanged() const
	{
		return !GetEntity().CompareVolatileStamp(m_volatileStamp);
	}

	void CellEntityAOIManager::SEntityInAOI::RefreshStampChanged()
	{
		m_volatileStamp =  GetEntity().GetVolatileStamp(); 
	}


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////


	//警告:CellEntity还没有构造完成，仅仅对m_entity赋值，不应该有其他的操作了;
	CellEntityAOIManager::CellEntityAOIManager( CellEntity & theEnt )
		:m_entity(theEnt)
		,m_ownClientVolatileStamp(0)
	{

	}

	CellEntityAOIManager::~CellEntityAOIManager()
	{

	}

	void CellEntityAOIManager::OnEnterAOI( CellEntity &ent )
	{
		MY_ASSERT(ent.GetID() != m_entity.GetID());
		if(isInAOI(ent.GetID()))
			return;
		SEntityInAOI aoiEnt(ent,ENTITY_UPDATE_FLAG_ENTER_AOI,ent.GetVolatileStamp());
		m_entitiesInAOI.insert(std::make_pair(ent.GetID(),aoiEnt));

		ent.RegisterListener(m_entity.GetID());
	}

	void CellEntityAOIManager::OnLeaveAOI( CellEntity &ent)
	{
		std::map<EntityID,SEntityInAOI>::iterator iter = m_entitiesInAOI.find(ent.GetID());
		if(iter == m_entitiesInAOI.end())
			return;
		MY_ASSERT(ent.GetID() != m_entity.GetID());
		ent.DeRegisterListener(m_entity.GetID());
		iter->second.SetFlag(ENTITY_UPDATE_FLAG_LEAVE_AOI);
	}

	void CellEntityAOIManager::OnTick( float fDelta )
	{
		if(m_entity.IsDestroying())
			return;
		CellApp_BaseApp * pBaseApp = m_entity.GetBaseApp();
		if(!pBaseApp)
			return;
		
		//更新信息到本客户端及Ghost;
		{
			SyncVolatileInfo();
		}
		
		//更新AOI内实体的信息变化;
		{
			std::vector<EntityID> vLeaveEntities;
			for(std::map<EntityID,SEntityInAOI>::iterator iter = m_entitiesInAOI.begin();iter != m_entitiesInAOI.end();++iter)
			{
				const EntityID eid = iter->first;
				SEntityInAOI & aoiEnt = iter->second;

				if( aoiEnt.HasFlag(ENTITY_UPDATE_FLAG_LEAVE_AOI) )
				{
					vLeaveEntities.push_back(eid);

					ClientInterface::PktEntityLeaveAOI pktToClient;
					pktToClient.m_eid = eid;

					pBaseApp->SendPacketToClient(pktToClient,m_entity.GetID());
					continue;
				}

				MY_ASSERT(aoiEnt.IsValid());

				if(aoiEnt.HasFlag(ENTITY_UPDATE_FLAG_ENTER_AOI))
				{
					aoiEnt.ClearFlag(ENTITY_UPDATE_FLAG_ENTER_AOI);

					ClientInterface::PktEntityEnterAOI pktToClient;
					pktToClient.m_eid = eid;
					pktToClient.m_tid = aoiEnt.GetEntity().GetTypeID();
					aoiEnt.GetEntity().GetPosXYZ(pktToClient.pos_x,pktToClient.pos_y,pktToClient.pos_z);
					aoiEnt.GetEntity().GetRotYPR(pktToClient.rot_y,pktToClient.rot_p,pktToClient.rot_r);

					pBaseApp->SendPacketToClient(pktToClient,m_entity.GetID());
					continue;
				}

				if(aoiEnt.IsStampChanged())
				{
					aoiEnt.RefreshStampChanged();
					ClientInterface::PktSyncPosRot pktToClient;
					pktToClient.m_eID = eid;
					aoiEnt.GetEntity().GetPosXYZ(pktToClient.pos_x,pktToClient.pos_y,pktToClient.pos_z);
					aoiEnt.GetEntity().GetRotYPR(pktToClient.rot_y,pktToClient.rot_p,pktToClient.rot_r);
					pBaseApp->SendPacketToClient(pktToClient,m_entity.GetID());
					continue;
				}
			}

			for(std::vector<EntityID>::const_iterator iter = vLeaveEntities.begin();iter != vLeaveEntities.end();++iter)
			{
				m_entitiesInAOI.erase(*iter);
			}
		}

	}

	void CellEntityAOIManager::SyncVolatileInfo()
	{
		if(m_entity.CompareVolatileStamp(m_ownClientVolatileStamp))
			return;
		//Log_HackF("entity[%u]\n",m_entity.GetID());
		RefreshOwnClientVolatileStamp();

		//发送到BaseApp;
		{
			BaseInternInterface::PktSyncPosRot newPkt;
			newPkt.m_eID = m_entity.GetID();
			m_entity.GetPosXYZ(newPkt.pos_x,newPkt.pos_y,newPkt.pos_z);
			m_entity.GetRotYPR(newPkt.rot_y,newPkt.rot_p,newPkt.rot_r);
			m_entity.SendToBase(newPkt);
		}
	}

	void CellEntityAOIManager::RefreshOwnClientVolatileStamp()
	{
		//Log_HackF("RefreshOwnClientVolatileStamp\n");
		m_ownClientVolatileStamp = m_entity.GetVolatileStamp();
	}

	void CellEntityAOIManager::RefreshAoI( CellEntity &ent )
	{
		if(ent.GetID() == m_entity.GetID())
			return;
		bool bIn = shouldInAoI(ent);
		bool bAlreadyIn =  isInAOI(ent.GetID());
		if(bIn == bAlreadyIn)
			return ;

		if(bIn)
			OnEnterAOI(ent);
		else
			OnLeaveAOI(ent);
	}

	bool CellEntityAOIManager::shouldInAoI( CellEntity &ent )
	{
		MY_ASSERT(ent.GetID() != m_entity.GetID());
		if(ent.GetID() == m_entity.GetID())
			return false;
		
		const EntityAoIInfo & aoiInfo = m_entity.GetType().GetAoIInfo();
		float fDist = max( fabs(ent.GetPosX() - m_entity.GetPosX()),fabs(ent.GetPosZ() - m_entity.GetPosZ()));
		return int32(fDist) < aoiInfo.m_nAoIRadius;
	}
}
