
#include "pch.h"
#include "LoginApp_Client.h"
#include "LoginApp.h"
#include "../../Clients/Client/ClientInterface.h"
#include "../appManager/AppManagerInterface.h"
#include "RSACipher.h"

namespace Aurora
{
	void LoginApp_Client::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_LoginExtern);
		HADNLE_PACKET_(LoginExternInterface::EPacketIndex_LoginRquest, LoginExternInterface::PktLoginRquest);
		END_HANDLE_PACKET();
	}

	void LoginApp_Client::HandlePacket( const LoginExternInterface::PktLoginRquest & pkt )
	{
		static LoginApp & theApp = LoginApp::get_mutable_instance();
		if(m_state != EState_Init)
		{
			m_state = EState_Error;
			return;
		}

		if(!LoginApp::get_mutable_instance().IsServering() || 0 == theApp.GetAppManagerConn())
		{
			LogF("RETURN FAILED\n");
			ClientInterface::PktLoginResponse pktRet;
			pktRet.m_code = ClientInterface::PktLoginResponse::EC_Failed;
			GetConn().SendPacket(pktRet);
			GetConn().SetCloseWhileAllSent();
			return;
		}
		LogF("REDIRECT TO APPMANAGER \n");
		AppManagerInterface::PktLoginRquest pktNew;
		
		pktNew.m_clntID = GetOwnerID();
		//std::string deUser,dePass;
		theApp.GetCRSACipher().Decrypt(pkt.m_strUserName,pktNew.m_strUserName);
		theApp.GetCRSACipher().Decrypt(pkt.m_strPasssWord,pktNew.m_strPassword);
		theApp.GetCRSACipher().Decrypt(pkt.m_strBlowfishKey,pktNew.m_strBlowfishKey);
		Log_NormalF("user=[%s] clntID=[%u]\n",pktNew.m_strUserName.c_str(),pktNew.m_clntID );
		theApp.GetAppManagerConn()->SendPacket(pktNew);
		m_state = EState_WaitingAppMgr;
		m_strAccountName = pktNew.m_strUserName;
	}

	ClientManager::ClientManager():m_clntIDPool(m_mapId2Owner,1,AppID(-1),BAD_APP_ID)
	{

	}

	bool ClientManager::HasClient( AppID aid )const
	{
		return m_mapId2Owner.find(aid) != m_mapId2Owner.end();
	}

	LoginApp_Client * ClientManager::GetClient( AppID aid )
	{
		if(!HasClient(aid))
			return 0;
		return (LoginApp_Client * )m_mapId2Owner[aid];
	}

	void ClientManager::OnNewConn( NetworkConn & newConn )
	{
		AppID aid;
		if(!m_clntIDPool.GenerateUniqueID(aid))
		{
			LogF("generate unique id faile \n");
			return;
		}
		LogF("clntId=%u\n",aid);
		LoginApp_Client * pClnt = new LoginApp_Client(aid,newConn);

		newConn.SetPacketHandler(this);

		m_mapId2Owner.insert(std::make_pair(aid,pClnt));
		m_mapConn2Owner.insert(std::make_pair(&newConn,pClnt));
	}

	void ClientManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		MY_ASSERT(m_mapConn2Owner.find(&conn) != m_mapConn2Owner.end());
		LoginApp_Client * pClnt = (LoginApp_Client * )m_mapConn2Owner[&conn];
		MY_ASSERT(0 != pClnt );
		pClnt->OnRecvPacket(pkt);
	}

	void ClientManager::OnLoseConn( NetworkConn & conn )
	{
		MY_ASSERT(m_mapConn2Owner.find(&conn) != m_mapConn2Owner.end());
		

		LoginApp_Client * pClnt = (LoginApp_Client *)m_mapConn2Owner[&conn];
		MY_ASSERT(0 != pClnt);

		MY_ASSERT(HasClient(pClnt->GetOwnerID()));

		m_mapId2Owner.erase(pClnt->GetOwnerID());
		m_mapConn2Owner.erase(&conn);

		SAFE_DELETE(pClnt);
	}

}
