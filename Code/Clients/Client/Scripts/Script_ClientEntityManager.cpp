
#include "pch.h"
#include <boost/python.hpp>
#include "Entity/ClientEntity.h"

#include "Entity/ClientEntityManager.h"
#include <EntityDef/EntityType.h>

#include "pyscript/PyScriptAutoRegister.h"
#include <boost/noncopyable.hpp>
#include "Script_ClientEntityManager.h"

namespace Aurora
{
	bool EntityManagerWarpper::HasKey( EntityID eid ) const
	{
		return ClientEntityManager::get_mutable_instance().HasEntity(eid);
	}

	bpy::object EntityManagerWarpper::Get( EntityID eid ) const
	{
		if(!HasKey(eid))
			return bpy::object();
		return ClientEntityManager::get_mutable_instance().m_entities[eid];
	}

	bpy::object EntityManagerWarpper::GetKeys() const
	{
		ClientEntityManager::MapEntitiesT & mapEnts  =  ClientEntityManager::get_mutable_instance().m_entities;
		bpy::list objList;
		for(ClientEntityManager::MapEntitiesT::const_iterator it = mapEnts.begin();it!=mapEnts.end();++it)
			objList.append(bpy::long_(it->first));
		return objList;
	}

	bpy::object EntityManagerWarpper::GetValues() const
	{
		ClientEntityManager::MapEntitiesT & mapEnts  =  ClientEntityManager::get_mutable_instance().m_entities;
		bpy::list objList;
		for(ClientEntityManager::MapEntitiesT::const_iterator it = mapEnts.begin();it!=mapEnts.end();++it)
			objList.append(it->second);
		return objList;
	}

	bpy::object EntityManagerWarpper::GetItems() const
	{
		ClientEntityManager::MapEntitiesT & mapEnts  =  ClientEntityManager::get_mutable_instance().m_entities;
		bpy::list objList;
		for(ClientEntityManager::MapEntitiesT::const_iterator it = mapEnts.begin();it!=mapEnts.end();++it)
			objList.append( bpy::make_tuple(it->first,it->second) );
		return objList;
	}

	uint32 EntityManagerWarpper::Len() const
	{
		return ClientEntityManager::get_mutable_instance().m_entities.size();
	}

	bpy::object EntityManagerWarpper::GetPlayer() const
	{
		ClientEntityManager & mgr = ClientEntityManager::get_mutable_instance();
		EntityID eid = mgr.GetPlayerID();
		return Get(eid);
	}

}




