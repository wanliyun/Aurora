
#include "pch.h"
#include "AppMgr_LoginApp.h"
#include "../loginApp/LoginInternInterface.h"
#include "entity/GlobalEntity.h"
#include "AppMgr_BaseApp.h"
#include "AppManager.h"
#include "BlowfishCipher.h"
#include "Entity/AccountManager.h"
#include "Apps/AppMgr_BaseApp.h"
#include "../baseApp/BaseInternInterface.h"

namespace Aurora
{
	LoginAppManager::LoginAppManager()
		:AppMgr_App_Manager("LoginApp")
	{

	}

	LoginAppManager::~LoginAppManager()
	{

	}

	bool LoginAppManager::OnWhoAmIPkt( NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt )
	{
		AppID nAppId;
		if(!m_appIDPool.GenerateUniqueID(nAppId))
		{
			Log_ErrorF(" generate id failed!\n");
			return false;
		}
		MY_ASSERT(!HasApp(nAppId));

		AppMgr_LoginApp * pApp = new AppMgr_LoginApp(nAppId,conn);
		if(!pApp)
		{
			Log_ErrorF("Failed! new AppMgr_LoginApp\n");
			return false;
		}
		
		Log_OKF("New LoginApp!id=[%u] addr=[%s] extern=[%s]\n",nAppId,conn.GetRemoteAddr().ToString().c_str(),pkt.m_externBindAddr.ToString().c_str());

		conn.SetPacketHandler(this);
		addConn(nAppId,conn,*pApp);

		LoginInternInterface::PktWhoAmIResponse retPkt;
		retPkt.m_bSucc = true;
		retPkt.m_id = nAppId;
		conn.SendPacket(retPkt);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	void AppMgr_LoginApp::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_AppManager);
			HADNLE_PACKET_(AppManagerInterface::EPacketIndex_LoginRquest, AppManagerInterface::PktLoginRquest);
		END_HANDLE_PACKET();
	}

	void AppMgr_LoginApp::OnLoseConn()
	{
		LogF("");
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void AppMgr_LoginApp::HandlePacket( const AppManagerInterface::PktLoginRquest & pkt )
	{
		bool bSucc = false;
		for(;;)
		{
			BaseAppManager & baseAppMgr = AppManager::get_mutable_instance().GetBaseAppMgr();
			AppMgr_BaseApp * pBaseapp = baseAppMgr.GetMinLoadApp();
			if(!pBaseapp)
			{
				LogF("user[%s] login Failed for baseAppMgr.GetMinLoadApp() is NULl\n",pkt.m_strUserName.c_str());
				break;
			}

			Account * pAccount = AccountManager::get_mutable_instance().AddAccount(pkt.m_strUserName,GetAppID(),pkt.m_clntID);
			if(!pAccount)
			{
				LogF("user[%s] login Failed for AddAccount()\n",pkt.m_strUserName.c_str());
				break;
			}

			BaseInternInterface::PktCreateAccountRquest newPkt;
			newPkt.m_strAccountName = pkt.m_strUserName;
			newPkt.m_strBlowfishKey = pkt.m_strBlowfishKey;
			pBaseapp->SendPacket(newPkt);

			pAccount->m_state = Account::EState_WaitingBaseAppCreate;

			bSucc = true;

			break;
		}

		if(!bSucc)
		{
			LoginInternInterface::PktLoginResponse pktRet;
			pktRet.m_code = LoginInternInterface::PktLoginResponse::EC_Failed;
			pktRet.m_clntAppID = pkt.m_clntID;
			SendPacket(pktRet);
		}
	}


	//////////////////////////////////////////////////////////////////////////


}
