#ifndef _BASE_ENTITY_MANAGER_H_
#define _BASE_ENTITY_MANAGER_H_

#include <map>
#include <boost/serialization/singleton.hpp>
#include <EntityDef/EntityBase.h>

namespace Aurora
{
	namespace BaseInternInterface
	{
		class PktCreateAccountRquest;
		class PktBroadcastMsg;
		class PktRedirectToClient;
		class PktRedirectBroadcastToClient;
		class PktMethodCallFromCell;
		class PktOneAttributeChangeByCell;
		class PktSyncPosRot;
	}
	namespace BaseExternInterface
	{
		class PktWhoAmI;
		class PktRedirectToCell;
		class PktMethodCallFromClient;
		class PktSyncPosRot;
	}

	class BaseEntity;
	class NetworkConn;
	class BaseApp_Client;
	class BaseApp_CellApp;
	class BaseEntityManager : public boost::serialization::singleton<BaseEntityManager>
	{
		friend class BaseEntityManagerWarpper;
	public:
		BaseEntityManager();
		~BaseEntityManager();

		void OnTick(float fTimeDelta);

		size_t GetNumEntities()const{return m_mapEntities.size();}

		bool HasEntity(EntityID eid)const{return m_mapEntities.find(eid) != m_mapEntities.end();}
		BaseEntity & GetEntity(EntityID eid);

		bpy::object CreateAccountEntity();
		//////////////////////////////////////////////////////////////////////////
		//来自AppManager的消息;
		bool HandlePacket(const BaseInternInterface::PktBroadcastMsg & pkt);

		//////////////////////////////////////////////////////////////////////////
		// 来自CellApp的消息;
		void HandleCellAppPacket(BaseApp_CellApp & cellApp,const BaseInternInterface::PktRedirectToClient & pkt);
		void HandleCellAppPacket(BaseApp_CellApp & cellApp,const BaseInternInterface::PktRedirectBroadcastToClient & pkt);
		void HandleCellAppPacket(BaseApp_CellApp & cellApp,const BaseInternInterface::PktMethodCallFromCell & pkt);
		void HandleCellAppPacket(BaseApp_CellApp & cellApp,const BaseInternInterface::PktOneAttributeChangeByCell & pkt);
		void HandleCellAppPacket(BaseApp_CellApp & cellApp,const BaseInternInterface::PktSyncPosRot & pkt);

		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//来自客户端的消息;
		bool OnNewClientWithMsg(BaseApp_Client & clnt, const BaseExternInterface::PktWhoAmI & pkt);
		void OnLoseClient(BaseApp_Client & clnt);

		void HandleClientPacket(BaseApp_Client & clnt, const BaseExternInterface::PktRedirectToCell & pkt);
		void HandleClientPacket(BaseApp_Client & clnt, const BaseExternInterface::PktMethodCallFromClient & pkt);
		void HandleClientPacket(BaseApp_Client & clnt, const BaseExternInterface::PktSyncPosRot & pkt);
		//////////////////////////////////////////////////////////////////////////
	private:
		typedef std::map<EntityID,bpy::object> MapEntitiesT;
		MapEntitiesT m_mapEntities;
		UniqueIDPool< MapEntitiesT ,EntityID> m_entityIDPool;
	};
}

#endif
