#ifndef __LOGIN_APP__APP_MANAGER_H_
#define __LOGIN_APP__APP_MANAGER_H_

#include <NetworkConn.h>
#include "LoginInternInterface.h"
 
namespace Aurora
{
	class LoginApp;
	class LoginApp_AppManager :  public boost::serialization::singleton<LoginApp_AppManager> ,public IPacketHandler,public NetworkConnHolder
	{
	public:
		LoginApp_AppManager();

		bool DoConnect(const SocketAddress & addr);
	private:
		LoginApp & m_theApp;
	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const LoginInternInterface::PktWhoAmIResponse & pkt);
		void HandlePacket(const LoginInternInterface::PktLoginResponse & pkt);
	};
}

#endif
