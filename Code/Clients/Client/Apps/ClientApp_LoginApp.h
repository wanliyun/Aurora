#ifndef _CLIENT_APP__LOGIN_APP_H_
#define _CLIENT_APP__LOGIN_APP_H_

#include <NetworkConn.h>
#include "ClientInterface.h"

namespace Aurora
{
	class ClientApp;
	class CRSACipher;
	class ClientApp_LoginApp : public IPacketHandler,public NetworkConnHolder
	{
	public:
		ClientApp_LoginApp();
		~ClientApp_LoginApp();

		bool ConnectLoginServer(const std::string & userName,const std::string & passwd);
		bool Init();
	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt) ;
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;
		
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const ClientInterface::PktLoginResponse & pkt );
	private:
		ClientApp & m_theApp;
		CRSACipher		*	m_pCRSACipher;
		std::string m_strBlowfishKey;
	};
}

#endif
