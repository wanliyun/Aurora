#ifndef CELL_ENTITY_H
#define CELL_ENTITY_H

#include <EntityDef/EntityBase.h>
#include "CellEntityAOIManager.h"

namespace Aurora
{
	class EntityType;
	class EntityDef;
	class PacketBase;
	class CellApp_BaseApp;
	class Cell;

	class CellEntity : public PosRotEntityBase, public DataTypeFixDict_PyObject
	{
		friend class CellEntityManager;
	public:
		CellEntity(XEntityID id,EntityTypePtr pType,AttributeFlagT flag,AppID baseAppID);
		~CellEntity();

		//为了boost::python 而添加，不应该被调用;
		CellEntity & operator = (const CellEntity &);
	public:
		void Init(const EntityType * pEntityType,const SFullCellID & fullID);
		void OnTick(float fDelta);

		const EntityType & GetType()const;

		Cell * GetCell();
		void SendToClient(const PacketBase & pkt);
		void SendToOtherClients(const PacketBase & pkt);
		void SendToBase(const PacketBase & pkt);

		CellApp_BaseApp * GetBaseApp();

		void Destroy();
		bool IsDestroying()const{return m_bIsDestroying;}

		CellEntityAOIManager & GetAOIManager(){return m_AOIManager;}

		void RegisterListener(EntityID id);
		void DeRegisterListener(EntityID id);

		void CallBaseMethod(std::string strName,bpy::dict args);
		void CallCellMethod(std::string strName,bpy::dict args);

		void CallOwnClientMethod(std::string strName,bpy::dict args);
		void CallOtherClientMethod(std::string strName,bpy::dict args);
		void CallAllClientMethod(std::string strName,bpy::dict args);

		VOLATILE_STAMP GetVolatileStamp()const{return m_volatileStamp;}
		void OnVolatileStampChanged(){++m_volatileStamp;}
		void OnVolatileStampChangedByClient();
		bool CompareVolatileStamp(VOLATILE_STAMP stamp){return m_volatileStamp == stamp; }

		inline void PySetPosXYZ(POS_TYPE x,POS_TYPE y,POS_TYPE z){PySetPosX(x);PySetPosY(y);PySetPosZ(z);}
		inline void PySetRotYPR(ROT_TYPE y,ROT_TYPE p,ROT_TYPE r){PySetRotY(y);PySetRotP(p);PySetRotR(r);}
		 
		inline void PySetPosX(POS_TYPE v){ if(v != GetPosX()){OnVolatileStampChanged();SetPosX(v);}	}
		inline void PySetPosY(POS_TYPE v){ if(v != GetPosY()){OnVolatileStampChanged();SetPosY(v);}	}
		inline void PySetPosZ(POS_TYPE v){ if(v != GetPosZ()){OnVolatileStampChanged();SetPosZ(v);}	}
		 
		inline void PySetRotY(ROT_TYPE v){ if(v != GetRotY()){OnVolatileStampChanged();SetRotY(v);}	 }
		inline void PySetRotP(ROT_TYPE v){ if(v != GetRotP()){OnVolatileStampChanged();SetRotP(v);}	 }
		inline void PySetRotR(ROT_TYPE v){ if(v != GetRotR()){OnVolatileStampChanged();SetRotR(v);}	 }
	public:
		//static void setattr(boost::python::object obj, std::string attr, boost::python::object val);
	private:
		//void onPreSetAttribute( boost::python::object obj,const std::string & str,boost::python::object val);
	private:
		const EntityType * m_pEntityType__;
		AppID m_nBaseAppID;
		SFullCellID m_fullCellID;
		bool m_bIsDestroying;

		std::list<EntityID> m_Listners;
		CellEntityAOIManager m_AOIManager;

		//位置和旋转的时间戳;
		VOLATILE_STAMP m_volatileStamp;
	};
}

#endif
