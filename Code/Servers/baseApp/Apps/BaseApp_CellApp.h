#ifndef __BASE_APP__CELLAPP_H_
#define __BASE_APP__CELLAPP_H_

#include <NetworkConn.h>
#include "BaseExternInterface.h"
#include "BaseInternInterface.h"

namespace Aurora
{
	class BaseApp_CellAppManager;

	class BaseApp;

	class BaseApp_CellApp :  public OneNetworkConnOwner<AppID>
	{
		friend class BaseApp_CellAppManager;
	private:
		BaseApp_CellApp(AppID id,NetworkConn & conn)
			:OneNetworkConnOwner<AppID> (id,conn)
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

		void OnRecvPacket(const PacketBase & pkt);
		void OnLoseConn();

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const BaseInternInterface::PktRedirectToClient & pkt);
		void HandlePacket(const BaseInternInterface::PktRedirectBroadcastToClient & pkt);
		void HandlePacket(const BaseInternInterface::PktMethodCallFromCell & pkt);
		void HandlePacket(const BaseInternInterface::PktOneAttributeChangeByCell & pkt);
		void HandlePacket(const BaseInternInterface::PktSyncPosRot & pkt);
	};

	class BaseApp_CellAppManager : public IPacketHandler ,public MultiNetworkConnOwnerManager<AppID>, public boost::serialization::singleton<BaseApp_CellAppManager>
	{
	public:
		BaseApp_CellAppManager();
		~BaseApp_CellAppManager();

		void OnTick(float fTimeDelta);

		void HandlePktNewCellApp(const BaseInternInterface::PktNewCellApp & pkt);
		BaseApp_CellApp * GetCellApp(AppID aid);
		BaseApp_CellApp * GetCellApp(NetworkConn & conn);
	private:
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt) ;
		virtual void OnLoseConn(NetworkConn & conn) ;
	public:
		BaseApp & m_theApp;
	};
}

#endif
