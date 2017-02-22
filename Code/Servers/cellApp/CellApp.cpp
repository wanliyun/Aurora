
#include "pch.h"
#include "CellApp.h"
#include <vector>
#include "../../Clients/Client/ClientInterface.h"
#include "CellAppConfig.h"
#include "apps/CellApp_BaseApp.h"
#include "Apps/CellApp_AppManager.h"
#include "CellInterface.h"
#include <EntityDef/EntityTypeManager.h>
#include "Entity/CellEntityManager.h"
#include "Space/CellManager.h"


namespace Aurora
{
	CellApp::CellApp()
		:ServerAppBase(CellAppConfig::get_mutable_instance())
		,m_appMgr(CellApp_AppManager::get_mutable_instance())
		,m_state(EState_Dummy)
	{

	}

	CellApp::~CellApp()
	{

	}

	bool CellApp::OnInit()
	{
		PyImportPaths path;
		path.addResPath(GetCellScriptDir());

		if(!PyScript::get_mutable_instance().Init(path,std::string("RunTime")))
		{
			Log_ErrorF("Failed to init python\n");
			return false;
		}

		if(!EntityTypeManager::get_mutable_instance().Init(std::string(GetClientScriptDir()),true,true))
			return false;

		if(!CellInterface::InitPacketFactory() )
			return false;

		CellAppConfig & config = GetMyConfig();
		if(!InitServer(config) )
			return false;
		m_state = EState_Registing;
	
		return true;
	}

	void CellApp::OnFini()
	{
		PyScript::get_mutable_instance().Fini();
	}

	void CellApp::OnNewConn( NetworkConn & newConn )
	{
		Log_NormalF("%s\n",newConn.GetRemoteAddr().ToString().c_str());
	}

	void CellApp::OnTick( float fTimeDelta )
	{
		PyScript::get_mutable_instance().Update(fTimeDelta);
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


	void CellApp::OnNetworkConnRecvPacket( NetworkConn & conn,const PacketBase & pkt )
	{
		if(pkt.GetHeader().GetInterface() != EPacketInterface_Cell)
		{
			Log_ErrorF("bad interface = %d\n",pkt.GetHeader().GetInterface() );
			return;
		}

		switch(pkt.GetHeader().GetIndex())
		{
		case CellInterface::EPacketIndex_WhoAmIOfBaseApp:
			{
				CellApp_BaseAppManager::get_mutable_instance().OnWhoAmIPkt(conn,pkt);
				RefreshTitle();
			}
			break;
		default:
			{
				Log_ErrorF("bad Packet Index = %d\n",pkt.GetHeader().GetIndex() );
				conn.Close();
			}
			break;
		}
	}


	void CellApp::onTick_Dummy( float fTimeDelta )
	{
		LogF(" tick = %d\n",getCurrentTick());
	}

	void CellApp::onTick_Registing( float fTimeDelta )
	{
		LogF(" tick = %d\n",getCurrentTick());
		MY_ASSERT( 0 == GetAppMgr().GetConn());
		CellAppConfig & config = GetMyConfig();

		if(GetAppMgr().DoConnect(SocketAddress(config.m_appMgrAddr,config.m_appMgrPort)))
		{
			m_state = EState_Servering;
		}
		else
		{
			m_state = EState_Closing;
		}
	}

	void CellApp::onTick_Servering( float fTimeDelta )
	{
		RefreshTitle();
		CellEntityManager::get_mutable_instance().OnTick(fTimeDelta);
		CellManager::get_mutable_instance().OnTick(fTimeDelta);
	}

	void CellApp::onTick_Closing( float fTimeDelta )
	{
		LogF(" tick = %d\n",getCurrentTick());
	}

	std::string CellApp::GetAppName() const
	{
		return std::string("CellApp");
	}

	std::string CellApp::GetAppVersion() const
	{
		return std::string("0.0.1");
	}

	void CellApp::RefreshTitle() const
	{
#ifdef WIN32
		char buf[MAX_PATH];
		sprintf_s(buf,sizeof(buf),"CellApp[%s] ID[%d] Cells[%u] Entities[%u]",
			GetAppVersion().c_str(),
			m_appId,
			CellManager::get_mutable_instance().GetNumCells(),
			CellEntityManager::get_mutable_instance().GetNumEntities());
		SetConsoleTitle(std::string(buf));
#endif
	}

	void CellApp::OnLoseAppMgr()
	{
		Log_NormalF("Lose AppManager! Will Exit!\n");
		m_state = EState_Closing;
		ShutDown(10);
	}

}
