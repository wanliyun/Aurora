
#include "pch.h"
#include "BaseApp.h"
#include <vector>
#include "../../Clients/Client/ClientInterface.cpp"
#include "../appManager/AppManagerInterface.h"
#include "BaseAppConfig.h"

#include "Apps/BaseApp_Client.h"
#include "Apps/BaseApp_AppManager.h"
#include "Apps/BaseApp_CellApp.h"
#include "Apps/BaseApp_BaseApp.h"
#include "Apps/BaseApp_DBManager.h"

#include "BaseInternInterface.h"
#include "Entity/BaseEntity.h"
#include <EntityDef/EntityTypeManager.h>
#include "Entity/BaseEntityManager.h"



namespace Aurora
{
	BaseApp::BaseApp()
		:ServerAppBase(BaseAppConfig::get_mutable_instance())
		,m_state(EState_Dummy)
		,m_entityMgr(BaseEntityManager::get_mutable_instance())
		,m_clntMgr(BaseApp_ClientManager::get_mutable_instance())
		,m_cellAppMgr(BaseApp_CellAppManager::get_mutable_instance())
		,m_appMgr(BaseApp_AppManager::get_mutable_instance())
		,m_dbMgr(BaseApp_DBManager::get_mutable_instance())
		,m_baseAppMgr(BaseApp_BaseAppManager::get_mutable_instance())
		,m_pPQConn(0)
	{

	}

	BaseApp::~BaseApp()
	{
		SAFE_DELETE(m_pPQConn);
	}
	void trimStr(std::string & str)
	{
		str = str.substr(0,str.find_last_not_of(" \t")+1);
	}
	bool BaseApp::OnInit()
	{
		PyImportPaths path;
		path.addResPath(GetBaseScriptDir());

		if(!PyScript::get_mutable_instance().Init(path,std::string("RunTime")))
		{
			Log_ErrorF("Failed to init python\n");
			return false;
		}


		if(!EntityTypeManager::get_mutable_instance().Init(std::string(GetBaseScriptDir()),true,true))
		{
			Log_ErrorF("EntityTypeManager::get_mutable_instance().Init()\n");
			return false;
		}

		if(!BaseExternInterface::InitPacketFactory() )
		{
			Log_ErrorF("BaseExternInterface::InitPacketFactory()\n");
			return false;
		}
		if(!BaseInternInterface::InitPacketFactory())
		{
			Log_ErrorF("BaseInternInterface::InitPacketFactory()\n");
			return false;
		}
		
		BaseAppConfig & config = GetMyConfig();

		MY_ASSERT(m_pPQConn == 0);

		try
		{
#if 0
			m_pPQConn = new pqxx::connection(config.m_dbConnStr);

			pqxx::work w(*m_pPQConn);
			pqxx::result ret = w.exec("select * from t_test");
			w.commit();
			for(size_t nRow = 0 ;nRow < ret.affected_rows();++nRow)
			{
				pqxx::tuple row = ret[nRow];
				std::string str(row["desc"].c_str());
				trimStr(str);
				Log_HackF("id[%s]\tdesc[%s]\n",row["id"].c_str(),str.c_str());
			}
#endif
		}
		catch(const std::exception & e)
		{
			Log_ErrorF("exception = %s\n",e.what());
			return false;
		}
		
		if(!InitServer(config) )
		{
			Log_ErrorF("InitServer(config)\n");
			return false;
		}
		m_state = EState_Registing;
		RefreshTitle();
		return true;
	}

	void BaseApp::OnFini()
	{
		PyScript::get_mutable_instance().Fini();
	}

	void BaseApp::OnNewConn( NetworkConn & newConn)
	{

	}

	void BaseApp::OnTick( float fTimeDelta )
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


	void BaseApp::OnNetworkConnRecvPacket( NetworkConn & conn,const PacketBase & pkt )
	{
		MY_ASSERT( 0 == conn.GetPacketHandler());

		if(conn.IsExtern()) //客户端来的连接;
		{
			if(pkt.GetHeader().GetIndex() == BaseExternInterface::EPacketIndex_WhoAmI)
			{
				m_clntMgr.OnWhoAmIPkt(conn,(BaseExternInterface::PktWhoAmI &)pkt);
			}
			else
			{
				Log_ErrorF("BAD PACKET from extern!conn's remote addr=[%s]",conn.GetRemoteAddr().ToString().c_str());
				conn.Close();
			}
		}
		else//其他BaseApp的新连接;
		{
			if(pkt.GetHeader().GetIndex() == BaseInternInterface::EPacketIndex_BaseApp_WhoAmI)
			{
				m_baseAppMgr.OnWhoAmIPkt(conn,(BaseExternInterface::PktWhoAmI &)pkt);
			}
			else
			{
				Log_ErrorF("BAD PACKET from intern! conn's remote addr=[%s]",conn.GetRemoteAddr().ToString().c_str());
				conn.Close();
			}
		}
	}

	void BaseApp::onTick_Dummy( float /*fTimeDelta*/ )
	{
		LogF("tick=%d\n",getCurrentTick());
	}

	void BaseApp::onTick_Registing( float /*fTimeDelta*/ )
	{
		LogF("tick=%d\n",getCurrentTick());

		BaseAppConfig & config = GetMyConfig();
		
		if(m_appMgr.DoConnect(SocketAddress(config.m_appMgrAddr.c_str(),config.m_appMgrPort)))
		{
			m_state = EState_Servering;
		}
		else
		{
			m_state = EState_Closing;
		}
	}

	void BaseApp::onTick_Servering( float fTimeDelta )
	{
		m_entityMgr.OnTick(fTimeDelta);
		//cout<<"onTick_Servering"<<getCurrentTick()<<endl;
	}

	void BaseApp::onTick_Closing( float /*fTimeDelta*/ )
	{
		LogF("tick=%d\n",getCurrentTick());
	}

	std::string BaseApp::GetAppName() const
	{
		return std::string("baseApp");
	}

	std::string BaseApp::GetAppVersion() const
	{
		return std::string("0.0.1");
	}

	void BaseApp::RefreshTitle() const
	{
#if WIN32
		char buf[MAX_PATH];
		sprintf_s(buf,sizeof(buf),"BaseApp[%s] ID[%d] Players[%u] InComingPlayers[%u] GlobalEntities[%u]",
			GetAppVersion().c_str(),
			m_appId,
			m_clntMgr.GetNumConns(),
			m_entityMgr.GetNumEntities(),
			m_entityMgr.GetNumEntities());
		SetConsoleTitle(std::string(buf));
#endif
	}

	void BaseApp::OnLoseAppManager()
	{
		Log_NormalF("Lose AppManager! Will Exit!\n");
		m_state = EState_Closing;
		ShutDown(10);
	}

}
