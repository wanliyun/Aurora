#ifndef __CELL_APP_BASE_APP_H_
#define __CELL_APP_BASE_APP_H_

#include <NetworkConn.h>
#include "CellInterface.h"

namespace Aurora
{
	class CellApp_BaseAppManager;
	class CellApp_BaseApp : public OneNetworkConnOwner<AppID>
	{
		friend class CellApp_BaseAppManager;
	public:
		CellApp_BaseApp(AppID aID,NetworkConn & conn)
			:OneNetworkConnOwner<AppID> (aID,conn)
		{

		}

		void OnRecvPacket(const PacketBase & pkt);
		void OnLoseConn(){}

		void SendPacketToClient(const PacketBase & pktToClient,EntityID eid);
		void BroadcastPacketToClient(const PacketBase & pktToClient);

		const AppID GetBaseAppID()const{return GetOwnerID();}
	private:
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const CellInterface::PktCreateEntityOnCell & pkt);
		void HandlePacket(const CellInterface::PktDestroyEntityOnCell & pkt);
		void HandlePacket(const CellInterface::PktMethodCallFromClient & pkt);
		void HandlePacket(const CellInterface::PktOneAttributeChange & pkt);
		void HandlePacket(const CellInterface::PktSyncPosRot & pkt);
	};

	class CellApp_BaseAppManager : public boost::serialization::singleton<CellApp_BaseAppManager>, public IPacketHandler , public MultiNetworkConnOwnerManager<AppID>
	{
	public:
		CellApp_BaseAppManager();
		~CellApp_BaseAppManager();

		void OnWhoAmIPkt(NetworkConn & conn,const PacketBase & pkt);

		CellApp_BaseApp *GetBaseApp(AppID aid);
		CellApp_BaseApp *GetBaseApp(NetworkConn & conn);
	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;
	};
}

#endif
