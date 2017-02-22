
#include "pch.h"
#include "BaseEntityManager.h"
#include "Apps/BaseApp_Client.h"
#include "Apps/BaseApp_CellApp.h"
#include "../../Clients/Client/ClientInterface.h"
#include "BaseEntity.h"
#include <EntityDef/EntityType.h>
#include <EntityDef/EntityTypeManager.h>
#include <EntityDef/DataTypes/DataTypeBase.h>
#include <memory_stream.h>
#include "../cellApp/CellInterface.h"


namespace Aurora
{
	boost::python::object CreateEntity(const std::string & strName,EntityID id,AttributeFlagT flag)
	{
		boost::shared_ptr<EntityType> pType = EntityTypeManager::get_mutable_instance().GetEntityType(strName);
		if(!pType)
			return bpy::object();

		try
		{
			bpy::object moduleObj = bpy::import("Aurora");
			BaseEntityPtr ptr(new BaseEntity(id,pType));
			bpy::object obj(ptr);

			pType->NewDict(obj.attr("__dict__"),flag,ptr.get());
			obj.attr("__class__") = pType->GetClass();
			obj.attr("__init__")();

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

	boost::python::object CreateEntity(EntityTypeID tid,EntityID id,AttributeFlagT flag)
	{
		boost::shared_ptr<EntityType> pType = EntityTypeManager::get_mutable_instance().GetEntityType(tid);
		if(!pType)
			return bpy::object();
		return CreateEntity(pType->GetName(),id,flag);
	}

	BaseEntityManager::BaseEntityManager()
		:m_entityIDPool(m_mapEntities,0x1,0x80000000,BAD_ENTITY_ID)
	{

	}

	BaseEntityManager::~BaseEntityManager()
	{
		m_mapEntities.clear();
	}

	void BaseEntityManager::OnTick( float fTimeDelta )
	{
		for(std::map<EntityID,bpy::object>::const_iterator iter = m_mapEntities.begin();iter != m_mapEntities.end();++iter)
		{
			try
			{
				 iter->second.attr("OnTick")(fTimeDelta);
			}
			catch(...)
			{
				PyErr_Print();
			}
		}
	}

	BaseEntity & BaseEntityManager::GetEntity( EntityID eid )
	{
		BaseEntity & ent=bpy::extract<BaseEntity&>(m_mapEntities[eid]);
		return ent;
	}


// 	bool BaseEntityManager::HandlePktCreateAccountRquest( const BaseInternInterface::PktCreateAccountRquest & pkt )
// 	{
// 		if( m_mapIncomingPlayers.find(pkt.m_id) != m_mapIncomingPlayers.end() )
// 		{
// 			Log_ErrorF("Incoming palyer id=[%u],ID ALREADY EXISTS IN INCOMING PLAYERS\n",pkt.m_id);
// 			return false;
// 		}
// 
// 		if( m_mapEntities.find(pkt.m_id) != m_mapEntities.end() )
// 		{
// 			Log_ErrorF("Incoming palyer id=[%u],ID ALREADY EXISTS\n",pkt.m_id);
// 			return false;
// 		}
// 
// 		SIncomingPlayer incomingPlayer;
// 		incomingPlayer.m_tid = pkt.m_tid;
// 		//incomingPlayer.m_authKey = 
// 		incomingPlayer.m_strBlowfishKey = pkt.m_strBlowfishKey;
// 		incomingPlayer.m_fLeftTime = 60.0f;	//最长等待60秒;
// 
// 		return m_mapIncomingPlayers.insert( std::make_pair(pkt.m_id,incomingPlayer)).second;
// 	}

	bool BaseEntityManager::HandlePacket( const BaseInternInterface::PktBroadcastMsg & pkt )
	{
		ClientInterface::PktBroadCast newPkt;
		memcpy(newPkt.m_buff,pkt.m_msg,sizeof(pkt.m_msg));
		for(std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.begin();iter!=m_mapEntities.end();++iter)
		{
			BaseEntity & ent = bpy::extract<BaseEntity &>(iter->second);
			if(!ent.HasClient())
				continue;
			ent.GetClient()->SendPacket(newPkt);
		}
		return true;
	}

	void BaseEntityManager::HandleCellAppPacket(BaseApp_CellApp & /*cellApp*/, const BaseInternInterface::PktRedirectToClient & pkt )
	{
		if(!HasEntity(pkt.m_targetClientEntityId))
		{
			Log_ErrorF("can not find entity %u\n",pkt.m_targetClientEntityId);
			return;
		}

		BaseEntity &ent = GetEntity(pkt.m_targetClientEntityId);

		BaseApp_Client * pClnt = ent.GetClient();
		if(!pClnt)
		{
			Log_ErrorF("entity %u does no has client\n",pkt.m_targetClientEntityId);
			return;
		}

		pClnt->GetConn().SendRawPacket(pkt.GetBuff(),(uint16)pkt.GetBuffLen());
	}

	void BaseEntityManager::HandleCellAppPacket(BaseApp_CellApp & /*cellApp*/, const BaseInternInterface::PktRedirectBroadcastToClient & pkt )
	{
		for(std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.begin();iter!=m_mapEntities.end();++iter)
		{
			BaseEntity & ent = bpy::extract<BaseEntity &>(iter->second);
			BaseApp_Client * pClnt = ent.GetClient();

			if(!pClnt)
				continue;
			pClnt->GetConn().SendRawPacket(pkt.GetBuff(),(uint16)pkt.GetBuffLen());
		}
	}

	void BaseEntityManager::HandleCellAppPacket( BaseApp_CellApp & /*cellApp*/,const BaseInternInterface::PktMethodCallFromCell & pkt )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);
		if(iter == m_mapEntities.end())
		{
			Log_ErrorF("PktMethodCallFromCell entity[%u] does not exists\n",pkt.m_eID);
			return;
		}

		try
		{
			BaseEntity & ent = bpy::extract<BaseEntity &>(iter->second);
			boost::shared_ptr<EntityMethod> pMethod = ent.GetType()->GetBaseMethods().GetMethod(pkt.m_mID);
			if(!pMethod)
			{
				Log_ErrorF("PktMethodCallFromCell entity[%u]  type[%s] does not has basemethod[%s] \n",pkt.m_eID,ent.GetType()->GetName().c_str(),pMethod->GetName().c_str());
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

	void BaseEntityManager::HandleCellAppPacket( BaseApp_CellApp & /*cellApp*/,const BaseInternInterface::PktOneAttributeChangeByCell & pkt )
	{
// 		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);
// 		if(iter == m_mapEntities.end())
// 		{
// 			Log_ErrorF("PktOneAttributeChangeByCell entity[%u] does not exists\n",pkt.m_eID);
// 			return;
// 		}
// 
// 		try
// 		{
// 			BaseEntity & ent = bpy::extract<BaseEntity &>(iter->second);
// 			boost::shared_ptr<DataTypeBase> pAttr = ent.GetType().GetChildTypeByID(pkt.m_attrID);
// 			if(!pAttr)
// 			{
// 				Log_ErrorF("PktOneAttributeChangeByCell entity[%u]  type[%s] does not has attrid[%d] \n",pkt.m_eID,ent.GetDef().GetName().c_str(),pkt.m_attrID);
// 				return;
// 			}
// 
// 			if(!pAttr->IsCellOwn())
// 			{
// 				Log_ErrorF("PktOneAttributeChangeByCell entity[%u]  type[%s] attr[%s] is not owned by cell\n",pkt.m_eID,ent.GetDef().GetName().c_str(),pAttr->m_strName.c_str());
// 				return;
// 			}
// 			
// 			//保存一份到本地;
// 			{
// 				MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
// 				bpy::object objNewVal = pAttr->ReadFromStream(is,);
// 				PyScript::SetAttr(iter->second,pAttr->m_strName,objNewVal,true);
// 			}
// 
// 
// 			//转发给本地客户端;
// 			if(pAttr->IsOwnClient())
// 			{
// 				ClientInterface::PktOneAttributeChange newPkt;
// 				newPkt.m_eID = pkt.m_eID;
// 				newPkt.m_attrID = pkt.m_attrID;
// 				newPkt.AppendToBuff(pkt.GetBuff(),pkt.GetBuffLen());
// 				ent.SendToClient(newPkt);
// 			}
// 
// 			//转发给Ghost;
// 			if(pAttr->IsGhostCell())
// 			{
// 				//ent.SendToGhostCells(newPkt);	
// 			}
// 		}
// 		catch(bpy::error_already_set)
// 		{
// 			PyErr_Print();
// 		}

	}

	void BaseEntityManager::HandleCellAppPacket( BaseApp_CellApp & /*cellApp*/,const BaseInternInterface::PktSyncPosRot & pkt )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);
		if(iter == m_mapEntities.end())
		{
			Log_ErrorF("PktSyncPosRot entity[%u] does not exists\n",pkt.m_eID);
			return;
		}

		try
		{
			BaseEntity & ent = bpy::extract<BaseEntity &>(iter->second);
			
			//保存一份到本地;
			{
				ent.SetPosXYZ(pkt.pos_x,pkt.pos_y,pkt.pos_z);
				ent.SetRotYPR(pkt.rot_y,pkt.rot_p,pkt.rot_r);
			}


			//转发给本地客户端;
			{
				ClientInterface::PktSyncPosRot newPkt;
				newPkt.m_eID = pkt.m_eID;
				newPkt.pos_x = pkt.pos_x;
				newPkt.pos_y = pkt.pos_y;
				newPkt.pos_z = pkt.pos_z;
				newPkt.rot_y = pkt.rot_y;
				newPkt.rot_p = pkt.rot_p;
				newPkt.rot_r = pkt.rot_r;
				ent.SendToClient(newPkt);
			}

			//转发给Ghost;
			{
				CellInterface::PktSyncPosRot newPkt;
				newPkt.m_eID = pkt.m_eID;
				newPkt.pos_x = pkt.pos_x;
				newPkt.pos_y = pkt.pos_y;
				newPkt.pos_z = pkt.pos_z;
				newPkt.rot_y = pkt.rot_y;
				newPkt.rot_p = pkt.rot_p;
				newPkt.rot_r = pkt.rot_r;
				ent.SendToGhostCells(newPkt);	
			}
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}


	void BaseEntityManager::HandleClientPacket( BaseApp_Client & /*clnt*/,const BaseExternInterface::PktRedirectToCell & pkt )
	{
		if(!HasEntity(pkt.m_nEntityID))
		{
			Log_ErrorF("can not find entity %u\n",pkt.m_nEntityID);
			return;
		}
		BaseApp_CellApp * pCellApp = GetEntity(pkt.m_nEntityID).GetCellApp();
		if(!pCellApp)
		{
			Log_ErrorF("entity %u does no has cell\n",pkt.m_nEntityID);
			return;
		}

		pCellApp->GetConn().SendRawPacket(pkt.GetBuff(),(uint16)pkt.GetBuffLen());
	}

	void BaseEntityManager::HandleClientPacket( BaseApp_Client & /*clnt*/, const BaseExternInterface::PktMethodCallFromClient & pkt )
	{
// 		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);
// 		if(iter == m_mapEntities.end())
// 		{
// 			Log_ErrorF("PktMethodCallFromClient entity[%u] does not exists\n",pkt.m_eID);
// 			return;
// 		}
// 
// 		try
// 		{
// 			BaseEntity & ent = bpy::extract<BaseEntity &>(iter->second);
// 			const EntityDefMethod * pMethod = ent.GetDef().GetBaseMethods().GetMethod(pkt.m_mID);
// 			if(!pMethod)
// 			{
// 				Log_ErrorF("PktMethodCallFromClient entity[%u]  type[%s] does not has basemethod[%s] \n",pkt.m_eID,ent.GetDef().GetName().c_str(),pMethod->GetName().c_str());
// 				return;
// 			}
// 
// 			if(!pMethod->IsExposed())
// 			{
// 				Log_ErrorF("PktMethodCallFromClient entity[%u]  type[%s] basemethod[%s] is not exposed to client\n",pkt.m_eID,ent.GetDef().GetName().c_str(),pMethod->GetName().c_str());
// 				return;
// 			}
// 
// 			MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
// 			bpy::object objArgs = pMethod->ReadFromStream(is);
// 			if(objArgs.is_none())
// 			{
// 				Log_ErrorF("PktMethodCallFromClient entity[%u]  type[%s] basemethod[%s] read args error\n",pkt.m_eID,ent.GetDef().GetName().c_str(),pMethod->GetName().c_str());
// 				return;
// 			}
// 			iter->second.attr(pMethod->GetName().c_str())(objArgs);
// 		}
// 		catch(bpy::error_already_set)
// 		{
// 			PyErr_Print();
// 		}
	}

	void BaseEntityManager::HandleClientPacket( BaseApp_Client & /*clnt*/, const BaseExternInterface::PktSyncPosRot & pkt )
	{
		std::map<EntityID,bpy::object>::iterator iter = m_mapEntities.find(pkt.m_eID);
		if(iter == m_mapEntities.end())
		{
			Log_ErrorF("PktSyncPosRot entity[%u] does not exists\n",pkt.m_eID);
			return;
		}

		try
		{
			BaseEntity & ent = bpy::extract<BaseEntity &>(iter->second);
			ent.SetPosXYZ(pkt.pos_x,pkt.pos_y,pkt.pos_z);
			ent.SetRotYPR(pkt.rot_y,pkt.rot_p,pkt.rot_r);

			CellInterface::PktSyncPosRot newPkt;
			newPkt.m_eID = pkt.m_eID;
			ent.GetPosXYZ(newPkt.pos_x,newPkt.pos_y,newPkt.pos_z);
			ent.GetRotYPR(newPkt.rot_y,newPkt.rot_p,newPkt.rot_r);
			ent.SendToCell(newPkt);
			ent.SendToGhostCells(newPkt);
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	bool BaseEntityManager::OnNewClientWithMsg( BaseApp_Client & clnt, const BaseExternInterface::PktWhoAmI & pkt )
	{
		EntityID entityID;
		if(!m_entityIDPool.GenerateUniqueID(entityID))
		{
			Log_ErrorF("Failed to GenerateUniqueID\n");
			return false;
		}

		bpy::object entObj = CreateEntity(0,entityID,DataTypeBase::EFlag_All);
		if(entObj.is_none())
		{
			Log_ErrorF("FAILED CreateEntity\n");
			return false;
		}

	
		BaseEntity & ent = bpy::extract<BaseEntity & >(entObj);
		m_mapEntities.insert( std::make_pair(entityID,entObj) );

		//回复客户端;
		ClientInterface::PktWhoAmIResponse newPkt;
		newPkt.m_bSucc =true;
		newPkt.m_accuntEntityID = ent.GetXID();
		clnt.SendPacket(newPkt);

		//客户端创建Entity;
		ent.OnAttachClient(entObj,clnt);
		return true;
	}

	void BaseEntityManager::OnLoseClient( BaseApp_Client & clnt )
	{
		EntityIDSet_T entityIds(clnt.GetEntities());	//只能拷贝,下面的循环中会删除clnt容器中的元素;

		for(EntityIDSet_T::const_iterator iter = entityIds.begin();iter!=entityIds.end();++iter)
		{
			EntityID eid = *iter;
			std::map<EntityID,bpy::object>::iterator itEnt = m_mapEntities.find(eid);

			if(itEnt == m_mapEntities.end())
			{
				Log_ErrorF(" can not find endity id=%u\n",eid);
				continue;
			}
			bpy::object objEnt = itEnt->second;
			MY_ASSERT(!objEnt.is_none());
			BaseEntity & ent = bpy::extract<BaseEntity&>( objEnt );
			ent.OnDetachClient(objEnt,clnt);

			m_mapEntities.erase(itEnt);
		}

		clnt.GetEntities().clear();
	}

	bpy::object  BaseEntityManager::CreateAccountEntity()
	{
		EntityID entityID;
		if(!m_entityIDPool.GenerateUniqueID(entityID))
		{
			Log_ErrorF("Failed to GenerateUniqueID\n");
			return bpy::object();
		}

		bpy::object entObj = CreateEntity(GetAccountTypeName(),entityID,DataTypeBase::EFlag_All);
		if(entObj.is_none())
		{
			Log_ErrorF("FAILED CreateEntity\n");
			return bpy::object();
		}

		m_mapEntities.insert( std::make_pair(entityID,entObj) );
		return entObj;
	}

}
