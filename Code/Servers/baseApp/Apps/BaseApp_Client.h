#ifndef __BASE_APP__CLIENT_H_
#define __BASE_APP__CLIENT_H_

#include <NetworkConn.h>
#include "BaseExternInterface.h"
#include "BaseInternInterface.h"
#include <set>

namespace Aurora
{
	class BaseApp_AppManager;
	class BaseApp_ClientManager;
	class BaseEntityManager;

	typedef std::set<EntityID> EntityIDSet_T;
	class BaseApp_Client : public OneNetworkConnOwner<AppID>
	{
		friend class LoginApp_AppManager;
		friend class BaseApp_ClientManager;
	public:
		BaseApp_Client(AppID eid,NetworkConn & conn)
			:OneNetworkConnOwner<AppID> (eid,conn)
			,m_state(EState_Init)
			,m_accountEntityID(BAD_ENTITY_ID)
		{

		}

		void SetClientInfo(EntityID accountEntityID,const std::string & strAccountName)
		{
			m_accountEntityID = accountEntityID;
			m_strAccountName = strAccountName;
		}
		
		EntityID GetAccountEntityID()const{return m_accountEntityID;}
		const std::string & GetAccountName()const{return m_strAccountName;}
		//////////////////////////////////////////////////////////////////////////
		//客户端控制的Entity;
		void AddControlEntity(EntityID eid);
		void DelControlEntity(EntityID eid);
		bool IsControlEntity(EntityID eid){return m_controledEntities.find(eid) != m_controledEntities.end();}

		EntityIDSet_T & GetEntities(){return m_controledEntities;}
	private:
		enum EState
		{
			EState_Init,
			EState_WaitingAppMgr,
			EState_Finish,
			EState_Error,
		};
		EState m_state;
		
		EntityIDSet_T m_controledEntities;
		EntityID m_accountEntityID;
		std::string m_strAccountName;
	private:
		void OnRecvPacket(const PacketBase & pkt);
		void OnLoseConn(){}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const BaseExternInterface::PktRedirectToCell & pkt);
		void HandlePacket(const BaseExternInterface::PktMethodCallFromClient & pkt);
		void HandlePacket(const BaseExternInterface::PktEcho & pkt);
		void HandlePacket(const BaseExternInterface::PktBroadCast & pkt);
		void HandlePacket(const BaseExternInterface::PktSyncPosRot & pkt);
	};

	class BaseApp_ClientManager : public IPacketHandler, public MultiNetworkConnOwnerManager<AppID>, public boost::serialization::singleton<BaseApp_ClientManager>
	{
	public:
		BaseApp_ClientManager();
		~BaseApp_ClientManager(){}

		void OnTick(float fTimeDelta);

		//来自AppManger的数据包;
		void HandlePktCreateAccountRequest(const BaseInternInterface::PktCreateAccountRquest & pkt,class NetworkConn & conn);

		//来自客户端的数据包;
		void OnWhoAmIPkt(NetworkConn & conn, const BaseExternInterface::PktWhoAmI & pkt);
	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt);
		virtual void OnLoseConn(NetworkConn & conn);
		//~override IPacketHandler;

	private:
		BaseEntityManager & m_entityMgr;
		UniqueIDPool<Id2Owner_T,AppID> m_clntIDPool;

		struct SIncomingAccount
		{
			explicit SIncomingAccount(const std::string & strBlowFish)
				:m_strBlowfishKey(strBlowFish)
				,m_fLifeTime(10.0f)//最多等待客户端10秒;
			{

			}
			std::string m_strBlowfishKey;
			float m_fLifeTime;
		};

		typedef std::map<std::string,SIncomingAccount*> MapIncomingAccountT;
		MapIncomingAccountT m_mapIncomingAccounts;
	};
}

#endif
