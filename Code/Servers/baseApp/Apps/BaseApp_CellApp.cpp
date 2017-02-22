
#include "pch.h"
#include "BaseApp_CellApp.h"
#include "BaseApp.h"
#include "../../Clients/Client/ClientInterface.h"
#include "../appManager/AppManagerInterface.h"
#include "entity/BaseEntity.h"
#include "entity/BaseEntityManager.h"
#include "../cellApp/CellInterface.h"

namespace Aurora
{
	void BaseApp_CellApp::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_BaseIntern);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_RedirectToClient	,BaseInternInterface::PktRedirectToClient	);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_RedirectBoradcastToClient	,BaseInternInterface::PktRedirectBroadcastToClient	);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_MethodCallFromCell	,BaseInternInterface::PktMethodCallFromCell	);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_OneAttributeChangeByCell	,BaseInternInterface::PktOneAttributeChangeByCell	);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_SyncPosRot	,BaseInternInterface::PktSyncPosRot	);
		END_HANDLE_PACKET();
	}

	void BaseApp_CellApp::HandlePacket( const BaseInternInterface::PktRedirectToClient & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleCellAppPacket(*this,pkt);
	}


	void BaseApp_CellApp::HandlePacket( const BaseInternInterface::PktRedirectBroadcastToClient & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleCellAppPacket(*this,pkt);
	}


	void BaseApp_CellApp::HandlePacket( const BaseInternInterface::PktMethodCallFromCell & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleCellAppPacket(*this,pkt);
	}

	void BaseApp_CellApp::HandlePacket( const BaseInternInterface::PktOneAttributeChangeByCell & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleCellAppPacket(*this,pkt);
	}

	void BaseApp_CellApp::HandlePacket( const BaseInternInterface::PktSyncPosRot & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleCellAppPacket(*this,pkt);
	}

	void BaseApp_CellApp::OnTick( float /*fTimeDelta*/ )
	{
		if(m_state != EState_Running)
			return;
	}

	void BaseApp_CellApp::OnLoseConn()
	{
		m_state = EState_Error;
	}

	BaseApp_CellAppManager::BaseApp_CellAppManager()
		:m_theApp(BaseApp::get_mutable_instance())
	{

	}

	BaseApp_CellAppManager::~BaseApp_CellAppManager()
	{
// 		m_mapId2Conns.clear();
// 		for(std::map<AppID,NetworkConn * >::iterator iter = m_mapId2Conns.begin();iter!= m_mapId2Conns.end();++iter)
// 		{
// 			MY_ASSERT(iter->second);
// 			SAFE_DELETE(iter->second);
// 		}
	}

	void BaseApp_CellAppManager::OnTick( float /*fTimeDelta*/ )
	{
// 		for(std::map<AppID,BaseApp_CellApp * >::const_iterator iter = m_mapId2CellApp.begin();iter!= m_mapId2CellApp.end();++iter)
// 		{
// 			MY_ASSERT(iter->second);
// 		}
	}

	void BaseApp_CellAppManager::HandlePktNewCellApp( const BaseInternInterface::PktNewCellApp & pkt )
	{
		if(GetCellApp(pkt.m_appID))
		{
			Log_ErrorF("BaseInternInterface::PktNewCellApp cellapp[%u] already exists\n",pkt.m_appID);
			return;
		}

		NetworkConn * pConnToCellApp = m_theApp.DoConnect(pkt.m_internBindAddr);
		if(0 == pConnToCellApp)
		{
			Log_ErrorF("FAILED to connect cellapp[%u][%s]\n",pkt.m_appID,pkt.m_internBindAddr.ToString().c_str());
			return;
		}

		Log_OKF("Connect to CellApp succeed! addr=[%s]\n",pkt.m_internBindAddr.ToString().c_str());

		BaseApp_CellApp * pCellApp = new BaseApp_CellApp(pkt.m_appID,*pConnToCellApp);
		

		if(0 == pCellApp)
		{
			Log_ErrorF("new BaseApp_CellApp \n");
			return;
		}	


		pCellApp->GetConn().SetPacketHandler(this);
		addConn(pkt.m_appID,*pConnToCellApp,*pCellApp);

		CellInterface::PktWhoAmIOfBaseApp newPkt;
		newPkt.m_baseAppID = m_theApp.GetAppID();
		pCellApp->SendPacket(newPkt);
	}

	void BaseApp_CellAppManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		BaseApp_CellApp * pCellapp = GetCellApp(conn);
		MY_ASSERT(pCellapp);
		pCellapp->OnRecvPacket(pkt);
	}

	void BaseApp_CellAppManager::OnLoseConn( NetworkConn & conn )
	{
		BaseApp_CellApp * pCellapp = GetCellApp(conn);
		Log_OKF("Lose CellApp!id=[%u] addr=[%s]\n",pCellapp->GetOwnerID(),conn.GetRemoteAddr().ToString().c_str());
		MY_ASSERT(pCellapp);
		pCellapp->OnLoseConn();
		delConn(conn);
		SAFE_DELETE(pCellapp);
	}

	BaseApp_CellApp * BaseApp_CellAppManager::GetCellApp( AppID aid )
	{
		return (BaseApp_CellApp * )getOwner(aid);
	}

	BaseApp_CellApp * BaseApp_CellAppManager::GetCellApp( NetworkConn & conn )
	{
		return (BaseApp_CellApp * )(getOwner(conn));
	}

}
