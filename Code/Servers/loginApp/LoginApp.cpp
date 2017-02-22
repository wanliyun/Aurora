
#include "pch.h"
#include "LoginApp.h"
#include <vector>
#include "../../Clients/Client/ClientInterface.h"
#include "../appManager/AppManagerInterface.h"
#include "LoginAppConfig.h"
#include "apps/LoginApp_Client.h"
#include "Apps/LoginApp_AppManager.h"
#include "LoginInternInterface.h"
#include "Apps/LoginApp_Client.h"
#include <EntityDef/EntityTypeManager.h>
#include "RSACipher.h"
#include "FileSystem.h"

namespace Aurora
{
	LoginApp::LoginApp()
		:ServerAppBase(LoginAppConfig::get_mutable_instance())
		,m_state(EState_Dummy)
		,m_appMgr(LoginApp_AppManager::get_mutable_instance())
		,m_clntAppMgr(ClientManager::get_mutable_instance())
		,m_pCRSACipher(0)
	{

	}

	LoginApp::~LoginApp()
	{
		if(m_pCRSACipher)
		{
			CRSACipher::Destroy(m_pCRSACipher);
			m_pCRSACipher = 0;
		}
	}

	bool LoginApp::OnInit()
	{
		if(!LoginExternInterface::InitPacketFactory() )
			return false;
		if(!LoginInternInterface::InitPacketFactory())
			return false;

		LoginAppConfig & config = GetMyConfig();
		if(!InitServer(config) )
			return false;

		//≥ı ºªØRSACipher;
		{
			MY_ASSERT(!m_pCRSACipher);
			std::string realkeyPath = FileSystem::get_mutable_instance().FindFilePath(config.m_strPriKeyPath);
			m_pCRSACipher = CRSACipher::Create(realkeyPath,false);
			if(!m_pCRSACipher)
				return false;
		}

		m_state = EState_Registing;
	
		return true;
	}

	void LoginApp::OnFini()
	{

	}

	void LoginApp::OnNewConn( NetworkConn & newConn )
	{
		LogFX();
		m_clntAppMgr.OnNewConn(newConn);
	}

	void LoginApp::OnTick( float fTimeDelta )
	{
		RefreshTitle();
		
		switch(m_state)
		{
		case EState_Dummy:onTick_Dummy(fTimeDelta);break;
		case EState_Registing:onTick_Registing(fTimeDelta);break;
		case EState_Servering:onTick_Servering(fTimeDelta);break;
		case EState_Closing:onTick_Closing(fTimeDelta);break;
		default:MY_ASSERT(0);break;
		}
	}

	void LoginApp::onTick_Dummy( float fTimeDelta )
	{
		LogF("tick=%d\n",getCurrentTick());
	}

	void LoginApp::onTick_Registing( float fTimeDelta )
	{
		LogF("tick=%d\n",getCurrentTick());

		LoginAppConfig & config = GetMyConfig();

		if(!m_appMgr.DoConnect(SocketAddress(config.m_appMgrAddr,config.m_appMgrPort)))
		{
			m_state = EState_Closing;
		}
		else
		{
			m_state = EState_Servering;
		}
	}

	void LoginApp::onTick_Servering( float fTimeDelta )
	{
// 		if(!.Tick(fTimeDelta))
// 		{
// 			LogF("APPMGR lose\n");
// 			m_state = EState_Closing;
// 		}
	}

	void LoginApp::onTick_Closing( float fTimeDelta )
	{
		LogF("tick=%d\n",getCurrentTick());
	}

	std::string LoginApp::GetAppName() const
	{
		return std::string("LoginApp");
	}

	std::string LoginApp::GetAppVersion() const
	{
		return std::string("0.0.1");
	}

	void LoginApp::RefreshTitle() const
	{
#ifdef WIN32
		char buf[MAX_PATH];
		sprintf_s(buf,sizeof(buf),"LoginApp[%s] ID[%d] Connections[%u]  Handled[%u]",
			GetAppVersion().c_str(),
			m_appId,
			0,0);
		SetConsoleTitle(std::string(buf));
#endif
	}

	void LoginApp::OnLoseAppManager()
	{
		Log_NormalF("Lose AppManager! Will Exit!\n");
		m_state = EState_Closing;
		ShutDown(10);
	}

	NetworkConn * LoginApp::GetAppManagerConn()
	{
		return m_appMgr.GetConn();
	}

}
