#ifndef __DBMANAGER_BASE_APP_H_
#define __DBMANAGER_BASE_APP_H_

#include <NetworkConn.h>
#include "DBManagerInterface.h"

namespace Aurora
{
	class DBManager_AppManager;
	class DBManager_BaseAppManager;
	class DBManager;
	class DBManager_BaseApp  : public OneNetworkConnOwner<AppID>
	{
		friend class DBManager_AppManager;
		friend class DBManager_BaseAppManager;
	public:
		DBManager_BaseApp(NetworkConn & conn,AppID baseAppID)
			:OneNetworkConnOwner<AppID> (baseAppID, conn)
			,m_state(EState_Init)
		{

		}

	private:
		enum EState
		{
			EState_Init,
			EState_WaitingAppMgr,
			EState_Finish,
			EState_Error,
		};
		EState m_state;
	private:
		void OnRecvPacket(const PacketBase & pkt);
		void OnLoseConn(){}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const DBManagerInterface::PktTest & pkt);
	};

	class DBManager_BaseAppManager :  public IPacketHandler , public MultiNetworkConnOwnerManager<AppID>, public boost::serialization::singleton<DBManager_BaseAppManager>
	{
	public:
		DBManager_BaseAppManager();
		~DBManager_BaseAppManager();

		DBManager_BaseApp * GetBaseApp(AppID aid);
		DBManager_BaseApp * GetBaseApp(NetworkConn &  conn);


		void HandlePacketWhoAmI( NetworkConn & newConn ,const DBManagerInterface::PktWhoAmI & pkt);
	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;

	private:
		DBManager & m_theApp;
	};
}

#endif
