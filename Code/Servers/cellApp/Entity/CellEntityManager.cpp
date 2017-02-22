
#include "pch.h"
#include "CellEntityManager.h"
#include "EntityDef/EntityType.h"
#include "EntityDef/EntityTypeManager.h"
#include "CellEntity.h"
#include "../../Clients/Client/ClientInterface.h"
#include <memory_stream.h>
#include "Space/Cell.h"
#include "Space/CellManager.h"
#include "Apps/CellApp_BaseApp.h"

namespace Aurora
{

	boost::python::object CreateEntity(EntityTypeID tid,EntityID id,AppID baseAppID,SFullCellID fullCellID,AttributeFlagT flg)
	{
		boost::shared_ptr<EntityType> pType = EntityTypeManager::get_mutable_instance().GetEntityType(tid);
		if(!pType)
			return bpy::object();

		try
		{
			bpy::object moduleObj = bpy::import("Aurora");
			bpy::object obj = moduleObj.attr("CellEntity")(id,pType,flg,baseAppID);

			obj.attr("__class__") = pType->GetClass();
			obj.attr("__init__")();
			CellEntity & entity = bpy::extract<CellEntity &>(obj);
			if(Cell * pCell = entity.GetCell())
			{
				pCell->OnNewEntity(id,obj);
			}
			else
			{
				Log_ErrorF("Entity id[%u] tid[%u] has no cell\n",id,tid);
			}

			//OnCreate;
			bpy::object objFunOnCreate = obj.attr("OnCreate");
			if(objFunOnCreate)
			{
				objFunOnCreate();
			}
			return obj;
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
		return bpy::object();
	}

	CellEntityManager::CellEntityManager()
	{

	}

	CellEntityManager::~CellEntityManager()
	{

	}

	void CellEntityManager::OnTick( float fTimeDelta )
	{
		try
		{
			for(std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.begin();iter != m_mapEntities.end();++iter)
			{
				CellEntity & ent=bpy::extract<CellEntity&>(iter->second);
				ent.OnTick(fTimeDelta);
			}
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	CellEntity * CellEntityManager::GetEntity( EntityID eid )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(eid);
		if(iter == m_mapEntities.end())
			return 0;
		try
		{
			CellEntity & ent=bpy::extract<CellEntity&>(iter->second);
			return  &ent;
		}
		catch(bpy::error_already_set )
		{
			PyErr_Print();
		}
		return 0;
	}

	void CellEntityManager::HandlePacket(CellApp_BaseApp & baseApp, const CellInterface::PktCreateEntityOnCell & pkt )
	{
		if(GetEntity(pkt.m_id) !=0 )
		{
			Log_NormalF("PktCreateEntityOnCell entity(%u) already exists\n",pkt.m_id);
			return;
		}	

		try
		{
			bpy::object objEnt = CreateEntity(pkt.m_tid,pkt.m_id,baseApp.GetBaseAppID(),pkt.m_fullCellID,DataTypeBase::EFlag_RealCell);
			m_mapEntities.insert(std::make_pair(pkt.m_id,objEnt));
			CellEntity & ent = bpy::extract<CellEntity &>(objEnt);
			ent.SetPosXYZ(0,0,0);
			ent.SetRotYPR(0,0,0);
			//发送PktCellPlayerCreate;
			{
				ClientInterface::PktPlayerEnterSpace pktToClient;
				pktToClient.m_spaceId = pkt.m_fullCellID.m_spaceId;
				pktToClient.m_channleID = pkt.m_fullCellID.m_channelId;
				pktToClient.pos_x = ent.GetPosX();
				pktToClient.pos_y = ent.GetPosY();
				pktToClient.pos_z = ent.GetPosZ();
				pktToClient.rot_p = ent.GetRotP();
				pktToClient.rot_r = ent.GetRotR();
				pktToClient.pos_y = ent.GetRotY();
				baseApp.SendPacketToClient(pktToClient,pkt.m_id);
			}
		}
		catch(bpy::error_already_set e)
		{
			PyErr_Print();
		}
	}

	void CellEntityManager::HandlePacket( CellApp_BaseApp & baseApp,const CellInterface::PktDestroyEntityOnCell & pkt )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_id);
		if( iter == m_mapEntities.end() )
		{
			Log_NormalF("PktDestroyEntityOnCell entity(%u) dose not exists\n",pkt.m_id);
			return;
		}	

		try
		{
			bpy::object objEnt = iter->second;
			m_mapEntities.erase(pkt.m_id);

			bpy::object objFun = objEnt.attr("OnDestroy");
			if(objFun)
			{
				objFun();
			}

			CellEntity & ent = bpy::extract<CellEntity &>(objEnt);
			ent.Destroy();
		}
		catch(bpy::error_already_set e)
		{
			PyErr_Print();
		}
	}

