#ifndef __CELL_APP__APP_MANAGER_H_
#define __CELL_APP__APP_MANAGER_H_

#include <NetworkConn.h>
#include "CellInterface.h"
 
namespace Aurora
{
	class CellApp;
	class CellApp_AppManager : public NetworkConnHolder , public IPacketHandler , public boost::serialization::singleton<CellApp_AppManager>
	{
	public:
		CellApp_AppManager( );


		bool DoConnect(const SocketAddress & addr);
	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const CellInterface::PktWhoAmIResponse & pkt);
		void HandlePacket(const CellInterface::PktSyncAllSpaceInfo & pkt);

	private:
		CellApp & m_theApp;
	};
}

#endif
