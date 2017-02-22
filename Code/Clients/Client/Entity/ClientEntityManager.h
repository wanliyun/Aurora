#ifndef CLIENT_ENTITY_MANAGER_H_
#define CLIENT_ENTITY_MANAGER_H_

#include <boost/serialization/singleton.hpp>
#include <boost/python.hpp>
#include "ClientInterface.h"

namespace Aurora
{
	class ClientApp;
	class EntityDef;
	class ClientEntity;
	class ClientEntityManager : public boost::serialization::singleton<ClientEntityManager>
	{
		friend class EntityManagerWarpper;
	public:
		ClientEntityManager();
		~ClientEntityManager();

		void OnTick(float fTimeDelta);
		void ReleaseAll();
		virtual void OnRecvBaseAppPacket(const PacketBase & pkt);
		virtual void OnBaseAppLoseConn(){}

		XEntityID GetPlayerID()const{return m_playerID;}
	public:
		bool HasEntity(EntityID eid)const{return m_entities.find(eid) != m_entities.end();}


		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		void HandlePacket(const ClientInterface::PktWhoAmIResponse & pkt );
		void HandlePacket(const ClientInterface::PktEchoBack & pkt );
		void HandlePacket(const ClientInterface::PktBroadCast & pkt );
		void HandlePacket(const ClientInterface::PktBasePlayerCreate & pkt );
		void HandlePacket(const ClientInterface::PktOneAttributeChange & pkt );
		void HandlePacket(const ClientInterface::PktPlayerEnterSpace & pkt );
		void HandlePacket(const ClientInterface::PktPlayerLeaveSpace & pkt );
		void HandlePacket(const ClientInterface::PktEntityEnterAOI & pkt );
		void HandlePacket(const ClientInterface::PktEntityLeaveAOI & pkt );
		void HandlePacket(const ClientInterface::PktMethodCallFromBase & pkt );
		void HandlePacket(const ClientInterface::PktMethodCallFromCell & pkt );
		void HandlePacket(const ClientInterface::PktSyncPosRot & pkt );
	private:
		typedef std::map<XEntityID,bpy::object>	MapEntitiesT;
		MapEntitiesT	m_entities;		//拥有这些指针;
		ClientApp &		m_theApp;

		XEntityID m_playerID;
		SpaceID  m_playerSpaceID;
		SpaceChannelID m_playerSpaceChannelID;
	};
}

#endif
