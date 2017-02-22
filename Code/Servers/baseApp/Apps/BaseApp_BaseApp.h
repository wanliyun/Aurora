//////////////////////////////////////////////////////////////////////////
//   BaseApp之间互相连接;
//   连接规则：ID小的BaseApp主动连接到大于自己ID的BaseApp上;
//
//
//////////////////////////////////////////////////////////////////////////
#ifndef __BASE_APP__BASE_APP_H_
#define __BASE_APP__BASE_APP_H_

#include <NetworkConn.h>
#include "BaseExternInterface.h"
#include "BaseInternInterface.h"

namespace Aurora
{
	class BaseApp_BaseAppManager;
	class BaseApp;

	class BaseApp_BaseApp : public OneNetworkConnOwner<AppID>
	{
		friend class BaseApp_BaseAppManager;
	private:
		explicit BaseApp_BaseApp(AppID aid,NetworkConn & conn)
			:OneNetworkConnOwner<AppID>(aid,conn)
			,m_state(EState_Running)
		{

		}
		void OnTick(float fTimeDelta);

		enum EState
		{
			EState_Running,
			EState_Error,
		};
		EState m_state;

		virtual void OnRecvPacket(const PacketBase & pkt);
		virtual void OnLoseConn(){m_state = EState_Error;}

		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const BaseInternInterface::Pkt_BaseApp_WhoAmI & pkt);
	};

	class BaseApp_BaseAppManager : public IPacketHandler, public MultiNetworkConnOwnerManager<AppID>, public boost::serialization::singleton<BaseApp_BaseAppManager>
	{
	public:
		BaseApp_BaseAppManager();
		~BaseApp_BaseAppManager();

		void OnTick(float fTimeDelta);

		void OnWhoAmIPkt(NetworkConn & conn,const PacketBase & pkt);

		void OnNewBaseApp(AppID appID,const SocketAddress & addr);

		void HandlePacket(const BaseInternInterface::PktNewBaseApp & pkt);

		BaseApp_BaseApp * GetBaseApp(AppID aid){return (BaseApp_BaseApp * )getOwner(aid);}
		BaseApp_BaseApp * GetBaseApp(NetworkConn & conn){return (BaseApp_BaseApp * )getOwner(conn);}

	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;
	public:
		BaseApp & m_theApp;
		std::map<AppID,BaseApp_BaseApp * >				m_mapID2App;	//拥有这些指针;
		std::map<NetworkConn * ,BaseApp_BaseApp * >		m_mapConn2App;	//拥有这些指针;
	};
}

#endif
