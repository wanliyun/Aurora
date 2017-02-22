
#ifndef _SCRIPT_CLIENT_ENTITY_MANAGER_H_
#define _SCRIPT_CLIENT_ENTITY_MANAGER_H_

namespace Aurora
{
	class EntityManagerWarpper
	{
	public:
		bool HasKey(EntityID eid)const;
		bpy::object Get(EntityID eid)const;
		bpy::object GetKeys()const;
		bpy::object GetValues()const;
		bpy::object GetItems()const;

		bpy::object GetPlayer()const;
		uint32 Len()const;
	};

}

#endif
