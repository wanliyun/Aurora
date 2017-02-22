
#include "pch.h"
#include "CellApp_AppManager.h"
#include "CellApp.h"
#include "CellApp_BaseApp.h"
#include "Entity/CellEntityManager.h"
#include <memory_stream.h>
#include "Space/CellManager.h"
#include "../appManager/AppManagerInterface.h"
namespace Aurora
{
	CellApp_AppManager::CellApp_AppManager( )
		:m_theApp(CellApp::get_mutable_instance())
	{

	}

	void CellApp_AppManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		MY_ASSERT(GetConn() == &conn);
		BEGINE_HANDLE_PACKET(EPacketInterface_Cell);
		HADNLE_PACKET_(CellInterface::EPacketIndex_WhoAmIResponse, CellInterface::PktWhoAmIResponse);
		HADNLE_PACKET_(CellInterface::EPacketIndex_SyncAllSpaceInfo, CellInterface::PktSyncAllSpaceInfo);
		END_HANDLE_PACKET();
	}


	void CellApp_AppManager::HandlePacket( const CellInterface::PktWhoAmIResponse & pkt )
	{
		LogF("PktWhoAmIResponse succ=%d,id=%d\n",pkt.m_bSucc,pkt.m_id);
		m_theApp.m_appId = pkt.m_id;
	}

	void CellApp_AppManager::HandlePacket( const CellInterface::PktSyncAllSpaceInfo & pkt )
	{
		CellManager::get_mutable_instance().OnRecvPacket(pkt);
	}

	void CellApp_AppManager::OnLoseConn( NetworkConn & conn )
	{
		m_theApp.OnLoseAppMgr();
	}

	bool CellApp_AppManager::DoConnect( const SocketAddress & addr )
	{
		if(!NetworkConnHolder::DoConnect(m_theApp,addr))
		{
			Log_ErrorF("Connect AppManager[%s] Failed!\n",addr.ToString().c_str());
			return false;
		}

		Log_OKF("Connect AppManager[%s] Succeed! my addr=[%s]\n",addr.ToString().c_str(),GetConn()->GetRemoteAddr().ToString().c_str());
		GetConn()->SetPacketHandler( this );

		AppManagerInterface::PktWhoAmI pkt;
		pkt.m_AppType = AppManagerInterface::PktWhoAmI::EAppType_CellApp;
		pkt.m_internBindAddr = m_theApp.GetInternBindAddr();
		pkt.m_externBindAddr = m_theApp.GetExternBindAddr();
		GetConn()->SendPacket(pkt);
		return true;
	}

}
