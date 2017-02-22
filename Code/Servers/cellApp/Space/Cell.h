#ifndef _CELL_APP_CELL_H
#define _CELL_APP_CELL_H

#include <ServerSpace.h>

namespace Aurora
{
	class CellEntity;
	class Cell
	{
	public:
		Cell(SFullCellID);
		~Cell();

		void OnTick(float fDeltaTime);
		void OnNewEntity(EntityID eid,bpy::object objEnt);

	private:
		void onEnterWorld(CellEntity & ent);
		void onLeaveWorld(CellEntity & ent);
	private:
		SFullCellID m_fullID;
		std::map<EntityID,bpy::object> m_mapEntities;
	};
}

#endif
