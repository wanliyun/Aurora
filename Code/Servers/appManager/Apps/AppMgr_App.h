#ifndef _APP_MGR_APP_H_
#define _APP_MGR_APP_H_

#include <network/MySocketAPI.h>
#include <NetworkConn.h>
#include "AppManagerInterface.h"

namespace Aurora
{
	class AppMgr_App_Manager;
	class AppMgr_App : public OneNetworkConnOwner<AppID>
	{
		friend class AppMgr_App_Manager;

	public:
		uint32		GetAppID()const{return GetOwnerID();}

	protected:
		AppMgr_App(uint32 nAppID,NetworkConn & conn)
			: OneNetworkConnOwner<AppID> (nAppID,conn)
		{}
		virtual ~AppMgr_App(){}

		virtual void OnRecvPacket(const PacketBase & pkt) = 0;
		virtual void OnLoseConn() = 0;
	};

	class AppMgr_App_Manager : public IPacketHandler ,public MultiNetworkConnOwnerManager<AppID>
	{
	public:
		AppMgr_App_Manager(const std::string & strAppName);
		virtual ~AppMgr_App_Manager();

		AppMgr_App * GetApp(AppID aid){return (AppMgr_App * )getOwner(aid);}
		AppMgr_App * GetApp(NetworkConn &  conn){return (AppMgr_App * )getOwner(conn);}

		bool HasApp(AppID aid){return 0 != GetApp(aid);}
		bool HasApp(NetworkConn & conn){return 0 != GetApp(conn);}

		virtual bool OnWhoAmIPkt(NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt) = 0;

	protected:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;

	protected:
		const std::string m_strAppName;
		UniqueIDPool< Id2Owner_T,AppID > m_appIDPool;

	};
}

#endif
