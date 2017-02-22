#ifndef _BASE_APP_H_
#define _BASE_APP_H_

#include <ServerAppBase.h>
#include <boost/serialization/singleton.hpp>
#include <map>
#include "BaseExternInterface.h"
#include <NetworkConn.h>
#include <ServerSpace.h>

#pragma warning(push)
#include <pqxx/pqxx>
#pragma warning(pop)

namespace Aurora
{
	class BaseAppConfig;
	class NetworkConn;
	class PacketBase;

	class BaseApp_ClientManager;
	class BaseApp_CellAppManager;
	class BaseApp_BaseAppManager;
	class BaseApp_AppManager;
	class BaseApp_DBManager;
	class ClientAppManager;
	class BaseEntityManager;

	class BaseApp : public ServerAppBase ,public boost::serialization::singleton<BaseApp>
	{
		friend class BaseApp_AppManager;
	public:
		BaseApp();
		~BaseApp();

		BaseAppConfig & GetMyConfig(){return (BaseAppConfig &) GetAppConfig();}
		bool IsServering()const{return m_state == EState_Servering;}

		BaseApp_ClientManager & GetClntMgr(){return m_clntMgr;}
		BaseApp_CellAppManager & GetCellAppMgr(){return m_cellAppMgr;}
		BaseApp_BaseAppManager & GetBaseAppMgr(){return m_baseAppMgr;}
		BaseApp_AppManager & GetAppMgr(){return m_appMgr;}
		BaseApp_DBManager & GetDBMgr(){return m_dbMgr;}
		BaseEntityManager & GetEntMgr(){return m_entityMgr;}
		SAllServerSpaceInfo & GetAllSpaceInfo(){return m_spaceInfo;}
		void RefreshTitle()const;

		void OnLoseAppManager();
	private:
		enum EState
		{
			EState_Dummy,
			EState_Registing,
			EState_Servering,
			EState_Closing,
		};
		EState m_state;
		BaseApp_ClientManager & m_clntMgr;
		BaseApp_CellAppManager & m_cellAppMgr;
		BaseApp_BaseAppManager & m_baseAppMgr;
		BaseApp_AppManager & m_appMgr;
		BaseApp_DBManager & m_dbMgr;
		BaseEntityManager & m_entityMgr;
		SAllServerSpaceInfo  m_spaceInfo;

		pqxx::connection  * m_pPQConn;
	private:
		//AppBase;
		virtual void OnNetworkConnRecvPacket(NetworkConn & conn,const PacketBase & pkt);
		//~AppBase

		//override ServerAppBase;
		virtual bool OnInit();
		virtual void OnFini();
		virtual void OnNewConn(NetworkConn & newConn);
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
