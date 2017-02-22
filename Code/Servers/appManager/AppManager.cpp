
#include "pch.h"
#include "AppManager.h"
#include <NetworkConn.h>
#include "AppManagerConfig.h"
#include "Apps/AppMgr_LoginApp.h"
#include "Apps/AppMgr_BaseApp.h"
#include "Apps/AppMgr_CellApp.h"
#include "Apps/AppMgr_DBManager.h"
#include "Space/SpaceManager.h"


namespace Aurora
{
	AppManager::AppManager():
		ServerAppBase(AppManagerConfig::get_mutable_instance()),
		m_baseAppMgr(BaseAppManager::get_mutable_instance()),
		m_cellAppMgr(CellAppManager::get_mutable_instance()),
		m_loginAppMgr(LoginAppManager::get_mutable_instance()),
		m_dbMgrAppMgr(DBManagerManager::get_mutable_instance())
	{

	}

	AppManager::~AppManager()
	{

	}

	bool AppManager::OnInit()
	{
		if(!AppManagerInterface::InitPacketFactory())
		{
			Log_CriticalF("AppManagerInterface::InitPacketFactory()\n");
			return false;
		}
		AppManagerConfig & theConfig = GetMyConfig();
		if(!InitServer( theConfig) )
		{
			Log_CriticalF("InitServer( theConfig)\n");
			return false;
		}
		if(!SpaceManager::get_mutable_instance().Init())
		{
			Log_CriticalF("SpaceManager::get_mutable_instance().Init()\n");
			return false;
		}
		Log_OKF("Init Succeed! Waiting for apps\n");
		return true;
	}

	void AppManager::OnFini()
	{
		std::map<SOCKET,NetworkConn * > tmp;
		tmp.swap(m_mapConns);
	}

	void AppManager::OnTick( float fTimeDelta )
	{
		RefreshTitle();
		SpaceManager::get_mutable_instance().OnTick(fTimeDelta);
		CellAppManager::get_mutable_instance().OnTick(fTimeDelta);
	}

	void AppManager::OnNewConn( NetworkConn & newConn )
	{
		LogF("%s\n",newConn.GetRemoteAddr().ToString().c_str());
	}

	void AppManager::OnNetworkConnRecvPacket( NetworkConn & conn,const PacketBase & pkt__ )
	{
		MY_ASSERT(conn.GetPacketHandler() == 0);

		if(pkt__.GetHeader().GetInterface() != EPacketInterface_AppManager || 
			pkt__.GetHeader().GetIndex() != AppManagerInterface::EPacketIndex_WhoAmI)
		{
			Log_ErrorF("Bad Packet! from addr=[%s]\n",conn.GetRemoteAddr().ToString().c_str());
			return;
		}
		
		AppManagerInterface::PktWhoAmI & pkt = (AppManagerInterface::PktWhoAmI&)pkt__;
		
		switch(pkt.m_AppType)
		{
		case AppManagerInterface::PktWhoAmI::EAppType_LoginApp:		((AppMgr_App_Manager &)m_loginAppMgr).OnWhoAmIPkt(conn,pkt);	break;
		case AppManagerInterface::PktWhoAmI::EAppType_BaseApp:		((AppMgr_App_Manager &)m_baseAppMgr).OnWhoAmIPkt(conn,pkt);		break;
		case AppManagerInterface::PktWhoAmI::EAppType_CellApp:		((AppMgr_App_Manager &)m_cellAppMgr).OnWhoAmIPkt(conn,pkt);		break;
		case AppManagerInterface::PktWhoAmI::EAppType_DBManager:	m_dbMgrAppMgr.HandleWhoAmIPkt(conn,pkt);	break;
		default:
			{
				Log_ErrorF("Unkonwn appType=[%u] from [%s]\n",pkt.m_AppType,conn.GetRemoteAddr().ToString().c_str());
			}
			break;
		}
	}

	std::string AppManager::GetAppName() const
	{
		return std::string("AppManager");
	}

	std::string AppManager::GetAppVersion() const
	{
		return std::string("0.0.1");
	}

	void AppManager::RefreshTitle()const
	{
#ifdef WIN32
		char buf[MAX_PATH];
		sprintf_s(buf,sizeof(buf),"AppManager[%s] Login[%u] Base[%u] Cell[%u] DB[%s]",GetAppVersion().c_str(),m_loginAppMgr.GetNumConns(),
			m_baseAppMgr.GetNumConns(),m_cellAppMgr.GetNumConns(),m_dbMgrAppMgr.HasDBManager() ? "on" : "off" );
		SetConsoleTitle(std::string(buf));
#endif
	}

	void AppManager::OnLoseDBManager()
	{

	}

}
