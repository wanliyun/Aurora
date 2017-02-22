
#include "pch.h"
#include "CellApp_BaseApp.h"
#include "CellApp.h"
#include "../baseApp/BaseInternInterface.h"
#include "../appManager/AppManagerInterface.h"
#include "Entity/CellEntity.h"
#include "Entity/CellEntityManager.h"
#include <memory_stream.h>

namespace Aurora
{
	void CellApp_BaseApp::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_Cell);
		HADNLE_PACKET_(CellInterface::EPacketIndex_CreateEntityOnCell, CellInterface::PktCreateEntityOnCell);
		HADNLE_PACKET_(CellInterface::EPacketIndex_DestroyEntityOnCell, CellInterface::PktDestroyEntityOnCell);
		HADNLE_PACKET_(CellInterface::EPacketIndex_MethodCallFromClient, CellInterface::PktMethodCallFromClient);
		HADNLE_PACKET_(CellInterface::EPacketIndex_OneAttributeChange, CellInterface::PktOneAttributeChange);
		HADNLE_PACKET_(CellInterface::EPacketIndex_SyncPosRot, CellInterface::PktSyncPosRot);
		END_HANDLE_PACKET();
	}

	void CellApp_BaseApp::HandlePacket( const CellInterface::PktCreateEntityOnCell & pkt )
	{
		Log_NormalF("PktCreateEntityOnCell\n");
		CellEntityManager::get_mutable_instance().HandlePacket(*this,pkt);
	}

	void CellApp_BaseApp::HandlePacket( const CellInterface::PktDestroyEntityOnCell & pkt )
	{
		Log_NormalF("PktDestroyEntityOnCell\n");
		CellEntityManager::get_mutable_instance().HandlePacket(*this,pkt);
	}

	void CellApp_BaseApp::HandlePacket( const CellInterface::PktMethodCallFromClient & pkt )
	{
		CellEntityManager::get_mutable_instance().HandlePacket(*this,pkt);
	}

	void CellApp_BaseApp::HandlePacket( const CellInterface::PktOneAttributeChange & pkt )
	{
		CellEntityManager::get_mutable_instance().HandlePacket(*this,pkt);
	}

	void CellApp_BaseApp::HandlePacket( const CellInterface::PktSyncPosRot & pkt )
	{
		CellEntityManager::get_mutable_instance().HandlePacket(*this,pkt);
	}

	void CellApp_BaseApp::SendPacketToClient( const PacketBase & pktToClient,EntityID eid )
	{
		LogFX();
		BaseInternInterface::PktRedirectToClient pktToBase;

		pktToBase.m_targetClientEntityId = eid;
		
		MemoryOStream os;
		NetworkConn::WritePacketToStream(pktToClient,os);
		pktToBase.AppendToBuff(os.data(),os.size());
		GetConn().SendPacket(pktToBase);
	}

	void CellApp_BaseApp::BroadcastPacketToClient( const PacketBase & pktToClient )
	{
		LogFX();
		BaseInternInterface::PktRedirectBroadcastToClient pktToBase;
		MemoryOStream os;
		NetworkConn::WritePacketToStream(pktToClient,os);
		pktToBase.AppendToBuff(os.data(),os.size());
		GetConn().SendPacket(pktToBase);
	}


	CellApp_BaseAppManager::CellApp_BaseAppManager()
		:MultiNetworkConnOwnerManager<AppID> ()
	{

	}

	CellApp_BaseAppManager::~CellApp_BaseAppManager()
	{

	}

	void CellApp_BaseAppManager::OnWhoAmIPkt( NetworkConn & conn,const PacketBase & pkt )
	{
		MY_ASSERT(pkt.GetHeader().GetInterface() == EPacketInterface_Cell);
		MY_ASSERT(pkt.GetHeader().GetIndex() == CellInterface::EPacketIndex_WhoAmIOfBaseApp);
		CellInterface::PktWhoAmIOfBaseApp & whoAmIPkt = (CellInterface::PktWhoAmIOfBaseApp & )pkt;
		if(GetBaseApp(whoAmIPkt.m_baseAppID))
		{
			Log_ErrorF("base app[%u] already exists\n",whoAmIPkt.m_baseAppID);
			return;
		}
		conn.SetPacketHandler( this );
		CellApp_BaseApp * pBaseApp = new CellApp_BaseApp(whoAmIPkt.m_baseAppID,conn);
		if(!pBaseApp)
		{
			Log_ErrorF("new CellApp_BaseApp FAILED\n");
			return ;
		}
		Log_OKF("New BaseApp!id=[%u] addr=[%s]\n",pBaseApp->GetOwnerID(),conn.GetRemoteAddr().ToString().c_str());
		addConn(whoAmIPkt.m_baseAppID,conn,*pBaseApp);
	}

	CellApp_BaseApp * CellApp_BaseAppManager::GetBaseApp( AppID aid )
	{
		return (CellApp_BaseApp *)(getOwner(aid));
	}

	CellApp_BaseApp * CellApp_BaseAppManager::GetBaseApp( NetworkConn & conn)
	{
		return (CellApp_BaseApp *)(getOwner(conn));
	}

	void CellApp_BaseAppManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		CellApp_BaseApp * pBaseApp = GetBaseApp(conn);
		MY_ASSERT(0!= pBaseApp);
		pBaseApp->OnRecvPacket(pkt);
	}

	void CellApp_BaseAppManager::OnLoseConn( NetworkConn & conn )
	{
		CellApp_BaseApp * pBaseApp = GetBaseApp(conn);
		Log_OKF("Lose BaseApp!id=[%u] addr=[%s]\n",pBaseApp->GetOwnerID(),conn.GetRemoteAddr().ToString().c_str());
		MY_ASSERT(0!= pBaseApp);
		pBaseApp->OnLoseConn();

		delConn(conn);
	}

}
