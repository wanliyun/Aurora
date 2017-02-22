
#include "pch.h"
#include "BaseApp_Client.h"
#include "BaseApp.h"
#include "../../Clients/Client/ClientInterface.h"
#include "../appManager/AppManagerInterface.h"
#include "entity/BaseEntity.h"
#include "entity/BaseEntityManager.h"
#include "BaseApp_AppManager.h"
#include "BlowfishPacketFilter.h"

namespace Aurora
{
	void BaseApp_Client::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_BaseExtern);
		HADNLE_PACKET_(BaseExternInterface::EPacketIndex_RedirectToCell, BaseExternInterface::PktRedirectToCell);
		HADNLE_PACKET_(BaseExternInterface::EPacketIndex_MethodCallFromClient, BaseExternInterface::PktMethodCallFromClient);
		HADNLE_PACKET_(BaseExternInterface::EPacketIndex_Echo, BaseExternInterface::PktEcho);
		HADNLE_PACKET_(BaseExternInterface::EPacketIndex_BroadCast, BaseExternInterface::PktBroadCast);
		HADNLE_PACKET_(BaseExternInterface::EPacketIndex_SyncPosRot, BaseExternInterface::PktSyncPosRot);
		END_HANDLE_PACKET();
	}

	void BaseApp_Client::HandlePacket( const BaseExternInterface::PktRedirectToCell & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleClientPacket(*this,pkt);
	}


	void BaseApp_Client::HandlePacket( const BaseExternInterface::PktMethodCallFromClient & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleClientPacket(*this,pkt);
	}


	void BaseApp_Client::HandlePacket( const BaseExternInterface::PktEcho & pkt )
	{
		Log_NormalF("PktEcho\n");
		ClientInterface::PktEchoBack newPkt;
		memcpy(newPkt.m_buff,pkt.m_buff,sizeof(pkt.m_buff));
		GetConn().SendPacket(newPkt);
	}

	void BaseApp_Client::HandlePacket( const BaseExternInterface::PktBroadCast & pkt )
	{
		Log_NormalF("PktBroadCast\n");
		AppManagerInterface::PktBroadcastMsg newPkt;
		memcpy(newPkt.m_msg,pkt.m_buff,sizeof(pkt.m_buff));
		BaseApp::get_mutable_instance().GetAppMgr().SendPacket(newPkt);
	}

	void BaseApp_Client::HandlePacket( const BaseExternInterface::PktSyncPosRot & pkt )
	{
		BaseEntityManager::get_mutable_instance().HandleClientPacket(*this,pkt);
	}

	void BaseApp_Client::AddControlEntity( EntityID eid )
	{
		m_controledEntities.insert(eid);
	}

	void BaseApp_Client::DelControlEntity( EntityID eid )
	{
		m_controledEntities.erase(eid); 
	}

	BaseApp_ClientManager::BaseApp_ClientManager()
		:m_entityMgr(BaseEntityManager::get_mutable_instance())
		,m_clntIDPool(m_mapId2Owner,1,AppID(-1),BAD_APP_ID)
	{

	}

	void BaseApp_ClientManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{
		Conn2Owner_T::iterator it = m_mapConn2Owner.find(&conn);
		MY_ASSERT(it != m_mapConn2Owner.end());
		BaseApp_Client * pClnt  = (BaseApp_Client *)(it->second);
		MY_ASSERT(0 != pClnt);
		pClnt->OnRecvPacket(pkt);
	}


	void BaseApp_ClientManager::OnWhoAmIPkt( NetworkConn & conn, const BaseExternInterface::PktWhoAmI & pkt )
	{
		MY_ASSERT(conn.IsExtern());

		MapIncomingAccountT::iterator iter = m_mapIncomingAccounts.find(pkt.m_strAccountName);
		if(iter == m_mapIncomingAccounts.end())
		{
			Log_ErrorF("Can not find accountname=[%s]\n",pkt.m_strAccountName.c_str());
			conn.Close();
			return;
		}
		conn.SetPacketFilter( new CBlowfishPacketFilter(iter->second->m_strBlowfishKey));

		AppID aid;
		bool bSucc = false;
		ClientInterface::PktWhoAmIResponse newPkt;
		for (;;)
		{
			if(!m_clntIDPool.GenerateUniqueID(aid))
			{
				Log_ErrorF("generate unique id faile \n");
				break;
			}

			std::auto_ptr<BaseApp_Client> pClnt(new BaseApp_Client(aid,conn));
			bpy::object entObj = m_entityMgr.CreateAccountEntity();
			if(entObj.is_none())
			{
				Log_ErrorF("Failed m_entityMgr.CreateAccountEntity\n");
				break;
			}
			
			//连接管理;
			{
				conn.SetPacketHandler(this);
				addConn(aid,conn,*pClnt.get());
			}


			BaseEntity & ent = bpy::extract<BaseEntity & >(entObj);
			//回复客户端;
			{
				newPkt.m_bSucc =true;
				newPkt.m_accuntEntityID = ent.GetXID();
				conn.SendPacket(newPkt);
			}

			//绑定关系 && 客户端创建Entity;
			{
				pClnt->SetClientInfo(ent.GetID(),iter->second->m_strBlowfishKey);
				ent.OnAttachClient(entObj,*pClnt);
			}
			pClnt.release();
			bSucc = true;

			break;
		}

		SAFE_DELETE(iter->second);
		m_mapIncomingAccounts.erase(iter);

		if(!bSucc)
		{
			ClientInterface::PktWhoAmIResponse newPkt;
			newPkt.m_bSucc = bSucc;
			conn.SendPacket(newPkt);
			conn.Close();
			return;
		}

		Log_OKF("New Client!id=[%u] addr=[%s]\n",aid,conn.GetRemoteAddr().ToString().c_str());
	}

	void BaseApp_ClientManager::OnLoseConn( NetworkConn & conn )
	{
		Conn2Owner_T::iterator it = m_mapConn2Owner.find(&conn);

		MY_ASSERT(it != m_mapConn2Owner.end());
		BaseApp_Client * pClnt  = (BaseApp_Client *)(it->second);
		MY_ASSERT(0 != pClnt);
		Log_OKF("Lose Client!id=[%u] addr=[%s]\n",pClnt->GetOwnerID(),conn.GetRemoteAddr().ToString().c_str());
		
		m_entityMgr.OnLoseClient(*pClnt);
		pClnt->OnLoseConn();
		delConn(conn);
	}

	void BaseApp_ClientManager::HandlePktCreateAccountRequest( const BaseInternInterface::PktCreateAccountRquest & pkt,class NetworkConn & conn )
	{
		AppManagerInterface::PktCreatePlayerResponse newPkt;
		newPkt.m_strAccountName = pkt.m_strAccountName;
		newPkt.m_bSucc = false;
		if(m_mapIncomingAccounts.find(pkt.m_strAccountName) == m_mapIncomingAccounts.end())
		{
			SIncomingAccount * pNewAccount = new SIncomingAccount(pkt.m_strBlowfishKey);
			if(pNewAccount)
			{
				newPkt.m_bSucc  =  true;
				m_mapIncomingAccounts.insert( std::make_pair(pkt.m_strAccountName,pNewAccount));
			}
		}
		else
		{
			Log_ErrorF("Account already exists [%s]\n",pkt.m_strAccountName.c_str());
		}
		conn.SendPacket(newPkt);
	}

	void BaseApp_ClientManager::OnTick( float fTimeDelta )
	{
		static float s_AccTime = 0;
		s_AccTime += fTimeDelta;
		if(s_AccTime < 1.0f)
			return;
		
		std::vector<std::string> vDel;
		for(MapIncomingAccountT::iterator iter = m_mapIncomingAccounts.begin();iter != m_mapIncomingAccounts.end();++iter)
		{
			if(iter->second->m_fLifeTime < s_AccTime)
			{
				vDel.push_back(iter->first);
				SAFE_DELETE(iter->second);
			}
			else
			{
				iter->second->m_fLifeTime -= s_AccTime;
			}
		}
		s_AccTime = 0;

		for(std::vector<std::string>::const_iterator iter = vDel.begin();iter != vDel.end();++iter)
		{
			m_mapIncomingAccounts.erase(*iter);
		}		
	}

}
