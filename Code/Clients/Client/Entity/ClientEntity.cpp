
#include "pch.h"
#include <entitydef/EntityType.h>
#include "ClientEntity.h"
#include "../../Servers/cellApp/CellInterface.h"
#include "../../Servers/baseApp/BaseExternInterface.h"
#include "ClientApp.h"
#include "Apps/ClientApp_BaseApp.h"
#include <memory_stream.h>
namespace Aurora
{

	ClientEntity::ClientEntity(XEntityID id,EntityTypePtr pType,AttributeFlagT flag )
		:PosRotEntityBase(pType,XID2ID(id),XID2AppID(id))
		,m_bBaseCreated(false)
		,m_bIsControlled(false)
	{

	}

	ClientEntity::~ClientEntity()
	{

	}

	void ClientEntity::OnTick( float fTimeDelta )
	{

	}

	void ClientEntity::OnBaseCreate( bpy::object & self )
	{
		m_bBaseCreated = true;
		try
		{
			self.attr("OnBaseCreate")();
		}
		catch(...)
		{
			PyErr_Print();
		}
	}


	void ClientEntity::SendToBase( PacketBase & pkt )
	{
		ClientApp::get_mutable_instance().GetBaseApp().SendPacket(pkt);
	}

	void ClientEntity::SendToCell( PacketBase & pkt )
	{
		ClientApp::get_mutable_instance().GetBaseApp().SendPacketToCell(pkt,GetID());
	}

	void ClientEntity::CallBaseMethod( std::string strName,bpy::dict args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType()->GetBaseMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no base method[%s]\n",GetType()->GetName().c_str(),strName.c_str());
			return ;
		}
		if(!pMethod->IsExposed())
		{
			Log_ScriptF("entity type [%s] basemethod[%s] is not exposed to client\n",GetType()->GetName().c_str(),strName.c_str());
			return ;
		}

		BaseExternInterface::PktMethodCallFromClient pktToBase;
		pktToBase.m_eID = m_Id;
		pktToBase.m_mID = idx;
		
		MemoryOStream os;
		pMethod->WriteToStream(os,args,DataTypeBase::EFlag_All);
		pktToBase.AppendToBuff(os.data(),os.size());
		SendToBase(pktToBase);
	}

	void ClientEntity::CallCellMethod( std::string strName,bpy::dict args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType()->GetBaseMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no cell method[%s]\n",GetType()->GetName().c_str(),strName.c_str());
			return ;
		}
		if(!pMethod->IsExposed())
		{
			Log_ScriptF("entity type [%s] cellmethod[%s] is not exposed to client\n",GetType()->GetName().c_str(),strName.c_str());
			return ;
		}

		CellInterface::PktMethodCallFromClient pkt;
		pkt.m_eID = m_Id;
		pkt.m_mID = idx;

		MemoryOStream os;
		pMethod->WriteToStream(os,args,DataTypeBase::EFlag_All);
		pkt.AppendToBuff(os.data(),os.size());
		SendToCell(pkt);
	}

	bool ClientEntity::StartControlled(boost::python::object self)
	{
		if(m_bIsControlled)
			return true;

		if(GetType()->GetControlledClass().is_none())
		{
			Log_NormalF("tid[%u] can has no controlled class\n",GetTypeID());
			return false;
		}

		try
		{
			self.attr("__class__") = GetType()->GetControlledClass();
			self.attr("OnStartControll")();
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
		m_bIsControlled = true;
		return true;
	}

	bool ClientEntity::StopControlled( boost::python::object self )
	{
		if(!m_bIsControlled)
			return true;
		if(GetType()->GetClass().is_none())
		{
			Log_NormalF("tid[%u] can has no class\n",GetTypeID());
			return false;
		}
		try
		{
			self.attr("__class__") = GetType()->GetClass();
			self.attr("OnStopControll")();
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
		m_bIsControlled = false;
		return true;
	}

	void ClientEntity::SyncPosRotToServer()
	{
		if(!m_bIsControlledPosRotChanged)
			return;
		m_bIsControlledPosRotChanged = true;

		BaseExternInterface::PktSyncPosRot newPkt;
		newPkt.m_eID = GetID();
		GetPosXYZ(newPkt.pos_x,newPkt.pos_y,newPkt.pos_z);
		GetRotYPR(newPkt.rot_y,newPkt.rot_p,newPkt.rot_r);
		SendToBase(newPkt);
	}

}

