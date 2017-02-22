
#include "pch.h"
#include "ClientApp.h"
#include "ClientInterface.h"
#include "../../Servers/loginApp/LoginExternInterface.h"
#include "Apps/ClientApp_BaseApp.h"
#include "Apps/ClientApp_LoginApp.h"
#include <EntityDef/EntityTypeManager.h>
#include "Scripts/Script_ClientEntity.h"
#include <boost/python.hpp>

#include <Entity/ClientEntity.h>
#include <EntityDef/EntityTypeManager.h>
#include <EntityDef/EntityType.h>
#include "Entity/ClientEntityManager.h"
#include "BlowfishCipher.h"


namespace Aurora
{
	ClientApp::ClientApp()
		:AppBase(ClientAppConfig::get_mutable_instance())
		,m_state(ES_DUMMY)
	{
	}

	ClientApp::~ClientApp()
	{

	}

	bool ClientApp::OnInit()
	{
		Log_OKF("\n");
		PyImportPaths path;
		path.addResPath(GetClientScriptDir());

		if(!PyScript::get_mutable_instance().Init(path,std::string("RunTime")))
		{
			Log_ErrorF("Init python\n");
			return false;
		}

		try
		{
			if(!EntityTypeManager::get_mutable_instance().Init(std::string(GetClientScriptDir()),true,true))
			{
				Log_ErrorF("Init EntityTypeManager\n");
				return false;
			}
		
			if(!GetLoginApp().Init())
			{
				Log_ErrorF("GetLoginApp().Init()\n");
				return false;
			}

			if(!ClientInterface::InitPacketFactory())
			{
				Log_ErrorF("ClientInterface::InitPacketFactory()\n");
				return false;
			}
		}
		catch(bpy::error_already_set e)
		{
			PyErr_Print();
		}

		m_state = ES_INITED;
		return true;
	}

	void ClientApp::OnFini()
	{
		
	}


	void ClientApp::OnTick( float fTimeDelta )
	{
		PyScript::get_mutable_instance().Update(fTimeDelta);
#ifdef WIN32
		::Sleep(16);//调试阶段：加上这句话,让客户端少耗CPU;
#endif
		switch(m_state)
		{
		case ES_DUMMY:break;
		case ES_INITED:							OnTick_Init					(fTimeDelta);break;
		case ES_LOGINAPP_CONNECTING:			OnTick_LoginApp_Connecting	(fTimeDelta);break;
		case ES_BASEAPP_CONNECTING:				OnTick_BaseApp_Connecting	(fTimeDelta);break;
		case ES_RUNNING:						OnTick_Running				(fTimeDelta);break;
		case ES_SHUTING_DOWN:					OnTick_Shuting_Down			(fTimeDelta);break;
		default:
			{
				MY_ASSERT(0);
			}
			break;
		}
	}

	std::string ClientApp::GetAppName() const
	{
		return std::string("ClientApp");
	}

	std::string ClientApp::GetAppVersion() const
	{
		return std::string("0.0.1");
	}

	void ClientApp::OnTick_Init( float fTimeDelta )
	{
		return;
	}

	void ClientApp::OnTick_LoginApp_Connecting( float fTimeDelta )
	{

	}

	void ClientApp::OnTick_BaseApp_Connecting( float fTimeDelta )
	{

	}

	void ClientApp::OnTick_Running( float fTimeDelta )
	{
		ClientEntityManager::get_mutable_instance().OnTick(fTimeDelta);
	}

	void ClientApp::OnTick_Shuting_Down( float fTimeDelta )
	{

	}

	bool ClientApp::StartLogin( const std::string & strName,const std::string strPassword )
	{
		if(m_state != ES_INITED)
			return false;

		return GetLoginApp().ConnectLoginServer(strName,strPassword);
	}

	void ClientApp::Disconnect()
	{

	}

	void ClientApp::Stop()
	{
		ShutDown();
	}

	void ClientApp::PostRun()
	{
		ClientEntityManager::get_mutable_instance().ReleaseAll();
		PyScript::get_mutable_instance().Fini();
		//GetLoginAppConn().GetSocket().close();
		Log_OKF("\n");
	}

}

