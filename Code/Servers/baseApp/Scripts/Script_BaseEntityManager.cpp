
#include "pch.h"
#include "Entity/BaseEntity.h"
#include "Entity/BaseEntityManager.h"
#include <EntityDef/EntityType.h>
#include "pyscript/PyScriptAutoRegister.h"
#include "Script_BaseEntityManager.h"

namespace Aurora
{
	bool BaseEntityManagerWarpper::HasKey( EntityID eid ) const
	{
		std::map<EntityID,bpy::object> & entities = BaseEntityManager::get_mutable_instance().m_mapEntities;
		return entities.find(eid) != entities.end() ;
	}

	bpy::object BaseEntityManagerWarpper::Get( EntityID eid ) const
	{
		std::map<EntityID,bpy::object> & entities = BaseEntityManager::get_mutable_instance().m_mapEntities;
		if(!HasKey(eid))
			return bpy::object();
		return entities[eid];
	}

	bpy::object BaseEntityManagerWarpper::GetKeys() const
	{
		std::map<EntityID,bpy::object> & entities = BaseEntityManager::get_mutable_instance().m_mapEntities;
		bpy::list objList;
		for(std::map<EntityID,bpy::object>::const_iterator it = entities.begin();it!=entities.end();++it)
			objList.append(bpy::long_(it->first));
		return objList;
	}

	bpy::object BaseEntityManagerWarpper::GetValues() const
	{
		std::map<EntityID,bpy::object> & entities = BaseEntityManager::get_mutable_instance().m_mapEntities;
		bpy::list objList;
		for(std::map<EntityID,bpy::object>::const_iterator it = entities.begin();it!=entities.end();++it)
			objList.append(it->second);
		return objList;
	}

	bpy::object BaseEntityManagerWarpper::GetItems() const
	{
		std::map<EntityID,bpy::object> & mapEnts = BaseEntityManager::get_mutable_instance().m_mapEntities;
		bpy::list objList;
		for(std::map<EntityID,bpy::object>::const_iterator it = mapEnts.begin();it!=mapEnts.end();++it)
			objList.append( bpy::make_tuple(it->first,it->second) );
		return objList;
	}

	uint32 BaseEntityManagerWarpper::Len() const
	{
		return  BaseEntityManager::get_mutable_instance().m_mapEntities.size();
	}
}




