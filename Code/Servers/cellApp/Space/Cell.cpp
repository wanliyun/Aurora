#include "pch.h"
#include "Cell.h"
#include "Entity/CellEntity.h"

namespace Aurora
{
	Cell::Cell( SFullCellID fid)
		:m_fullID(fid)
	{
		LogFX();
	}

	Cell::~Cell()
	{
		LogFX();
	}

	void Cell::OnNewEntity( EntityID eid,bpy::object objEnt )
	{
		if(m_mapEntities.find(eid) != m_mapEntities.end())
		{
			Log_ErrorF("Duplex entity[%u]\n",eid);
			return;
		}
		m_mapEntities.insert( std::make_pair(eid,objEnt));
		CellEntity & newEnt =bpy::extract<CellEntity &>(objEnt);
		MY_ASSERT(eid == newEnt.GetID());
		onEnterWorld(newEnt);
	}

	void Cell::OnTick( float fDeltaTime )
	{
		//删除死掉的实体;
		{
			std::vector<EntityID> vDeadEntities;
			for(std::map<EntityID,bpy::object>::iterator iter =m_mapEntities.begin();iter != m_mapEntities.end();++iter)
			{
				CellEntity & entity = bpy::extract<CellEntity &>(iter->second);
				if(entity.IsDestroying())
				{
					onLeaveWorld(entity);
					vDeadEntities.push_back(iter->first);
				}
			}

			for(std::vector<EntityID>::const_iterator iter = vDeadEntities.begin();iter!= vDeadEntities.end();++iter)
			{
				m_mapEntities.erase(*iter);	
			}
		}

		//更新实体的AOI;
		{
			std::vector<bpy::object> vEntities;

			for(std::map<EntityID,bpy::object>::iterator iter =m_mapEntities.begin();iter != m_mapEntities.end();++iter)
				vEntities.push_back(iter->second);

			for(std::vector<bpy::object>::iterator it_ent = vEntities.begin();it_ent != vEntities.end();++it_ent)
			{
				CellEntity & entity = bpy::extract<CellEntity &>(*it_ent);
				for(std::map<EntityID,bpy::object>::iterator iter =m_mapEntities.begin();iter != m_mapEntities.end();++iter)
				{
					CellEntity & targetEntity = bpy::extract<CellEntity &>(iter->second);
					entity.GetAOIManager().RefreshAoI(targetEntity);
				}
				
			}
		}

	}

	void Cell::onEnterWorld( CellEntity & ent )
	{
		for(std::map<EntityID,bpy::object>::iterator iter =m_mapEntities.begin();iter != m_mapEntities.end();++iter)
		{
			CellEntity & entity = bpy::extract<CellEntity &>(iter->second);
			if(ent.GetID() == entity.GetID())
				continue;
			if(entity.IsDestroying())
				continue;
			entity.GetAOIManager().OnEnterAOI(ent);
			ent.GetAOIManager().OnEnterAOI(entity);
		}
	}

	void Cell::onLeaveWorld( CellEntity & ent )
	{
		for(std::map<EntityID,bpy::object>::iterator iter =m_mapEntities.begin();iter != m_mapEntities.end();++iter)
		{
			CellEntity & entity = bpy::extract<CellEntity &>(iter->second);
			if(entity.IsDestroying())
				continue;
			if(ent.GetID() == entity.GetID())
				continue;

			entity.GetAOIManager().OnLeaveAOI(ent);
			ent.GetAOIManager().OnLeaveAOI(entity);
		}
	}
}
