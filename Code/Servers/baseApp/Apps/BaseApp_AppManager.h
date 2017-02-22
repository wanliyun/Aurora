#ifndef __LOGIN_APP__APP_MANAGER_H_
#define __LOGIN_APP__APP_MANAGER_H_

#include <NetworkConn.h>
#include "BaseInternInterface.h"
 
namespace Aurora
{
	class BaseApp;
	class BaseApp_AppManager : public IPacketHandler,public NetworkConnHolder,public boost::serialization::singleton<BaseApp_AppManager>
	{
	public:
		BaseApp_AppManager();

		bool DoConnect(const SocketAddress & addr);
	private:
		BaseApp & m_theApp;
	private:
		//override  IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt) ;
		virtual void OnLoseConn(NetworkConn & conn) ;
		//~override IPacketHandler;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const BaseInternInterface::PktWhoAmIResponse & pkt);
		void HandlePacket(const BaseInternInterface::PktSyncAllSpaceInfo & pkt);
		void HandlePacket(const BaseInternInterface::PktCreateAccountRquest & pkt);
		void HandlePacket(const BaseInternInterface::PktNewCellApp & pkt);
		void HandlePacket(const BaseInternInterface::PktBroadcastMsg & pkt);
		void HandlePacket(const BaseInternInterface::PktNewBaseApp & pkt);
	};
}

#endif