	void CellEntityManager::HandlePacket( CellApp_BaseApp & baseApp,const CellInterface::PktMethodCallFromClient & pkt )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);

		if( iter == m_mapEntities.end() )
		{
			Log_NormalF("PktMethodCallFromClient can not find entity(%u)\n",pkt.m_eID);
			return;
		}	

		try
		{
			CellEntity & ent = bpy::extract<CellEntity &>(iter->second);
			boost::shared_ptr<EntityMethod> pMethod = ent.GetType().GetCellMethods().GetMethod(pkt.m_mID);
			if(!pMethod)
			{
				Log_ErrorF("PktMethodCallFromClient entity[%u]  type[%s] does not has cellmethod[%s] \n",pkt.m_eID,ent.GetType().GetName().c_str(),pMethod->GetName().c_str());
				return;
			}

			if(!pMethod->IsExposed())
			{
				Log_ErrorF("PktMethodCallFromClient entity[%u]  type[%s] cellmethod[%s] is not exposed to client\n",pkt.m_eID,ent.GetType().GetName().c_str(),pMethod->GetName().c_str());
				return;
			}

			MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
			bpy::object objArgs = pMethod->NewObj(is);
			iter->second.attr(pMethod->GetName().c_str())(objArgs);
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void CellEntityManager::HandlePacket( CellApp_BaseApp & baseApp,const CellInterface::PktOneAttributeChange & pkt )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);

		if( iter == m_mapEntities.end() )
		{
			Log_NormalF("PktOneAttributeChange can not find entity(%u)\n",pkt.m_eID);
			return;
		}	

		try
		{
// 			CellEntity & ent = bpy::extract<CellEntity &>(iter->second);
// 			const EntityDefAttribute * pAttr = ent.GetType().GetAttribute(pkt.m_attrID);
// 			if(!pAttr)
// 			{
// 				Log_ErrorF("PktOneAttributeChange entity[%u]  type[%s] does not has attrid[%d] \n",pkt.m_eID,ent.GetType().GetName().c_str(),pkt.m_attrID);
// 				return;
// 			}
// 
// 			if( !(pAttr->IsRealCell() || pAttr->IsGhostCell())  )
// 			{
// 				Log_ErrorF("PktOneAttributeChange entity[%u]  type[%s] attr[%s] is not visiable for cell\n",pkt.m_eID,ent.GetType().GetName().c_str(),pAttr->m_strName.c_str());
// 				return;
// 			}
// 
// 			//保存一份到本地;
// 			{
// 				MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
// 				ent.GetType().ReadOneFromStream(is,iter->second,pkt.m_attrID,true);
// 			}
// 
// 			//通知其他客户端;
// 			if(pAttr->IsOtherClient())
// 			{
// 				ClientInterface::PktOneAttributeChange newPkt;
// 				newPkt.m_eID = pkt.m_eID;
// 				newPkt.m_attrID = pkt.m_attrID;
// 				newPkt.AppendToBuff(pkt.GetBuff(),pkt.GetBuffLen());
// 				ent.SendToOtherClients(newPkt);
// 			}
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void CellEntityManager::HandlePacket( CellApp_BaseApp & baseApp,const CellInterface::PktSyncPosRot & pkt )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);

		if( iter == m_mapEntities.end() )
		{
			Log_NormalF("PktSyncPosRot can not find entity(%u)\n",pkt.m_eID);
			return;
		}	

		try
		{
			CellEntity & ent = bpy::extract<CellEntity &>(iter->second);
			ent.SetPosXYZ(pkt.pos_x,pkt.pos_y,pkt.pos_z);
			ent.SetRotYPR(pkt.rot_y,pkt.rot_p,pkt.rot_r);
			ent.OnVolatileStampChangedByClient();
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

}
