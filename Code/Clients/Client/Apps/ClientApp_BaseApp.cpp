
#include "pch.h"
#include "ClientApp_BaseApp.h"
#include "../../Servers/baseApp/BaseExternInterface.h"
#include "../../Servers/cellApp/CellInterface.h"
#include "ClientApp_LoginApp.h"
#include "ClientAppConfig.h"
#include  "ClientApp.h"
#include "ClientApp_LoginApp.h"
#include "Entity/ClientEntityManager.h"
#include <memory_stream.h>


namespace Aurora
{

	ClientApp_BaseApp::ClientApp_BaseApp()
		:m_theApp(ClientApp::get_mutable_instance())
	{

	}

	bool ClientApp_BaseApp::ConnectBaseApp(const ClientInterface::PktLoginResponse & pkt)
	{
		SocketAddress baseAppAddr(pkt.m_baseAppIp,pkt.m_baseAppPort);
		Log_NormalF("baseapp = [%s]\n",baseAppAddr.ToString().c_str());
		MY_ASSERT(0 == GetConn());

		if(!DoConnect(m_theApp,baseAppAddr))
		{
			Log_ErrorF("connect to [%s]\n",baseAppAddr.ToString());
			return false;
		}

		LogF("connect to baseapp=[%s] succeed\n",baseAppAddr.ToString().c_str());

		GetConn()->SetPacketHandler(this);
		
		m_theApp.m_state = ClientApp::ES_BASEAPP_CONNECTING;

		BaseExternInterface::PktWhoAmI newPkt;
		newPkt.m_strAccountName = pkt.m_strAccountName;
		GetConn()->SendPacket(newPkt);
		return true;

	}


	void ClientApp_BaseApp::OnRecvPacket(NetworkConn & conn,const PacketBase & pkt )
	{
		ClientEntityManager::get_mutable_instance().OnRecvBaseAppPacket(pkt);
	}

	void ClientApp_BaseApp::OnLoseConn( NetworkConn & conn )
	{

	}
	void ClientApp_BaseApp::SendEcho()
	{
		LogFX();
		BaseExternInterface::PktEcho newPkt;
		snprintf(newPkt.m_buff,sizeof(newPkt.m_buff),"hahahha");
		GetConn()->SendPacket(newPkt);
	}

	void ClientApp_BaseApp::SendBroadCast(EntityID eid)
	{
		BaseExternInterface::PktBroadCast newPkt;
		snprintf(newPkt.m_buff,sizeof(newPkt.m_buff),"[%u]hahahha",eid);
		LogF("%s\n",newPkt.m_buff);
		GetConn()->SendPacket(newPkt);
	}


	void ClientApp_BaseApp::SendPacketToCell( const PacketBase & pktToCell,EntityID eid )
	{
		BaseExternInterface::PktRedirectToCell pktToBase;

		pktToBase.m_nEntityID = eid;

		MemoryOStream os;
		NetworkConn::WritePacketToStream(pktToCell,os);
		pktToBase.AppendToBuff(os.data(),os.size());
		GetConn()->SendPacket(pktToBase);
	}

	void ClientApp_BaseApp::SendPacket( const PacketBase & pkt )
	{
		MY_ASSERT(GetConn());
		GetConn()->SendPacket(pkt);
	}



}

