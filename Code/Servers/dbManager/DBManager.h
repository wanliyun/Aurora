#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#include <ServerAppBase.h>
#include <boost/serialization/singleton.hpp>
#include <map>
#include "DBManagerInterface.h"
#include <NetworkConn.h>

namespace Aurora
{
	class DBManagerConfig;
	class NetworkConn;
	class PacketBase;
	class DBManager_AppManager;
	class DBManager_BaseApp;
	class DBManager_BaseAppManager;
	class DBManager : public ServerAppBase ,public boost::serialization::singleton<DBManager>
	{
		friend class DBManager_AppManager;
	public:
		DBManager();
		~DBManager();

		DBManagerConfig & GetMyConfig(){return (DBManagerConfig &) GetAppConfig();}
		bool IsServering()const{return m_state == EState_Servering;}

		DBManager_AppManager & GetAppMgr(){return m_appMgr;}
		DBManager_BaseAppManager & GetBaseAppMgr(){return m_baseAppMgr;}

		void OnLoseAppMgr();

		void RefreshTitle()const;

	private:
		enum EState
		{
			EState_Dummy,
			EState_Registing,
			EState_Servering,
			EState_Closing,
		};
		EState m_state;
		DBManager_AppManager & m_appMgr;
		DBManager_BaseAppManager & m_baseAppMgr;
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

		//////////////////////////////////////////////////////////////////////////
		//×´Ì¬»ú;
		void onTick_Dummy(float fTimeDelta);
		void onTick_Registing(float fTimeDelta);
		void onTick_Servering(float fTimeDelta);
		void onTick_Closing(float fTimeDelta);
		//////////////////////////////////////////////////////////////////////////
	};
}

#endif
