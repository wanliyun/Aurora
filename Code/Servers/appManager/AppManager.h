#ifndef _LOGIN_APP_H_
#define _LOGIN_APP_H_

#include <ServerAppBase.h>
#include <boost/serialization/singleton.hpp>
#include <map>
#include "NetworkConn.h"
#include "AppManagerInterface.h"
namespace Aurora
{
	class AppManagerConfig;
	class AppMgr_App_Manager;
	class BaseAppManager;
	class CellAppManager;
	class LoginAppManager;
	class DBManagerManager;

	class AppManager : public ServerAppBase ,public boost::serialization::singleton<AppManager>
	{
	public:
		AppManager();
		~AppManager();
		AppManagerConfig & GetMyConfig(){return (AppManagerConfig &)GetAppConfig();}

		BaseAppManager & GetBaseAppMgr(){return  m_baseAppMgr;}
		CellAppManager & GetCellAppMgr(){return m_cellAppMgr;}
		LoginAppManager & GetLoginAppMgr(){return m_loginAppMgr;}

		DBManagerManager & GetDBMgrMgr(){return m_dbMgrAppMgr;}

		void OnLoseDBManager();

		void RefreshTitle()const;
	private:
		//AppBase;
		virtual void OnNetworkConnRecvPacket(NetworkConn & conn,const PacketBase & pkt);
		//~AppBase

		//override ServerAppBase;
		virtual bool OnInit();
		virtual void OnFini();
		virtual void OnNewConn(NetworkConn & newConn) ;
		virtual void OnTick(float fTimeDelta);
		virtual std::string GetAppName()const;
		virtual std::string GetAppVersion()const;
		//~override ServerAppBase;

	private:
		BaseAppManager & m_baseAppMgr;
		CellAppManager & m_cellAppMgr;
		LoginAppManager & m_loginAppMgr;
		DBManagerManager & m_dbMgrAppMgr;
	};
}

#endif
