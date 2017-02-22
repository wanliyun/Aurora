
#include "pch.h"
#include "DBManager.h"
#include <vector>
#include "../../Clients/Client/ClientInterface.h"
#include "../appManager/AppManagerInterface.h"
#include "DBManagerConfig.h"
#include "Apps/DBManager_BaseApp.h"
#include "Apps/DBManager_AppManager.h"

namespace Aurora
{
	DBManager::DBManager()
		:ServerAppBase(DBManagerConfig::get_mutable_instance())
		,m_state(EState_Dummy)
		,m_appMgr(DBManager_AppManager::get_mutable_instance())
		,m_baseAppMgr(DBManager_BaseAppManager::get_mutable_instance())
	{

	}

	DBManager::~DBManager()
	{
	}

	bool DBManager::OnInit()
	{
		if(!DBManagerInterface::InitPacketFactory() )
			return false;

		DBManagerConfig & config = GetMyConfig();
		if(!InitServer(config))
			return false;
		m_state = EState_Registing;
	
		return true;
	}

	void DBManager::OnFini()
	{

	}

	void DBManager::OnNewConn( NetworkConn & newConn )
	{
		Log_NormalF("remote addr=[%s]\n",newConn.GetRemoteAddr().ToString().c_str());
	}

	void DBManager::OnTick( float fTimeDelta )
	{
		RefreshTitle();
		//TickConnList(fTimeDelta);
		
		switch(m_state)
		{
		case EState_Dummy:onTick_Dummy(fTimeDelta);break;
		case EState_Registing:onTick_Registing(fTimeDelta);break;
		case EState_Servering:onTick_Servering(fTimeDelta);break;
		case EState_Closing:onTick_Closing(fTimeDelta);break;
		default:MY_ASSERT(0);break;
		}
	}

	void DBManager::OnLoseAppMgr()
	{
		Log_NormalF("Lose AppManager! Will Exit!\n");
		m_state = EState_Closing;
		ShutDown(10);
	}

	void DBManager::OnNetworkConnRecvPacket( NetworkConn & conn,const PacketBase & pkt )
	{
		if( pkt.GetHeader().GetInterface() != EPacketInterface_DBManager
			||pkt.GetHeader().GetIndex() != DBManagerInterface::EPacketIndex_WhoAmI ) 
		{
			Log_ErrorF("BAD PACKET !remote addr=[%s]\n",conn.GetRemoteAddr().ToString().c_str());
			conn.Close();
			return;
		}
		m_baseAppMgr.HandlePacketWhoAmI(conn,(const DBManagerInterface::PktWhoAmI & )pkt);
	}

	void DBManager::onTick_Dummy( float fTimeDelta )
	{
		LogF("tick=%d\n",getCurrentTick());
	}

	void DBManager::onTick_Registing( float fTimeDelta )
	{
		LogF("tick=%d\n",getCurrentTick());

		DBManagerConfig & config = GetMyConfig();
		if(m_appMgr.ConnectAppMgr(SocketAddress(config.m_appMgrAddr.c_str(),config.m_appMgrPort)))
		{
			m_state = EState_Servering;
			AppManagerInterface::PktWhoAmI pkt;
			pkt.m_AppType = AppManagerInterface::PktWhoAmI::EAppType_DBManager;
			m_appMgr.SendPacket(pkt);
			LogF("SENDING WHOAMI\n");
		}
		else
		{
			m_state = EState_Closing;
		}
	}

	void DBManager::onTick_Servering( float fTimeDelta )
	{
		//cout<<"onTick_Servering"<<getCurrentTick()<<endl;
	}

	void DBManager::onTick_Closing( float fTimeDelta )
	{
		LogF("tick=%d\n",getCurrentTick());
	}

	std::string DBManager::GetAppName() const
	{
		return std::string("DBManager");
	}

	std::string DBManager::GetAppVersion() const
	{
		return std::string("0.0.1");
	}

	void DBManager::RefreshTitle() const
	{
#ifdef WIN32
		char buf[MAX_PATH];
		sprintf_s(buf,sizeof(buf),"DBManager[%s]",
			GetAppVersion().c_str());
		SetConsoleTitle(std::string(buf));
#endif
	}
}
