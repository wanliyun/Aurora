
#include "pch.h"
#include "AppMgr_App.h"

namespace Aurora
{
	AppMgr_App_Manager::AppMgr_App_Manager(const std::string & strAppName)
		:m_strAppName(strAppName)
		,m_appIDPool(m_mapId2Owner,1,AppID(-1),BAD_APP_ID)
	{

	}

	AppMgr_App_Manager::~AppMgr_App_Manager()
	{

	}

	void AppMgr_App_Manager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		AppMgr_App * pApp = GetApp(conn);
		MY_ASSERT(0 != pApp);
		pApp->OnRecvPacket(pkt);
	}

	void AppMgr_App_Manager::OnLoseConn( NetworkConn & conn )
	{
		AppMgr_App * pApp = GetApp(conn);
		MY_ASSERT(0 != pApp);
		pApp->OnLoseConn();
		Log_OKF("Lose %s!id=[%u] addr=[%s]\n",m_strAppName.c_str(),pApp->GetAppID(),conn.GetRemoteAddr().ToString().c_str());
		//Йѕіэ№ЬАн;
		delConn(conn);
	}

}
