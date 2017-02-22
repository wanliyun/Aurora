
#include "pch.h"
#include "ClientApp_LoginApp.h"
#include "ClientAppConfig.h"
#include  "ClientApp.h"
#include "../../Servers/loginApp/LoginExternInterface.h"
#include "Apps/ClientApp_BaseApp.h"
#include "EntityDef/EntityType.h"
#include "EntityDef/EntityTypeManager.h"
#include "RSACipher.h"
#include "FileSystem.h"
#include "BlowfishPacketFilter.h"

namespace Aurora
{

	ClientApp_LoginApp::ClientApp_LoginApp(  )
		:m_theApp(ClientApp::get_mutable_instance())
		,m_pCRSACipher(0)
	{

	}
	ClientApp_LoginApp::~ClientApp_LoginApp()
	{
		if(m_pCRSACipher)
		{
			CRSACipher::Destroy(m_pCRSACipher);
			m_pCRSACipher = 0;
		}
	}

	bool ClientApp_LoginApp::Init()
	{
		MY_ASSERT(!m_pCRSACipher);
		ClientAppConfig & config = m_theApp.GetMyConfig();
		std::string realPubkeyPath = FileSystem::get_mutable_instance().FindFilePath(config.m_strPubKeyPath);
		m_pCRSACipher = CRSACipher::Create(realPubkeyPath,true);
		return m_pCRSACipher != 0;
	}

	bool ClientApp_LoginApp::ConnectLoginServer(const std::string & userName,const std::string & passwd)
	{
		MY_ASSERT(0 == GetConn());
		if(userName.length() >= LoginExternInterface::PktLoginRquest::MAX_USERNAME_LEN || 
			passwd.length() >= LoginExternInterface::PktLoginRquest::MAX_PASSWORD_LEN)
		{
			Log_ErrorF("Bad userName or password TOO LEN\n");
			return false;
		}
		if(userName.length() == 0 || passwd.length() == 0)
		{
			Log_ErrorF("Bad userName or password CAN NOT BE EMPTY\n");
			return false;
		}

		if(!m_pCRSACipher)
		{
			Log_ErrorF("LOGINAPP PUBLIC KEY NOT LOADED\n");
			return false;
		}
		ClientAppConfig & cfg = ClientApp::get_mutable_instance().GetMyConfig();

		boost::shared_ptr<EntityType>  pEntityType=  EntityTypeManager::get_mutable_instance().GetEntityType( m_theApp.GetMyConfig().m_strPlayerType );
		if(!pEntityType)
			return false;

		LogF("start login \n");

		std::set<size_t> failedAddrIdxs;
		while(failedAddrIdxs.size() < cfg.m_loginAppAddrs.size())
		{
			while(true)
			{
				int rd = rand() % int(cfg.m_loginAppAddrs.size());
				if(rd<0)
					continue;
				if(failedAddrIdxs.find(rd) != failedAddrIdxs.end())
					continue;
				const SocketAddress & addr = cfg.m_loginAppAddrs[rd];
				LogF("trying to connect to login[%s]\n",addr.ToString().c_str());

				if(DoConnect(m_theApp,addr))
				{
					MY_ASSERT(GetConn());
					GetConn()->SetPacketHandler(this);

					Log_OKF("connect to loginapp succeed\n");

					m_theApp.m_state = ClientApp::ES_LOGINAPP_CONNECTING;
					CBlowfishPacketFilter::GenerateKey(m_strBlowfishKey,16);
					LoginExternInterface::PktLoginRquest pkt;
					m_pCRSACipher->Encrypt(userName,pkt.m_strUserName);
					m_pCRSACipher->Encrypt(passwd,pkt.m_strPasssWord);
					m_pCRSACipher->Encrypt(m_strBlowfishKey,pkt.m_strBlowfishKey);
					GetConn()->SendPacket(pkt);

					return true;
				}
				else
				{
					Log_Error("connect failed\n");
					failedAddrIdxs.insert(rd);
				}
			}
		
		}

		return false;
	}

	void ClientApp_LoginApp::OnRecvPacket(NetworkConn & conn, const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_Client);

		HADNLE_PACKET_(ClientInterface::EPacketIndex_LoginResponse, ClientInterface::PktLoginResponse);

		END_HANDLE_PACKET();
	}

	void ClientApp_LoginApp::HandlePacket( const ClientInterface::PktLoginResponse & pkt )
	{
		//断开与Login 的连接;
		DoDisconnect();

		bool bSucc = (pkt.m_code == ClientInterface::PktLoginResponse::EC_Sucess);
		Log_NormalF("PktLoginResponse succ = %s\n", bSucc ? "true" : "false");
		if(!bSucc)
			return;

		SocketAddress baseAppAddr(pkt.m_baseAppIp,pkt.m_baseAppPort);
		Log_NormalF("\tBaseAddr=%s\n",baseAppAddr.ToString().c_str());

		if(m_theApp.GetBaseApp().ConnectBaseApp(pkt))
		{
			if(m_pCRSACipher)
			{
				m_theApp.GetBaseApp().GetConn()->SetPacketFilter(new CBlowfishPacketFilter(m_strBlowfishKey));
			}

			Log_NormalF("Connect to Base App succeed\n");
		}
	}

	void ClientApp_LoginApp::OnLoseConn( NetworkConn & conn )
	{

	}

}

