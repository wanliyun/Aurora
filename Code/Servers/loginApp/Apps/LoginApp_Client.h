#ifndef __LOGIN_APP__CLIENT_H_
#define __LOGIN_APP__CLIENT_H_

#include <NetworkConn.h>
#include "LoginExternInterface.h"
#include "LoginInternInterface.h"


namespace Aurora
{
	class LoginApp_AppManager;
	class ClientManager;


	class LoginApp_Client : public OneNetworkConnOwner<AppID>
	{
		friend class LoginApp_AppManager;
		friend class ClientManager;
	public:
		const std::string & GetAccountName()const{return m_strAccountName;}
	private:
		LoginApp_Client(AppID aid,NetworkConn & conn)
			: OneNetworkConnOwner<AppID> (aid ,conn )
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

		std::string m_strAccountName;
	private:
		void OnRecvPacket(const PacketBase & pkt);

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const LoginExternInterface::PktLoginRquest & pkt);
	};

	class ClientManager : public boost::serialization::singleton<ClientManager> , public IPacketHandler, public MultiNetworkConnOwnerManager<AppID>
	{
		friend class LoginApp_AppManager;
	public:
		ClientManager();
		~ClientManager(){}

		bool HasClient(AppID aid)const;
		LoginApp_Client * GetClient(AppID aid);

		void OnNewConn( NetworkConn & newConn );

	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;

	private:
		UniqueIDPool< Id2Owner_T,AppID  > m_clntIDPool;
	};
}

#endif
