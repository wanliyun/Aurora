
#ifndef _SCRIPT_CELL_ENTITY_MANAGER_H_
#define _SCRIPT_CELL_ENTITY_MANAGER_H_

namespace Aurora
{
	class CellEntityManagerWarpper
	{
	public:
		bool HasKey(EntityID eid)const;
		bpy::object Get(EntityID eid)const;
		bpy::object GetKeys()const;
		bpy::object GetValues()const;
		bpy::object GetItems()const;
		uint32 Len()const;
	};
}

#endif
