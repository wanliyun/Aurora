
#include "pch.h"
#include "AppMgr_CellApp.h"
#include "../cellApp/CellInterface.h"
#include "Entity/GlobalEntity.h"
#include "AppMgr_BaseApp.h"
#include "../baseApp/BaseInternInterface.h"
#include <memory_stream.h>
#include "Space/SpaceManager.h"

namespace Aurora
{
	CellAppManager::CellAppManager()
		:AppMgr_App_Manager("CellApp")
	{

	}

	CellAppManager::~CellAppManager()
	{

	}

	bool CellAppManager::OnWhoAmIPkt( NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt )
	{
		AppID nAppId;
		if(!m_appIDPool.GenerateUniqueID(nAppId))
		{
			LogF(" generate id failed!\n");
			return false;
		}
		MY_ASSERT(!HasApp(nAppId));

		AppMgr_CellApp *pApp =new AppMgr_CellApp(nAppId,conn,pkt.m_internBindAddr);
		
		if(!pApp)
		{
			Log_ErrorF("Failed ! new AppMgr_CellApp()\n");
			return false;
		}

		Log_OKF("New CellApp!id=[%u] addr=[%s] intern=[%s]\n",nAppId,conn.GetRemoteAddr().ToString().c_str(),pkt.m_internBindAddr.ToString().c_str());

		conn.SetPacketHandler(this);
		
		//添加管理;
		{
			addConn(nAppId,conn,*pApp);
		}
		

		//回复;
		{
			CellInterface::PktWhoAmIResponse retPkt;
			retPkt.m_bSucc = true;
			retPkt.m_id = nAppId;
			conn.SendPacket(retPkt);
		}

		//同步SpaceInfo;
		{
			CellInterface::PktSyncAllSpaceInfo retPkt;
			MemoryOStream os;
			SpaceManager::get_mutable_instance().GetAllSpaceInfo().WriteToStream(os);
			retPkt.AppendToBuff(os.data(),os.size());

			conn.SendPacket(retPkt);
		}


		//通知所有的BaseApp;
		BaseAppManager::get_mutable_instance().OnNewCellApp(*pApp);

		return true;
	}

	void CellAppManager::HandlePacket( const AppManagerInterface::PktCreateEntityOnCell & pkt )
	{
// 		GlobalEntity * pEntity = GlobalEntityManager::get_mutable_instance().GetEntity(pkt.m_id);
// 		if(!pEntity)
// 		{
// 			Log_NormalF("PktCreateEntityOnCell Can not find entity[%u]\n",pkt.m_id);
// 			return;
// 		}

		//pEntity->CreateCellEntity();
	}

	void CellAppManager::OnNewBaseApp( AppMgr_BaseApp & baseApp )
	{
		for(Id2Owner_T::iterator iter = m_mapId2Owner.begin();iter != m_mapId2Owner.end();++iter)
		{
			AppMgr_CellApp * pCellApp = (AppMgr_CellApp *)(iter->second);
			MY_ASSERT(pCellApp);

			BaseInternInterface::PktNewCellApp newPkt;
			newPkt.m_appID = pCellApp->GetAppID();
			newPkt.m_internBindAddr = pCellApp->m_internBindAddr;
			baseApp.SendPacket(newPkt);
		}
	}

	void CellAppManager::OnTick( float fDelta )
	{
		bool bSpaceInfoChagne = false;
		//分配初始化场景;
		{
			SAllServerSpaceInfo & allInfo = SpaceManager::get_mutable_instance().GetAllSpaceInfo();
			for(SAllServerSpaceInfo::MapSpaceT::iterator iter =allInfo.m_mapSpaces.begin();iter != allInfo.m_mapSpaces.end();++iter)
			{
				SSpaceInfo & info = iter->second;
				if(!info.m_bIsMain)
					continue;
				if(!info.m_mapChannels.empty())
					continue;

				AppMgr_CellApp * pApp = GetMinLoadApp();
				if(!pApp)
					return;

				info.m_mapChannels.insert(std::make_pair(0,SChannelInfo()));
				SChannelInfo & cinfo = info.m_mapChannels[0];
				cinfo.m_mapCells.insert( std::make_pair(0,SCellInfo() ));
				SCellInfo & cellInfo = cinfo.m_mapCells[0];
				cellInfo.m_cellAppId = pApp->GetAppID();
				bSpaceInfoChagne = true;
			}
		}

		if(bSpaceInfoChagne)
		{
			MemoryOStream os;
			SpaceManager::get_mutable_instance().GetAllSpaceInfo().WriteToStream(os);


			//通知所有CellApp;
			{
				CellInterface::PktSyncAllSpaceInfo pkt;
				pkt.AppendToBuff(os.data(),os.size());
				BroadcastToAllApps(pkt);
			}

			//通知所有的BaseApp;
			{
				BaseInternInterface::PktSyncAllSpaceInfo pkt;
				pkt.AppendToBuff((char*)os.data(),os.size());
				BaseAppManager::get_mutable_instance().BroadcastToAllApps(pkt);
			}

		}
		return;
	}

	AppMgr_CellApp * CellAppManager::GetMinLoadApp()
	{
		if(m_mapId2Owner.empty())
			return 0 ;
		return (AppMgr_CellApp *)(m_mapId2Owner.begin()->second);
	}

	void CellAppManager::BroadcastToAllApps( const PacketBase & pkt )
	{
		for(Conn2Owner_T::iterator iter = m_mapConn2Owner.begin();iter != m_mapConn2Owner.end();++iter)
		{
			MY_ASSERT(iter->first);
			iter->first->SendPacket(pkt);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	void AppMgr_CellApp::OnRecvPacket( const PacketBase & pkt )
	{
		//BEGINE_HANDLE_PACKET(EPacketInterface_AppManager);
		//	HADNLE_PACKET_(AppManagerInterface::EPacketIndex_LoginRquest, AppManagerInterface::PktLoginRquest);
		//END_HANDLE_PACKET();
	}

	void AppMgr_CellApp::OnLoseConn()
	{
		LogF("");
	}
}
