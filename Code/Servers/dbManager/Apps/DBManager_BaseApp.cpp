
#include "pch.h"
#include "DBManager_BaseApp.h"
#include "DBManager.h"
#include "../baseApp/BaseInternInterface.h"


namespace Aurora
{
	void DBManager_BaseApp::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_LoginExtern);
		HADNLE_PACKET_(DBManagerInterface::EPacketIndex_Test, DBManagerInterface::PktTest);
		END_HANDLE_PACKET();
	}

	void DBManager_BaseApp::HandlePacket( const DBManagerInterface::PktTest & pkt )
	{

	}

	DBManager_BaseAppManager::DBManager_BaseAppManager()
		:m_theApp(DBManager::get_mutable_instance())
	{

	}

	DBManager_BaseAppManager::~DBManager_BaseAppManager()
	{

	}

	DBManager_BaseApp * DBManager_BaseAppManager::GetBaseApp( AppID aid )
	{
		return (DBManager_BaseApp *)getOwner(aid);
	}

	DBManager_BaseApp * DBManager_BaseAppManager::GetBaseApp( NetworkConn & conn )
	{
		return (DBManager_BaseApp *)getOwner(conn);
	}

	void DBManager_BaseAppManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		DBManager_BaseApp * pBaseApp = GetBaseApp(conn);
		MY_ASSERT(pBaseApp);
		pBaseApp->OnRecvPacket(pkt);
	}

	void DBManager_BaseAppManager::OnLoseConn( NetworkConn & conn )
	{
		DBManager_BaseApp * pBaseApp = GetBaseApp(conn);
		MY_ASSERT(pBaseApp);
		pBaseApp->OnLoseConn();
	}

	void DBManager_BaseAppManager::HandlePacketWhoAmI( NetworkConn & newConn ,const DBManagerInterface::PktWhoAmI & pkt )
	{

	}

}
