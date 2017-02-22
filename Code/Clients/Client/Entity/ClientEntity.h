#ifndef CLIENT_ENTITY_H_
#define CLIENT_ENTITY_H_

#include "EntityDef/EntityBase.h"

namespace Aurora
{
	class EntityType;
	class EntityDef;
	class PacketBase;
	class ClientEntity : public PosRotEntityBase, public AttributeOwnerBase
	{
	public:
		ClientEntity(XEntityID id,EntityTypePtr pType,AttributeFlagT flag);
		~ClientEntity();

		void OnTick( float fTimeDelta );
		void OnBaseCreate(bpy::object & self);

		bool StartControlled(boost::python::object self);
		bool StopControlled(boost::python::object self);

		void SendToBase(PacketBase & pkt);
		void SendToCell(PacketBase & pkt);

		bool IsControlled()const{return m_bIsControlled;}

		void CallBaseMethod(std::string strName,bpy::dict args);
		void CallCellMethod(std::string strName,bpy::dict args);
		
		//同步当前的位置信息到Server;
		void SyncPosRotToServer();


		inline void PySetPosXYZ(POS_TYPE x,POS_TYPE y,POS_TYPE z){PySetPosX(x);PySetPosY(y);PySetPosZ(z);}
		inline void PySetRotYPR(ROT_TYPE y,ROT_TYPE p,ROT_TYPE r){PySetRotY(y);PySetRotP(p);PySetRotR(r);}

		inline void PySetPosX(POS_TYPE v){ if(m_bIsControlled && v != GetPosX()){m_bIsControlledPosRotChanged = true;SetPosX(v);}	}
		inline void PySetPosY(POS_TYPE v){ if(m_bIsControlled && v != GetPosY()){m_bIsControlledPosRotChanged = true;SetPosY(v);}	}
		inline void PySetPosZ(POS_TYPE v){ if(m_bIsControlled && v != GetPosZ()){m_bIsControlledPosRotChanged = true;SetPosZ(v);}	}
		inline void PySetRotY(ROT_TYPE v){ if(m_bIsControlled && v != GetRotY()){m_bIsControlledPosRotChanged = true;SetRotY(v);}	 }
		inline void PySetRotP(ROT_TYPE v){ if(m_bIsControlled && v != GetRotP()){m_bIsControlledPosRotChanged = true;SetRotP(v);}	 }
		inline void PySetRotR(ROT_TYPE v){ if(m_bIsControlled && v != GetRotR()){m_bIsControlledPosRotChanged = true;SetRotR(v);}	 }
	private:
		bool	m_bBaseCreated;
		bool	m_bIsControlled;
		bool	m_bIsControlledPosRotChanged;
	};
	typedef boost::shared_ptr<ClientEntity> ClientEntityPtr;
}

#endif
