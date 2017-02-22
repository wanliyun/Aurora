#ifndef _CLIENT_APP_H_
#define _CLIENT_APP_H_

#include <AppBase.h>
#include <network/Socket.h>
#include <NetworkConn.h>
#include "ClientAppConfig.h"
#include "Apps/ClientApp_BaseApp.h"
#include "Apps/ClientApp_LoginApp.h"
#include "../../Clients/Client/ClientInterface.h"


namespace Aurora
{
	class ClientApp_BaseApp;
	class ClientApp_LoginApp;
	class ClientEntityManager;
	
	class ClientApp : public AppBase ,public boost::serialization::singleton<ClientApp>
	{
		friend class ClientApp_BaseApp;
		friend class ClientApp_LoginApp;
		friend class ClientEntityManager;
		friend class ClientAppWarpper;
	public:
		ClientApp();
		~ClientApp();
	
		ClientApp_LoginApp & GetLoginApp(){return m_loginApp;}
		ClientApp_BaseApp & GetBaseApp(){return m_baseApp;}
		ClientAppConfig & GetMyConfig(){return (ClientAppConfig & )GetAppConfig();}

		enum EState
		{
			ES_DUMMY,
			ES_INITED,
			ES_LOGINAPP_CONNECTING,	//LoginApp连接中;
			ES_BASEAPP_CONNECTING,	
			ES_RUNNING,
			ES_SHUTING_DOWN,
		};

	public:
		bool StartLogin(const std::string & strName,const std::string strPassword);
		void Disconnect();
		void Stop();
	private:

		//override AppBase;
		virtual bool OnInit();
		virtual void PreRun() {}
		virtual void PostRun();
		virtual void OnFini();
		virtual void OnTick(float fTimeDelta);
		virtual std::string GetAppName()const;
		virtual std::string GetAppVersion()const;
		//~override AppBase;

		//////////////////////////////////////////////////////////////////////////
		//各种状态下的tick;
		void OnTick_Init				(float fTimeDelta);
		void OnTick_LoginApp_Connecting	(float fTimeDelta);
		void OnTick_BaseApp_Connecting	(float fTimeDelta);
		void OnTick_Running				(float fTimeDelta);
		void OnTick_Shuting_Down		(float fTimeDelta);
		//////////////////////////////////////////////////////////////////////////
	private:
		EState m_state;
	private:
		ClientApp_LoginApp  m_loginApp;
		ClientApp_BaseApp   m_baseApp;

		SocketAddress		m_servAddr;
	};
}

#endif
