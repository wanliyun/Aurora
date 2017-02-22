#ifndef __LOGIN_APP__DB_MANAGER_H_
#define __LOGIN_APP__DB_MANAGER_H_

#include <NetworkConn.h>
#include "BaseInternInterface.h"
 
namespace Aurora
{
	class BaseApp;
	class BaseApp_DBManager : public IPacketHandler,public NetworkConnHolder,public boost::serialization::singleton<BaseApp_DBManager>
	{
	public:
		BaseApp_DBManager();
	private:
		BaseApp & m_theApp;
	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;
	};
}

#endif
