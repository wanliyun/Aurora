#ifndef __CELL_APP__APP_MANAGER_H_
#define __CELL_APP__APP_MANAGER_H_

#include <NetworkConn.h>
#include "DBManagerInterface.h"
 
namespace Aurora
{
	class DBManager;
	class DBManager_AppManager : public IPacketHandler,public NetworkConnHolder,public boost::serialization::singleton<DBManager_AppManager>
	{
	public:
		DBManager_AppManager();
		
		bool ConnectAppMgr(const SocketAddress & addr);
	private:
		DBManager & m_theApp;
	private:

		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt) ;
		virtual void OnLoseConn(NetworkConn & conn) ;
		//~override IPacketHandler;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const DBManagerInterface::PktWhoAmIResponse & pkt);
		void HandlePacket(const DBManagerInterface::PktTest & pkt);
	};
}

#endif
