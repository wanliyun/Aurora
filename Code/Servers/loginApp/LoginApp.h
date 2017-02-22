#ifndef _LOGIN_APP_H_
#define _LOGIN_APP_H_

#include <ServerAppBase.h>
#include <boost/serialization/singleton.hpp>
#include <map>
#include "LoginExternInterface.h"
#include <NetworkConn.h>

namespace Aurora
{
	class LoginAppConfig;
	class NetworkConn;
	class PacketBase;
	class LoginApp_AppManager;
	class ClientManager;
	class CRSACipher;

	class LoginApp : public ServerAppBase ,public boost::serialization::singleton<LoginApp>
	{
		friend class LoginApp_AppManager;
	public:
		LoginApp();
		~LoginApp();

		LoginAppConfig & GetMyConfig(){return (LoginAppConfig &) GetAppConfig();}
		bool IsServering()const{return m_state == EState_Servering;}

		LoginApp_AppManager & GetAppManager(){return m_appMgr;}
		NetworkConn * GetAppManagerConn();
		ClientManager & GetClntAppMgr(){return m_clntAppMgr;}

		CRSACipher		&	GetCRSACipher()const{return *m_pCRSACipher;}

		void OnLoseAppManager();

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
		LoginApp_AppManager & m_appMgr;
		ClientManager & m_clntAppMgr;
	private:
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
	private:
		CRSACipher		*	m_pCRSACipher;
	};
}

#endif
