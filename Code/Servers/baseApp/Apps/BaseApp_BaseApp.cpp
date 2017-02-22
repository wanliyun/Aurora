
#include "pch.h"
#include "BaseApp_BaseApp.h"
#include "BaseApp.h"
#include "entity/BaseEntity.h"
#include "entity/BaseEntityManager.h"

namespace Aurora
{
	void BaseApp_BaseApp::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_BaseIntern);
			HADNLE_PACKET_(BaseInternInterface::EPacketIndex_BaseApp_WhoAmI	,BaseInternInterface::Pkt_BaseApp_WhoAmI	);
		END_HANDLE_PACKET();
	}

	void BaseApp_BaseApp::HandlePacket( const BaseInternInterface::Pkt_BaseApp_WhoAmI & /*pkt*/ )
	{

	}

	void BaseApp_BaseApp::OnTick( float /*fTimeDelta*/ )
	{
		if(m_state != EState_Running)
			return;
	}

	BaseApp_BaseAppManager::BaseApp_BaseAppManager()
		:m_theApp(BaseApp::get_mutable_instance())
	{

	}

	BaseApp_BaseAppManager::~BaseApp_BaseAppManager()
	{

	}

	void BaseApp_BaseAppManager::OnTick( float /*fTimeDelta*/ )
	{
	}


	void BaseApp_BaseAppManager::HandlePacket( const BaseInternInterface::PktNewBaseApp & pkt )
	{
		if(pkt.m_appID == m_theApp.GetAppID())
		{
			Log_NormalF("BaseInternInterface::PktNewBaseApp Ingore myApp\n");
			return;
		}
		if(GetBaseApp(pkt.m_appID))
		{
			Log_ErrorF("BaseInternInterface::PktNewBaseApp baseapp[%u] already exists\n",pkt.m_appID);
			return;
		}

		OnNewBaseApp(pkt.m_appID,pkt.m_internBindAddr);
	}

	void BaseApp_BaseAppManager::OnWhoAmIPkt( NetworkConn & conn,const PacketBase & pkt )
	{
		MY_ASSERT(pkt.GetHeader().GetInterface() == EPacketInterface_BaseIntern);
		MY_ASSERT(pkt.GetHeader().GetIndex() == BaseInternInterface::EPacketIndex_BaseApp_WhoAmI);
		MY_ASSERT(!conn.IsExtern());

		BaseInternInterface::Pkt_BaseApp_WhoAmI & whoAmIPkt = (BaseInternInterface::Pkt_BaseApp_WhoAmI & )pkt;
		
		if(GetBaseApp(whoAmIPkt.m_appID))
		{
			Log_ErrorF("base app[%u] already exists\n",whoAmIPkt.m_appID);
			return;
		}
		BaseApp_BaseApp * pNewBaseApp = new BaseApp_BaseApp(whoAmIPkt.m_appID,conn);
		if(!pNewBaseApp)
		{
			Log_ErrorF("new BaseApp_BaseApp FAILD id=%u\n",whoAmIPkt.m_appID);
			return;
		}
		
		Log_OKF("Base Connect Me ! id=[%u] addr=[%s]\n",whoAmIPkt.m_appID,conn.GetRemoteAddr().ToString().c_str());

		conn.SetPacketHandler( this );
		addConn(whoAmIPkt.m_appID,conn,*pNewBaseApp);
	}

	void BaseApp_BaseAppManager::OnNewBaseApp( AppID appID,const SocketAddress & addr)
	{
		if( appID == m_theApp.GetAppID() )
			return;

		if(GetBaseApp(appID))
		{
			Log_ErrorF("Try to connect baseapp id=[%u] (already exists)\n",appID);
			return;
		}

		if( appID > m_theApp.GetAppID() ) //主动连接大于自己ID的App(当然小于自己ID的BaseApp会主动连接到自己),保证所有BaseApp互联;
		{
			Log_NormalF("Try to connect baseapp[%u][%s]\n",appID,addr.ToString().c_str());

			NetworkConn * pConn = m_theApp.DoConnect(addr);
			if(pConn == 0)
			{
				Log_ErrorF("connect to baseapp[%s] FAILED\n",addr.ToString().c_str());
				return;
			}
			BaseApp_BaseApp * pNewBaseApp = new BaseApp_BaseApp(appID,*pConn);
			if(0 == pNewBaseApp)
			{
				pConn->Close();
				Log_ErrorF("FAILED TO new BaseApp_BaseApp id=%u \n",appID);
				return;
			}
			Log_OKF("Connect Other BaseApp! id=[%u] addr=[%s]\n",appID,addr.ToString().c_str());
			pConn->SetPacketHandler(this);
			addConn(appID,*pConn,*pNewBaseApp);

			BaseInternInterface::Pkt_BaseApp_WhoAmI newPkt;
			newPkt.m_appID = m_theApp.GetAppID();
			pNewBaseApp->SendPacket(newPkt);
		}
		else
		{
			Log_NormalF("baseapp id=[%u] should connected (or will connect) me\n",appID);
		}
	}



	void BaseApp_BaseAppManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		BaseApp_BaseApp * pBaseApp = GetBaseApp(conn);
		MY_ASSERT(pBaseApp);
		pBaseApp->OnRecvPacket(pkt);
	}

	void BaseApp_BaseAppManager::OnLoseConn( NetworkConn & conn )
	{
		BaseApp_BaseApp * pBaseApp = GetBaseApp(conn);
		MY_ASSERT(pBaseApp);
		Log_OKF("Lose BaseApp! id=[%u] addr=[%s]\n",pBaseApp->GetOwnerID(),conn.GetRemoteAddr().ToString().c_str());
		pBaseApp->OnLoseConn();
		delConn(conn);
	}


}
