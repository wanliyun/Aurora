
#include "pch.h"
#if 0
#include "GlobalEntity.h"
#include "Apps/AppMgr_CellApp.h"
#include "EntityDef/EntityTypeManager.h"

namespace Aurora
{
	GlobalEntity::GlobalEntity(EntityTypeID tid,EntityID id):
		EntityBase(tid,id)
	{

	}

	void GlobalEntity::CreateCellEntity( AppMgr_CellApp & cellApp )
	{
		cellApp.GetConn();
	}


	PlayerEntity::PlayerEntity(EntityTypeID tid,EntityID id):
		GlobalEntity(tid,id)
	{

	}

	GlobalEntityManager::GlobalEntityManager():
		m_globalEntityIDPool(m_mapGlobalEntities,0x80000000,uint32(-1))
	{

	}

	GlobalEntityManager::~GlobalEntityManager()
	{
		for(std::map<EntityID,GlobalEntity *>::iterator iter = m_mapGlobalEntities.begin();iter!=m_mapGlobalEntities.end();++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_mapGlobalEntities.clear();
	}

	void GlobalEntityManager::OnTick( float fTimeDelta )
	{

	}


	GlobalEntity * GlobalEntityManager::AddEntity( EntityTypeID tid,bool isPlayer)
	{
		EntityID id;
		if(!m_globalEntityIDPool.GenerateUniqueID(id))
		{
			LogF("Failed to generate id\n");
			return 0;
		}
		GlobalEntity * pNewEnt = isPlayer ? new PlayerEntity(tid,id) : new GlobalEntity(tid,id);
		if(!pNewEnt)
		{
			LogF(" Failed tid=%u,id=%u",tid,id);
		}

		m_mapGlobalEntities.insert(std::make_pair(id,pNewEnt));
		return pNewEnt;
	}
	void GlobalEntityManager::DelEntity( GlobalEntity * pEnt )
	{
		if(!pEnt)
			return;
		DelEntity(pEnt->GetID());
	}

	void GlobalEntityManager::DelEntity( EntityID id )
	{
		if(!HasEntity(id))
			return;
		SAFE_DELETE(m_mapGlobalEntities[id]);
		m_mapGlobalEntities.erase(id);
	}

	bool GlobalEntityManager::HasEntity( EntityID id ) const
	{
		return m_mapGlobalEntities.find(id) != m_mapGlobalEntities.end();
	}

	GlobalEntity * GlobalEntityManager::GetEntity( EntityID id )
	{
		if(!HasEntity(id))
			return 0;
		return m_mapGlobalEntities[id];
	}

}
#endif