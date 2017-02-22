#ifndef  CELL_ENTITY_MANANGER_H
#define  CELL_ENTITY_MANANGER_H

#include <map>
#include <boost/serialization/singleton.hpp>
#include "CellEntityManager.h"
#include "CellInterface.h"

namespace Aurora
{
	class CellEntity;
	class CellApp_BaseApp;
	class CellEntityManager : public boost::serialization::singleton<CellEntityManager>
	{
		friend class CellEntityManagerWarpper;
	public:
		CellEntityManager();
		~CellEntityManager();

		void OnTick(float fTimeDelta);

		size_t GetNumEntities()const{return m_mapEntities.size();}
		CellEntity * GetEntity(EntityID eid);
		bool HasEntity(EntityID eid)const{return m_mapEntities.find(eid) != m_mapEntities.end();}

		void HandlePacket(CellApp_BaseApp & baseApp,const CellInterface::PktCreateEntityOnCell & pkt);
		void HandlePacket(CellApp_BaseApp & baseApp,const CellInterface::PktDestroyEntityOnCell & pkt);
		void HandlePacket(CellApp_BaseApp & baseApp,const CellInterface::PktMethodCallFromClient & pkt);
		void HandlePacket(CellApp_BaseApp & baseApp,const CellInterface::PktOneAttributeChange & pkt);
		void HandlePacket(CellApp_BaseApp & baseApp,const CellInterface::PktSyncPosRot & pkt);
	private:
		std::map<EntityID,bpy::object>		m_mapEntities;	//拥有这些指针;
	};
}

#endif
