
#ifndef _GLOBAL_ENTITY_H_
#define _GLOBAL_ENTITY_H_
#if 0
#include <map>
#include <boost/serialization/singleton.hpp>
#include "EntityDef/EntityBase.h"

namespace Aurora
{
	class GlobalEntityManager;
	class AppMgr_CellApp;

	class GlobalEntity : public EntityBase
	{
		friend class GlobalEntityManager;
	protected:
		GlobalEntity(EntityTypeID tid,EntityID id);
		~GlobalEntity(){}
		virtual bool IsPlayer(){return false;}

		void CreateCellEntity( AppMgr_CellApp & cellApp );
	public:
		AppID m_nBaseAppID;
		AppID m_nCellAppID;
	};

	class PlayerEntity : public GlobalEntity
	{
		friend class GlobalEntityManager;

	public:
		AppID GetLoginAppId()const{return m_loginAppID;}
		void SetLoginAppId(AppID aid){m_loginAppID = aid;}

		AppID GetClntAppIdOfLoginApp()const{return m_nClntAppIdOfLoginApp;}
		void SetClntAppIdOfLoginApp(AppID aid){m_nClntAppIdOfLoginApp = aid;}
		virtual bool IsPlayer(){return true;}
	protected:
		PlayerEntity(EntityTypeID tid,EntityID id);
		~PlayerEntity(){}

		enum EState
		{
			EState_dummy,
			EState_WaitingBaseAppCreate,
			EState_Inited
		};
		EState m_state;

		AppID	m_nClntAppIdOfLoginApp;
		AppID	m_loginAppID;
	};

	class GlobalEntityManager : public boost::serialization::singleton<GlobalEntityManager>
	{
	public:
		GlobalEntityManager();
		~GlobalEntityManager();

		void OnTick(float fTimeDelta);

		GlobalEntity * AddEntity(EntityTypeID tid,bool isPlayer);
		void DelEntity(GlobalEntity * pEnt);
		void DelEntity(EntityID id);

		bool HasEntity(EntityID id)const;
		GlobalEntity * GetEntity(EntityID id);
	private:
		std::map<EntityID,GlobalEntity *> m_mapGlobalEntities;		//拥有这些指针;
		UniqueIDPool< std::map<EntityID,GlobalEntity *> > m_globalEntityIDPool;
	};
}

#endif
#endif