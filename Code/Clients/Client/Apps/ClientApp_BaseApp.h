#ifndef _CLIENT_APP__BASE_APP_H_
#define _CLIENT_APP__BASE_APP_H_

#include <NetworkConn.h>
#include "ClientInterface.h"

namespace Aurora
{
	class ClientApp;
	class PacketBase;
	class ClientApp_BaseApp : public IPacketHandler ,public NetworkConnHolder
	{
	public:
		ClientApp_BaseApp();

		bool ConnectBaseApp(const ClientInterface::PktLoginResponse & pkt);
		void SendEcho();
		void SendBroadCast(EntityID eid);

		void SendPacket(const PacketBase & pkt);
		void SendPacketToCell(const PacketBase & pktToCell,EntityID eid);
	private:

		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt) ;
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;
	private:
		ClientApp & m_theApp;
	};
}

#endif
