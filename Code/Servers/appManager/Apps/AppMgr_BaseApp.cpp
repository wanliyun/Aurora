
#include "pch.h"
#include "AppMgr_BaseApp.h"
#include "../baseApp/BaseInternInterface.h"
#include "../loginApp/LoginInternInterface.h"
#include "Entity/GlobalEntity.h"
#include "AppMgr_LoginApp.h"
#include "AppMgr_CellApp.h"
#include "Space/SpaceManager.h"
#include <memory_stream.h>
#include "Entity/AccountManager.h"

namespace Aurora
{
	BaseAppManager::BaseAppManager()
		:AppMgr_App_Manager("BaseApp")
	{

	}

	BaseAppManager::~BaseAppManager()
	{

	}

	bool BaseAppManager::OnWhoAmIPkt( NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt )
	{
		AppID nAppId;
		if(!m_appIDPool.GenerateUniqueID(nAppId))
		{
			Log_ErrorF(" generate id failed!\n");
			return false;
		}
		MY_ASSERT(!HasApp(nAppId));

		AppMgr_BaseApp * pApp = new AppMgr_BaseApp(nAppId,conn,pkt.m_internBindAddr,pkt.m_externBindAddr);
		conn.SetPacketHandler(this);

		//回复确认;
		{
			BaseInternInterface::PktWhoAmIResponse retPkt;
			retPkt.m_bSucc = true;
			retPkt.m_id = nAppId;

			{
				MemoryOStream os;
				for(Id2Owner_T::iterator iter = m_mapId2Owner.begin();iter != m_mapId2Owner.end();++iter)
				{
					AppMgr_BaseApp * pBaseApp = (AppMgr_BaseApp *)(iter->second);
					os<<pBaseApp->GetAppID()<<pBaseApp->m_internBindAddr;
				}
				retPkt.AppendToBuff(os.data(),os.size());
			}

			conn.SendPacket(retPkt);
		}

		//通知所有的BaseApp;
		{
			BaseInternInterface::PktNewBaseApp newPkt;
			newPkt.m_appID = pApp->GetAppID();
			newPkt.m_internBindAddr = pApp->m_internBindAddr;
			BroadcastToAllApps(newPkt);
		}

		//加入管理;
		{
			addConn(nAppId,conn,*pApp);
		}
		

		//通知所有的CellApp;
		{
			CellAppManager::get_mutable_instance().OnNewBaseApp(*pApp);
		}

		//发送场景信息;
		{
			BaseInternInterface::PktSyncAllSpaceInfo retPkt;
			MemoryOStream os;
			SpaceManager::get_mutable_instance().GetAllSpaceInfo().WriteToStream(os);
			retPkt.AppendToBuff(os.data(),os.size());
			conn.SendPacket(retPkt);
		}

		Log_OKF("New BaseApp!id=[%u] addr=[%s] intern=[%s] extern=[%s]\n",
			nAppId,conn.GetRemoteAddr().ToString().c_str(),pkt.m_internBindAddr.ToString().c_str(),pkt.m_externBindAddr.ToString().c_str());

		return true;
	}

// 	bool BaseAppManager::CreateAccount( PlayerEntity & player )
// 	{
// 		AppMgr_BaseApp * pTargetBaseApp = GetMinLoadApp();
// 
// 		if(pTargetBaseApp == 0)
// 		{
// 			LogF("failed for no base app\n");
// 			return false;
// 		}
// 
// 		BaseInternInterface::PktCreateAccountRquest newPkt;
// 		newPkt.m_strBlowfishKey = player.GetBlowfishKey();
// 		newPkt.m_strAccountName
// 		pTargetBaseApp->GetConn().SendPacket(newPkt);
// 		LogF("Sending PktCreatePlayerRquest to baseapp[%d]\n",pTargetBaseApp->GetAppID());
// 		return true;
//	}

	AppMgr_BaseApp * BaseAppManager::GetMinLoadApp()
	{
		AppMgr_BaseApp * pTargetBaseApp = 0;
		uint32 minEntities(-1);
		for(Id2Owner_T::iterator iter = m_mapId2Owner.begin();iter != m_mapId2Owner.end();++iter)
		{
			AppMgr_BaseApp * pBaseApp = (AppMgr_BaseApp *)(iter->second);
			MY_ASSERT(pBaseApp);
			if(pBaseApp->m_nNumEntities < minEntities)
			{
				minEntities = pBaseApp->m_nNumEntities;
				pTargetBaseApp = pBaseApp;
			}
		}
		return pTargetBaseApp;
	}
	void BaseAppManager::HandlePacket( const AppManagerInterface::PktBroadcastMsg & pkt )
	{
		BaseInternInterface::PktBroadcastMsg newPkt;
		memcpy(newPkt.m_msg,pkt.m_msg,sizeof(pkt.m_msg));

		BroadcastToAllApps(newPkt);

		LogF("PktBroadcastMsg  end\n");
	}

	void BaseAppManager::OnNewCellApp( AppMgr_CellApp & cellApp )
	{
		BaseInternInterface::PktNewCellApp newPkt;
		newPkt.m_appID = cellApp.GetAppID();
		newPkt.m_internBindAddr = cellApp.m_internBindAddr;
		BroadcastToAllApps(newPkt);
	}

	void BaseAppManager::BroadcastToAllApps( const PacketBase & pkt )
	{
		for(Conn2Owner_T::iterator iter = m_mapConn2Owner.begin();iter != m_mapConn2Owner.end();++iter)
		{
			MY_ASSERT(iter->first);
			iter->first->SendPacket(pkt);
		}
	}




