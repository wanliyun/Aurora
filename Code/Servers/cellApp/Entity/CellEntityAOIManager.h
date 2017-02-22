#ifndef CELL_ENTITY_AOI_MANAGER_H
#define CELL_ENTITY_AOI_MANAGER_H


namespace Aurora
{
	class CellEntity;
	class PacketBase;

	//维护CellEntity的AOI;
	class CellEntityAOIManager
	{
	private:
		struct SEntityInAOI
		{
		public:
			SEntityInAOI(CellEntity & ent,uint32 flg,VOLATILE_STAMP stamp)
				:m_pEntity(&ent)
				,m_nFlags(flg)
				,m_volatileStamp(stamp)
			{
				MY_ASSERT(m_pEntity);
			}

			inline bool IsValid()const{return m_pEntity != 0;}

			inline bool HasFlag(uint32 flg)const{return (m_nFlags & flg) != 0;}
			inline void SetFlag(uint32 flg){m_nFlags |= flg;}
			inline void ClearFlag(uint32 flg){m_nFlags &= ~flg;}
			
			inline CellEntity & GetEntity()const{return *m_pEntity;}
			
			bool IsStampChanged()const;
			void RefreshStampChanged();
		private:
			CellEntity * m_pEntity;
			uint32 m_nFlags;
			VOLATILE_STAMP m_volatileStamp;
		};
	public:
		CellEntityAOIManager(CellEntity & theEnt);
		~CellEntityAOIManager();

	public:
		void OnTick(float fDelta);
		void OnEnterAOI(CellEntity &ent);
		void OnLeaveAOI(CellEntity &ent);

		//重新计算AoI;
		void RefreshAoI(CellEntity &ent);

		void RefreshOwnClientVolatileStamp();
	private:
		bool shouldInAoI(CellEntity &ent);
		inline bool isInAOI(EntityID eid)const{return m_entitiesInAOI.find(eid) != m_entitiesInAOI.end();}
		void SyncVolatileInfo();
	private:
		CellEntity & m_entity;
		VOLATILE_STAMP m_ownClientVolatileStamp;
		std::map<EntityID,SEntityInAOI> m_entitiesInAOI;
	};
}

#endif
