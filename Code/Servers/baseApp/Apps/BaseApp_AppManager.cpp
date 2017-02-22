
#include "pch.h"
#include "BaseApp_AppManager.h"
#include "BaseApp.h"
#include "BaseApp_Client.h"
#include "entity/BaseEntity.h"
#include "entity/BaseEntityManager.h"
#include "../appManager/AppManagerInterface.h"
#include "BaseApp_CellApp.h"
#include "BaseApp_BaseApp.h"
#include <memory_stream.h>

namespace Aurora
{
	BaseApp_AppManager::BaseApp_AppManager(  )
		:m_theApp(BaseApp::get_mutable_instance())
	{

	}
	void BaseApp_AppManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		MY_ASSERT(&conn == GetConn());

		BEGINE_HANDLE_PACKET(EPacketInterface_BaseIntern);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_WhoAmIResponse	,BaseInternInterface::PktWhoAmIResponse	);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_SyncAllSpaceInfo	,BaseInternInterface::PktSyncAllSpaceInfo	);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_CreateAccountRquest, BaseInternInterface::PktCreateAccountRquest);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_NewCellApp, BaseInternInterface::PktNewCellApp);
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_BroadCast, BaseInternInterface::PktBroadcastMsg );
		HADNLE_PACKET_(BaseInternInterface::EPacketIndex_NewBaseApp, BaseInternInterface::PktNewBaseApp );
		END_HANDLE_PACKET();
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void BaseApp_AppManager::HandlePacket( const BaseInternInterface::PktWhoAmIResponse & pkt )
	{
		LogF("PktWhoAmIResponse succ=%d,id=%d\n",pkt.m_bSucc,pkt.m_id);
		m_theApp.m_appId = pkt.m_id;

		AppID appId;
		SocketAddress internBindAddr;
		MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
		while(is.remainingLength() > 0)
		{
			is>>appId>>internBindAddr;
			if(is.error())
			{
				Log_ErrorF("BaseInternInterface::PktWhoAmIResponse  bad packet\n");
				break;
			}
			BaseApp_BaseAppManager::get_mutable_instance().OnNewBaseApp(appId,internBindAddr);
		}
	}


	void BaseApp_AppManager::HandlePacket( const BaseInternInterface::PktCreateAccountRquest & pkt )
	{
		if(!GetConn())
		{
			Log_ErrorF("BaseInternInterface::PktCreatePlayerRquest no conn\n");
			return;
		}
		BaseApp_ClientManager::get_mutable_instance().HandlePktCreateAccountRequest(pkt,*GetConn());
	}

	void BaseApp_AppManager::HandlePacket( const BaseInternInterface::PktNewCellApp & pkt )
	{
		BaseApp_CellAppManager::get_mutable_instance().HandlePktNewCellApp(pkt);
	}

	void BaseApp_AppManager::HandlePacket( const BaseInternInterface::PktBroadcastMsg & pkt )
	{
		LogF("PktBroadcastMsg\n");
		BaseEntityManager::get_mutable_instance().HandlePacket(pkt);
	}

	void BaseApp_AppManager::HandlePacket( const BaseInternInterface::PktSyncAllSpaceInfo & pkt )
	{
		Log_NormalF("PktSyncAllSpaceInfo\n");
		MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
		m_theApp.get_mutable_instance().GetAllSpaceInfo().ReadFromStream(is);
	}

	void BaseApp_AppManager::HandlePacket( const BaseInternInterface::PktNewBaseApp & pkt )
	{
		BaseApp_BaseAppManager::get_mutable_instance().HandlePacket(pkt);
	}

	void BaseApp_AppManager::OnLoseConn( NetworkConn & conn )
	{
		m_theApp.OnLoseAppManager();
	}

	bool BaseApp_AppManager::DoConnect( const SocketAddress & addr )
	{
		if(!NetworkConnHolder::DoConnect(m_theApp,addr))
		{
			Log_ErrorF("Connect AppManager[%s] Failed!\n",addr.ToString().c_str());
			return false;
		}

		Log_OKF("Connect AppManager[%s] Succeed! my addr=[%s]\n",addr.ToString().c_str(),GetConn()->GetRemoteAddr().ToString().c_str());
		GetConn()->SetPacketHandler( this );

		AppManagerInterface::PktWhoAmI pkt;
		pkt.m_AppType = AppManagerInterface::PktWhoAmI::EAppType_BaseApp;
		pkt.m_internBindAddr = m_theApp.GetInternBindAddr();
		pkt.m_externBindAddr = m_theApp.GetExternBindAddr();
		GetConn()->SendPacket(pkt);
		LogF("SENDING WHOAMI\n");
		return true;
	}

}
