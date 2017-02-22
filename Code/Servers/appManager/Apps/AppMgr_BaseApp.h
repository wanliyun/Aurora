#ifndef _APP_MGR_BASE_APP_H_
#define _APP_MGR_BASE_APP_H_

#include "AppMgr_App.h"
#include <boost/serialization/singleton.hpp>

namespace Aurora
{
	class PlayerEntity;
	class BaseAppManager;
	class AppMgr_CellApp;
	class AppMgr_BaseApp : public AppMgr_App
	{
		friend class BaseAppManager;
	public:
		AppMgr_BaseApp(uint32 nAppID,NetworkConn & conn,const SocketAddress & internBindAddr,const SocketAddress & externBindAddr)
			:AppMgr_App(nAppID,conn)
			,m_internBindAddr(internBindAddr)
			,m_externBindAddr(externBindAddr)
			,m_nNumEntities(0)
		{

		}

		const SocketAddress m_internBindAddr;
		const SocketAddress m_externBindAddr;
	private:
		uint32 m_nNumEntities;
	private:
		//~IAppPacketHandler;
		virtual void OnRecvPacket(const PacketBase & pkt);
		virtual void OnLoseConn();
		//IAppPacketHandler;

		//////////////////////////////////////////////////////////////////////////
		//处理来自BaseApp的消息;
		void HandlePacket(const AppManagerInterface::PktCreatePlayerResponse & pkt);
		void HandlePacket(const AppManagerInterface::PktNoticeClientLose & pkt);
		void HandlePacket(const AppManagerInterface::PktBroadcastMsg & pkt);
		void HandlePacket(const AppManagerInterface::PktCreateEntityOnCell & pkt);

	};

	class BaseAppManager : public AppMgr_App_Manager ,public boost::serialization::singleton<BaseAppManager>
	{
	public:
		BaseAppManager();
		~BaseAppManager();

		//bool CreateAccount(PlayerEntity & player);

		void HandlePacket(const AppManagerInterface::PktBroadcastMsg & pkt);
		
		void OnNewCellApp(AppMgr_CellApp & cellApp);

		void BroadcastToAllApps(const PacketBase & pkt);

		AppMgr_BaseApp * GetMinLoadApp();
	private:
		//override AppMgr_App_Manager;
		virtual bool OnWhoAmIPkt(NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt);
		//~override AppMgr_App_Manager;
	};
}

#endif
