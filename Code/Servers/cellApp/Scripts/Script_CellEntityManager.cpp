
#include "pch.h"
#include "Entity/CellEntity.h"
#include "Entity/CellEntityManager.h"
#include <EntityDef/EntityType.h>
#include "Script_CellEntityManager.h"

namespace Aurora
{
	bool CellEntityManagerWarpper::HasKey( EntityID eid ) const
	{
		std::map<EntityID,bpy::object> & entities = CellEntityManager::get_mutable_instance().m_mapEntities;
		return entities.find(eid) != entities.end() ;
	}

	bpy::object CellEntityManagerWarpper::Get( EntityID eid ) const
	{
		std::map<EntityID,bpy::object> & entities = CellEntityManager::get_mutable_instance().m_mapEntities;
		if(!HasKey(eid))
			return bpy::object();
		return entities[eid];
	}

	bpy::object CellEntityManagerWarpper::GetKeys() const
	{
		std::map<EntityID,bpy::object> & entities = CellEntityManager::get_mutable_instance().m_mapEntities;
		bpy::list objList;
		for(std::map<EntityID,bpy::object>::const_iterator it = entities.begin();it!=entities.end();++it)
			objList.append(bpy::long_(it->first));
		return objList;
	}

	bpy::object CellEntityManagerWarpper::GetValues() const
	{
		std::map<EntityID,bpy::object> & entities = CellEntityManager::get_mutable_instance().m_mapEntities;
		bpy::list objList;
		for(std::map<EntityID,bpy::object>::const_iterator it = entities.begin();it!=entities.end();++it)
			objList.append(it->second);
		return objList;
	}

	bpy::object CellEntityManagerWarpper::GetItems() const
	{
		std::map<EntityID,bpy::object> & mapEnts = CellEntityManager::get_mutable_instance().m_mapEntities;
		bpy::list objList;
		for(std::map<EntityID,bpy::object>::const_iterator it = mapEnts.begin();it!=mapEnts.end();++it)
			objList.append( bpy::make_tuple(it->first,it->second) );
		return objList;
	}

	uint32 CellEntityManagerWarpper::Len() const
	{
		return  CellEntityManager::get_mutable_instance().m_mapEntities.size();
	}


}




