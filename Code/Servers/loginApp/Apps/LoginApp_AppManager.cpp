
#include "pch.h"
#include "LoginApp_AppManager.h"
#include "LoginApp.h"
#include "LoginApp_Client.h"
#include "RSACipher.h"
#include "../../Clients/Client/ClientInterface.h"
#include "../appManager/AppManagerInterface.h"

namespace Aurora
{
	LoginApp_AppManager::LoginApp_AppManager( )
		:m_theApp(LoginApp::get_mutable_instance())
	{

	}

	void LoginApp_AppManager::OnRecvPacket(NetworkConn & conn,  const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_LoginIntern);
		HADNLE_PACKET_(LoginInternInterface::EPacketIndex_LoginResponse, LoginInternInterface::PktLoginResponse);
		HADNLE_PACKET_(LoginInternInterface::EPacketIndex_WhoAmIResponse, LoginInternInterface::PktWhoAmIResponse);
		END_HANDLE_PACKET();
	}

	void LoginApp_AppManager::OnLoseConn(NetworkConn & conn )
	{
		m_theApp.OnLoseAppManager();
	}


	void LoginApp_AppManager::HandlePacket( const LoginInternInterface::PktWhoAmIResponse & pkt )
	{
		LogF("PktWhoAmIResponse succ =%d,id=%d\n",pkt.m_bSucc,pkt.m_id);
		m_theApp.m_appId = pkt.m_id;
	}


	void LoginApp_AppManager::HandlePacket( const LoginInternInterface::PktLoginResponse & pkt )
	{
		ClientManager & clntMgr = ClientManager::get_mutable_instance();
		LoginApp_Client * pClnt = clntMgr.GetClient(pkt.m_clntAppID);
		if(pClnt == 0)
		{
			LogF("can not find client id=%u\n",pkt.m_clntAppID);
			return;
		}

		MY_ASSERT(pkt.m_clntAppID == pClnt->GetOwnerID());

		ClientInterface::PktLoginResponse newPkt;
		newPkt.m_baseAppIp = pkt.m_baseAppIp;
		newPkt.m_baseAppPort = pkt.m_baseAppPort;
		newPkt.m_code = pkt.m_code;
		newPkt.m_strAccountName = pClnt->GetAccountName();
		LogF("RETURN FROM APPMANAGER AccountName=[%s]\n",newPkt.m_strAccountName.c_str());
		pClnt->GetConn().SendPacket(newPkt);
		pClnt->GetConn().SetCloseWhileAllSent();
	}


	bool LoginApp_AppManager::DoConnect( const SocketAddress & addr)
	{
		if(!NetworkConnHolder::DoConnect(m_theApp,addr))
		{
			Log_ErrorF("Connect AppManager[%s] Failed!\n",addr.ToString().c_str());
			return false;
		}

		Log_OKF("Connect AppManager[%s] Succeed! my addr=[%s]\n",addr.ToString().c_str(),GetConn()->GetRemoteAddr().ToString().c_str());
		GetConn()->SetPacketHandler( this );

		AppManagerInterface::PktWhoAmI pkt;
		pkt.m_AppType = AppManagerInterface::PktWhoAmI::EAppType_LoginApp;
		pkt.m_internBindAddr = m_theApp.GetInternBindAddr();
		pkt.m_externBindAddr = m_theApp.GetExternBindAddr();
		GetConn()->SendPacket(pkt);
		LogF("SENDING WHOAMI\n");
		return true;
	}

}
