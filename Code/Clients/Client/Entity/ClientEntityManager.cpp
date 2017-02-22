
#include "pch.h"
#include "ClientEntityManager.h"
#include "ClientApp.h"
#include "EntityDef/EntityTypeManager.h"
#include "EntityDef/EntityType.h"
#include "ClientEntity.h"
#include <memory_stream.h>

namespace Aurora
{
	boost::python::object CreateEntity(EntityTypeID tid,XEntityID id,AttributeFlagT flag)
	{
		boost::shared_ptr<EntityType> pType = EntityTypeManager::get_mutable_instance().GetEntityType(tid);
		if(!pType)
			return bpy::object();

		try
		{
			ClientEntityPtr ptr(new ClientEntity(id,pType,flag));
			bpy::object obj(ptr);

			pType->NewDict(obj.attr("__dict__"),flag,ptr.get());
			obj.attr("__class__") = pType->GetClass();
			obj.attr("__init__")();

			return obj;
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
		return bpy::object();
	}

	ClientEntityManager::ClientEntityManager()
		:m_theApp(ClientApp::get_mutable_instance())
		,m_playerID(BAD_ENTITY_ID)
		,m_playerSpaceID(BAD_SPACE_ID)
		,m_playerSpaceChannelID(BAD_SPACE_CHANNEL_ID)
	{

	}

	ClientEntityManager::~ClientEntityManager()
	{
		
	}

	void ClientEntityManager::OnTick( float fTimeDelta )
	{
		for(MapEntitiesT::iterator iter = m_entities.begin();iter != m_entities.end();++iter)
		{
			ClientEntity & ent = bpy::extract<ClientEntity &>(iter->second);
			ent.OnTick(fTimeDelta);

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

	void ClientEntityManager::OnRecvBaseAppPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_Client);

		HADNLE_PACKET_(ClientInterface::EPacketIndex_WhoAmIResponse, ClientInterface::PktWhoAmIResponse);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_EchoBack, ClientInterface::PktEchoBack);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_BroadCast, ClientInterface::PktBroadCast);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_BasePlayerCreate, ClientInterface::PktBasePlayerCreate);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_OneAttributeChange, ClientInterface::PktOneAttributeChange);

		HADNLE_PACKET_(ClientInterface::EPacketIndex_PlayerEnterSpace, ClientInterface::PktPlayerEnterSpace);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_PlayerLeaveSpace, ClientInterface::PktPlayerLeaveSpace);

		HADNLE_PACKET_(ClientInterface::EPacketIndex_EntityEnterAOI, ClientInterface::PktEntityEnterAOI);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_EntityLeaveAOI, ClientInterface::PktEntityLeaveAOI);

		HADNLE_PACKET_(ClientInterface::EPacketIndex_MethodCallFromBase, ClientInterface::PktMethodCallFromBase);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_MethodCallFromCell, ClientInterface::PktMethodCallFromCell);
		HADNLE_PACKET_(ClientInterface::EPacketIndex_SyncPosRot, ClientInterface::PktSyncPosRot);
		END_HANDLE_PACKET();
	}
	void ClientEntityManager::HandlePacket( const ClientInterface::PktWhoAmIResponse & pkt )
	{
		LogF("PktWhoAmIResponse\n");

		if(pkt.m_bSucc)
		{
			m_theApp.m_state = ClientApp::ES_RUNNING;
			LogF("succeed\n");
		}
		else
		{
			m_theApp.m_state = ClientApp::ES_INITED;
			LogF("failed\n");
		}
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktEchoBack & pkt )
	{
		LogF("PktEchoBack [%s]\n",pkt.m_buff);
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktBroadCast & pkt )
	{
		LogF("PktBroadCast [%s]\n",pkt.m_buff);
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktBasePlayerCreate & pkt )
	{
		if( m_entities.find(pkt.m_id) != m_entities.end())
		{
			Log_NormalF("PktBasePlayerCreate entity (id=%u) already exists\n",pkt.m_id);
			return;
		}
		
		bpy::object obj = CreateEntity(pkt.m_tid,pkt.m_id,DataTypeBase::EFlag_OwnClient);
		if(!obj)
		{
			Log_ErrorF("PktBasePlayerCreate Create Entity Failed\n");
			return;
		}
		
		m_entities.insert( std::make_pair(pkt.m_id,obj));
		m_playerID = pkt.m_id;

		ClientEntity & ent = bpy::extract<ClientEntity&>(obj);
		MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());

		ent.GetType()->ReadFromStream(is,obj);
		ent.OnBaseCreate(obj);
		ent.StartControlled(obj);
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktOneAttributeChange & pkt )
	{
		MapEntitiesT::iterator iter =  m_entities.find(pkt.m_eID);
		if( iter == m_entities.end())
		{
			Log_NormalF("PktOneAttributeChange can not find entity (id=%u)\n",pkt.m_eID);
			return;
		}
		bpy::object & obj = iter->second;
		ClientEntity & ent = bpy::extract<ClientEntity&>(obj);
		MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
		ent.GetType()->OnSingleAttributeChange(obj,pkt.m_path,pkt.m_attrID,is,true);
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktPlayerEnterSpace & pkt )
	{
		MapEntitiesT::iterator iter = m_entities.find(m_playerID);
		if(iter == m_entities.end())
		{
			Log_ErrorF("PktPlayerEnterSpace player[%u] does exist \n",m_playerID);
			return;
		}
		try
		{
			ClientEntity & ent = bpy::extract<ClientEntity &>(iter->second);
			
			ent.SetPosXYZ(pkt.pos_x,pkt.pos_y,pkt.pos_z);
			ent.SetRotYPR(pkt.rot_y,pkt.rot_p,pkt.rot_r);
			m_playerSpaceID = pkt.m_spaceId;
			m_playerSpaceChannelID  = pkt.m_channleID;

			iter->second.attr("OnPlayerEnterSpace")();
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktPlayerLeaveSpace & pkt )
	{
		MapEntitiesT::iterator iter = m_entities.find(m_playerID);
		if(iter == m_entities.end())
		{
			Log_ErrorF("PktPlayerLeaveSpace player[%u] does exist \n",m_playerID);
			return;
		}
		try
		{
			ClientEntity & ent = bpy::extract<ClientEntity &>(iter->second);

			m_playerSpaceID = BAD_SPACE_ID;
			m_playerSpaceChannelID = BAD_SPACE_CHANNEL_ID;

			iter->second.attr("OnPlayerLeaveSpace")();
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktEntityEnterAOI & pkt )
	{
		Log_NormalF("PktEntityEnterAOI eid[%u] tid[%u]\n",pkt.m_eid,pkt.m_tid);

		if( m_entities.find(pkt.m_eid) != m_entities.end())
		{
			Log_NormalF("PktEntityEnterAOI entity (id=%u) already exists\n",pkt.m_eid);
			return;
		}
		try
		{
			bpy::object obj = CreateEntity(pkt.m_tid,pkt.m_eid,DataTypeBase::EFlag_OtherClient);
			if(!obj)
			{
				Log_ErrorF("PktEntityEnterAOI Create Entity Failed\n");
				return;
			}

			ClientEntity & ent = bpy::extract<ClientEntity &>(obj);
			ent.SetPosXYZ(pkt.pos_x,pkt.pos_y,pkt.pos_z);
			ent.SetRotYPR(pkt.rot_y,pkt.rot_p,pkt.rot_r);

			m_entities.insert( std::make_pair(pkt.m_eid,obj));

			obj.attr("OnEnterAOI")();
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktEntityLeaveAOI & pkt )
	{
		Log_NormalF("PktEntityLeaveAOI eid[%u]\n",pkt.m_eid);
		MapEntitiesT::iterator iter = m_entities.find(pkt.m_eid);
		if( iter == m_entities.end())
		{
			Log_NormalF("PktEntityLeaveAOI entity (id=%u) does not exists\n",pkt.m_eid);
			return;
		}
		try
		{
			iter->second.attr("OnLeaveAOI")();
			m_entities.erase(pkt.m_eid);
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktMethodCallFromBase & pkt )
	{
		MapEntitiesT::iterator iter = m_entities.find(pkt.m_eid);
		if(iter == m_entities.end())
		{
			Log_ErrorF("PktMethodCallFromBase entity[%u] does exist \n",pkt.m_eid);
			return;
		}
		try
		{
			PyObject * pyobj = PyBytes_FromStringAndSize(pkt.m_strFunArg.c_str(),pkt.m_strFunArg.size());
			boost::python::object o(boost::python::handle<>(boost::python::borrowed(pyobj)));
			iter->second.attr(pkt.m_strFunName.c_str())(o);
			Py_XDECREF(pyobj);
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void ClientEntityManager::HandlePacket( const ClientInterface::PktMethodCallFromCell & pkt )
	{
		MapEntitiesT::iterator iter = m_entities.find(pkt.m_eID);
		if(iter == m_entities.end())
		{
			Log_ErrorF("PktMethodCallFromCell entity[%u] does exist \n",pkt.m_eID);
			return;
		}

		try
		{
			ClientEntity & ent = bpy::extract<ClientEntity &>(iter->second);
			boost::shared_ptr<EntityMethod> pMethod = ent.GetType()->GetClientMethods().GetMethod(pkt.m_mID);
			if(!pMethod)
			{
				Log_ErrorF("PktMethodCallFromCell entity[%u]  type[%s] does not has clientmethod[%s] \n",pkt.m_eID,ent.GetType()->GetName().c_str(),pMethod->GetName().c_str());
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

	void ClientEntityManager::HandlePacket( const ClientInterface::PktSyncPosRot & pkt )
	{
		MapEntitiesT::iterator iter = m_entities.find(pkt.m_eID);
		if(iter == m_entities.end())
		{
			Log_ErrorF("PktSyncPosRot entity[%u] does exist \n",pkt.m_eID);
			return;
		}
		Log_HackF("PktSyncPosRot [%u] [%f]\n",pkt.m_eID,pkt.pos_x);
		try
		{
			ClientEntity & ent = bpy::extract<ClientEntity &>(iter->second);
			ent.SetPosXYZ(pkt.pos_x,pkt.pos_y,pkt.pos_z);
			ent.SetRotYPR(pkt.rot_y,pkt.rot_p,pkt.rot_r);
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
	}

	void ClientEntityManager::ReleaseAll()
	{
		MapEntitiesT tmp;
		tmp.swap(m_entities);
	}

}

