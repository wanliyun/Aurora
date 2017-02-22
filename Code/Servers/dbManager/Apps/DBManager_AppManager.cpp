
#include "pch.h"
#include "DBManager_AppManager.h"
#include "DBManager.h"

namespace Aurora
{
	DBManager_AppManager::DBManager_AppManager(  )
		:m_theApp(DBManager::get_mutable_instance())
	{

	}

	bool DBManager_AppManager::ConnectAppMgr( const SocketAddress & addr )
	{
		MY_ASSERT(!GetConn());

		if(!DoConnect(m_theApp,addr))
		{
			Log_ErrorF("Connect AppManager[%s] Failed!\n",addr.ToString().c_str());
			return true;
		}
		Log_OKF("Connect AppManager[%s] Succeed! my addr=[%s]\n",addr.ToString().c_str(),GetConn()->GetRemoteAddr().ToString().c_str());
		GetConn()->SetPacketHandler(this);
		return true;
	}

	void DBManager_AppManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		MY_ASSERT(GetConn() == & conn);

		BEGINE_HANDLE_PACKET(EPacketInterface_DBManager);
			HADNLE_PACKET_(DBManagerInterface::EPacketIndex_WhoAmIResponse, DBManagerInterface::PktWhoAmIResponse);
			HADNLE_PACKET_(DBManagerInterface::EPacketIndex_Test, DBManagerInterface::PktTest);
		END_HANDLE_PACKET();
	}

	void DBManager_AppManager::HandlePacket( const DBManagerInterface::PktWhoAmIResponse & pkt )
	{
		LogF("PktWhoAmIResponse succ=%d,id=%d\n",pkt.m_bSucc,pkt.m_id);
		m_theApp.m_appId = pkt.m_id;
	}


	void DBManager_AppManager::HandlePacket( const DBManagerInterface::PktTest & pkt )
	{

	}

	void DBManager_AppManager::OnLoseConn( NetworkConn & conn )
	{
		m_theApp.OnLoseAppMgr();
	}

}