	void AppMgr_BaseApp::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_AppManager);

		HADNLE_PACKET_(AppManagerInterface::EPacketIndex_NoticeClientLose, AppManagerInterface::PktNoticeClientLose);
		HADNLE_PACKET_(AppManagerInterface::EPacketIndex_CreatePlayerResponse, AppManagerInterface::PktCreatePlayerResponse);
		HADNLE_PACKET_(AppManagerInterface::EPacketIndex_BroadCast, AppManagerInterface::PktBroadcastMsg);
		HADNLE_PACKET_(AppManagerInterface::EPacketIndex_CreateEntityOnCell, AppManagerInterface::PktCreateEntityOnCell);

		END_HANDLE_PACKET();
	}

	void AppMgr_BaseApp::OnLoseConn()
	{
		LogFX();
	}

	void AppMgr_BaseApp::HandlePacket( const AppManagerInterface::PktNoticeClientLose & pkt )
	{
		LogF("PktNoticeClientLose\n");
	}

	void AppMgr_BaseApp::HandlePacket( const AppManagerInterface::PktCreatePlayerResponse & pkt )
	{
		Account * pAccount = AccountManager::get_mutable_instance().GetAccount(pkt.m_strAccountName);
		if(!pAccount)
		{
			Log_ErrorF("Can not find Account[%s]\n",pkt.m_strAccountName.c_str());
			return;
		}

		if(pAccount->m_state != Account::EState_WaitingBaseAppCreate)
		{
			Log_ErrorF("Account[%s] bad state=[%d]\n",pkt.m_strAccountName.c_str(),pAccount->m_state);
			return;
		}
		AppMgr_LoginApp * pApp = (AppMgr_LoginApp * )LoginAppManager::get_mutable_instance().GetApp(pAccount->m_loginAppID);
		if(!pApp)
		{
			Log_ErrorF("Account[%s] Lose LoginApp\n",pkt.m_strAccountName.c_str());
			return;
		}
		pAccount->m_state = Account::EState_Gaming;

		Log_ErrorF("Account[%s] ClntAppid=[%u] succ=[%s]\n",pkt.m_strAccountName.c_str(),pAccount->m_nClntAppId,
			pkt.m_bSucc ? "True" : "Fales");

		LoginInternInterface::PktLoginResponse newPkt;
		if(pkt.m_bSucc)
		{
			newPkt.m_code  = LoginInternInterface::PktLoginResponse::EC_Sucess;
			newPkt.m_clntAppID = pAccount->m_nClntAppId;

			newPkt.m_baseAppIp = m_externBindAddr.GetIP();
			newPkt.m_baseAppPort = m_externBindAddr.GetPort();

			//处理自己;
			++m_nNumEntities;
		}
		else
		{
			newPkt.m_code  = LoginInternInterface::PktLoginResponse::EC_Failed;
		}

		pApp->SendPacket(newPkt);

// 		GlobalEntityManager & entMgr = GlobalEntityManager::get_mutable_instance();
// 		GlobalEntity * pEnt = entMgr.GetEntity(pkt.m_eID);
// 		if(!pEnt)
// 		{
// 			LogF("Can not Find Entity id = %u\n",pkt.m_eID);
// 			return;
// 		}
// 		
// 		PlayerEntity *pPlayerEnt = (PlayerEntity * )pEnt;
// 		LoginAppManager & loginAppMgr = LoginAppManager::get_mutable_instance();
// 		AppMgr_LoginApp * pLoginApp = (AppMgr_LoginApp * )loginAppMgr.GetApp(pPlayerEnt->GetLoginAppId());
// 		if(!pLoginApp)
// 		{
// 			LogF("can not find login App\n");
// 			return;
// 		}
// 		LoginInternInterface::PktLoginResponse newPkt;
// 		newPkt.m_code  = pkt.m_bSucc ?  LoginInternInterface::PktLoginResponse::EC_Sucess :  LoginInternInterface::PktLoginResponse::EC_Failed;
// 		if(pkt.m_bSucc)
// 		{
// 			newPkt.m_nClntAppIdOfLoginApp = pPlayerEnt->GetClntAppIdOfLoginApp();
// 			newPkt.m_baseAppIp = m_externBindAddr.GetIP();
// 			newPkt.m_baseAppPort = m_externBindAddr.GetPort();
// 			newPkt.m_nPlayerId = pPlayerEnt->GetID();
// 			newPkt.m_strBlowfishKey = pPlayerEnt->GetBlowfishKey();
// 			pPlayerEnt->m_nBaseAppID = GetAppID();
// 			
// 			//处理自己;
// 			++m_nNumEntities;
// 		}
// 		pLoginApp->GetConn().SendPacket(newPkt);
	}

	void AppMgr_BaseApp::HandlePacket( const AppManagerInterface::PktBroadcastMsg & pkt )
	{
		LogF("PktBroadcastMsg\n");
		BaseAppManager::get_mutable_instance().HandlePacket(pkt);
	}

	void AppMgr_BaseApp::HandlePacket( const AppManagerInterface::PktCreateEntityOnCell & pkt )
	{
		Log_NormalF("PktCreateEntityOnCell(%u,%u)\n",pkt.m_id,pkt.m_tid);
		CellAppManager::get_mutable_instance().HandlePacket(pkt);
	}



}
