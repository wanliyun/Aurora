
#include "pch.h"
#include "AppMgr_DBManager.h"
#include "AppManager.h"
#include "../appManager/AppManagerInterface.h"
#include "../dbManager/DBManagerInterface.h"

namespace Aurora
{
	void AppMgr_DBManager::OnRecvPacket( const PacketBase & pkt )
	{
// 		BEGINE_HANDLE_PACKET(EPacketInterface_AppManager);
// 			HADNLE_PACKET_(AppManagerInterface::EPacketIndex_WhoAmI, AppManagerInterface::PktWhoAmI);
// 		END_HANDLE_PACKET();
	}

	void AppMgr_DBManager::OnLoseConn()
	{
		LogFX();
	}

	void DBManagerManager::HandleWhoAmIPkt( NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt )
	{		
		if(m_pDBManager)
		{
			Log_WarningF("CONN ALREADY EXISTS!new conn's remote addr=[%s]\n",conn.GetRemoteAddr().ToString().c_str());
			return;
		}
	
		m_pDBManager =new AppMgr_DBManager(conn);
		if(!m_pDBManager)
		{
			Log_ErrorF("new AppMgr_DBManager FAILED\n");
			return;
		}
		
		Log_OKF("Get DBManager Connection ! addr=[%s]\n",conn.GetRemoteAddr().ToString().c_str());

		conn.SetPacketHandler(this);
		DBManagerInterface::PktWhoAmIResponse retPkt;
		retPkt.m_bSucc = true;
		retPkt.m_id = 0;
		conn.SendPacket(retPkt);
	}

	void DBManagerManager::OnLoseConn( NetworkConn & conn )
	{
		MY_ASSERT(m_pDBManager);
		m_pDBManager->OnLoseConn();
		SAFE_DELETE(m_pDBManager);

		m_theApp.OnLoseDBManager();
	}

	void DBManagerManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{

	}

	DBManagerManager::DBManagerManager()
		:m_theApp(AppManager::get_mutable_instance())
		,m_pDBManager(0)
	{

	}

	DBManagerManager::~DBManagerManager()
	{

	}

}
