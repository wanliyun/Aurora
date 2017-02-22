#ifndef _CELL_APP_H_
#define _CELL_APP_H_

#include <ServerAppBase.h>
#include <boost/serialization/singleton.hpp>
#include <map>
#include "CellInterface.h"
#include <NetworkConn.h>
#include "Apps/CellApp_AppManager.h"
namespace Aurora
{
	class CellAppConfig;
	class NetworkConn;
	class PacketBase;
	class CellApp_AppManager;
	class CellApp : public ServerAppBase ,public boost::serialization::singleton<CellApp>
	{
		friend class CellApp_AppManager;
	public:
		CellApp();
		~CellApp();

		CellAppConfig & GetMyConfig(){return (CellAppConfig &) GetAppConfig();}
		bool IsServering()const{return m_state == EState_Servering;}

		CellApp_AppManager & GetAppMgr(){return m_appMgr;}
		void RefreshTitle()const;

		virtual void OnLoseAppMgr();
	private:
		enum EState
		{
			EState_Dummy,
			EState_Registing,
			EState_Servering,
			EState_Closing,
		};
		EState m_state;
		CellApp_AppManager & m_appMgr;
	
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
