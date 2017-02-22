#ifndef _APP_MGR_CELL_APP_H_
#define _APP_MGR_CELL_APP_H_

#include "AppMgr_App.h"

namespace Aurora
{
	class AppMgr_BaseApp;
	class CellAppManager;
	class AppMgr_CellApp : public AppMgr_App
	{
		friend class CellAppManager;
	public:
		AppMgr_CellApp(uint32 nAppID,NetworkConn & conn,const SocketAddress & bindAddr)
			:AppMgr_App(nAppID,conn)
			,m_internBindAddr(bindAddr)
		{
	
		}

		const SocketAddress m_internBindAddr;
	private:
		uint32 m_nEntities;
		uint32 m_nPlayers;
		uint32 m_nScenes;

	private:
		//~IAppPacketHandler;
		virtual void OnRecvPacket(const PacketBase & pkt);
		virtual void OnLoseConn();
		//IAppPacketHandler;
	};

	class CellAppManager : public AppMgr_App_Manager ,public boost::serialization::singleton<CellAppManager>
	{
	public:
		CellAppManager();
		~CellAppManager();

		void HandlePacket(const AppManagerInterface::PktCreateEntityOnCell & pkt);

		void OnNewBaseApp(AppMgr_BaseApp & baseApp);
		void OnTick(float fDelta);

		void BroadcastToAllApps(const PacketBase & pkt);
	private:
		AppMgr_CellApp * GetMinLoadApp();
	private:
		//override AppMgr_App_Manager;
		virtual bool OnWhoAmIPkt(NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt);
		//~override AppMgr_App_Manager;
	};
}

#endif
