#ifndef _ENTITY_BASE_H_
#define _ENTITY_BASE_H_

#include "Types.h"
#include "interface/INoneCopyable.h"
#include "DataTypes/DataTypeFixedDict.h"

namespace Aurora
{
	const uint32 ENTITY_UPDATE_FLAG_ENTER_AOI			=  BIT_(0);
	const uint32 ENTITY_UPDATE_FLAG_LEAVE_AOI			=  BIT_(1);
	const uint32 ENTITY_UPDATE_FLAG_PROPERTY_CHANGE		=  BIT_(2);

	class EntityType;
	typedef boost::shared_ptr<EntityType> EntityTypePtr;

	class EntityBase 
	{
	public:
		EntityBase(EntityTypePtr pType,EntityID id,AppID ownerBaseAppID);
		virtual ~EntityBase(){}

		EntityTypePtr GetType(){return m_pType;}
		EntityTypeID GetTypeID()const;
		EntityID	 GetID()const{return m_Id;}
		XEntityID	 GetXID()const{return XID(m_ownerBaseAppID,m_Id);}
		AppID		GetOwnerBaseAppID()const{return m_ownerBaseAppID;}
		void		SetOwnerBaseAppID(AppID id){m_ownerBaseAppID  = id;}


		inline bool IsGlobal()const{return IsGlobalEntityID(m_Id);}
		inline bool IsLocal()const{return IsLocalEntityID(m_Id);}

		static inline bool IsBadEntityID(EntityID id){return id == BAD_ENTITY_ID;}
		static inline bool IsGlobalEntityID(EntityID id){return (id & 0x80000000) != 0;}
		static inline bool IsLocalEntityID(EntityID id){return !IsGlobalEntityID(id);}

	protected:
		EntityTypePtr	m_pType;
		AppID			m_ownerBaseAppID;
		EntityID		m_Id;
	};

	//////////////////////////////////////////////////////////////////////////
	//使用右手坐标系;
	//		+Z
	//		 ^	   _ +Y
	//		/|\	   /|
	//		 |    /
	//		 |   /
	//		 |  /
	//		 | /
	//		 |---------------->+X
	//
	//////////////////////////////////////////////////////////////////////////
	class PosEntityBase : public EntityBase
	{
	public:
		PosEntityBase(EntityTypePtr pType,EntityID id,AppID ownerBaseAppID)
			:EntityBase(pType,id,ownerBaseAppID)
			,_pos_x(0)
			,_pos_y(0)
			,_pos_z(0)
		{

		}

		inline POS_TYPE  GetPosX()const{return _pos_x;}
		inline POS_TYPE  GetPosY()const{return _pos_y;}
		inline POS_TYPE  GetPosZ()const{return _pos_z;}
		inline void GetPosXYZ(POS_TYPE & x,POS_TYPE & y,POS_TYPE & z){x = _pos_x;y=_pos_y;z=_pos_z;}

		inline void SetPosX(POS_TYPE v){_pos_x = v ;}
		inline void SetPosY(POS_TYPE v){_pos_y = v ;}
		inline void SetPosZ(POS_TYPE v){_pos_z = v ;}
		inline void SetPosXYZ(POS_TYPE  x,POS_TYPE  y,POS_TYPE  z){_pos_x = x;_pos_y = y;_pos_z=z;}

		inline POS_TYPE GetDistance(PosEntityBase & ent)
		{
			POS_TYPE dx = ent._pos_x = _pos_x;
			POS_TYPE dy = ent._pos_y = _pos_y;
			POS_TYPE dz = ent._pos_z = _pos_z;
			return sqrt( dx * dx + dy * dy + dz * dz);
		}
		inline POS_TYPE GetSqureDistance(PosEntityBase & ent)
		{
			POS_TYPE dx = ent._pos_x = _pos_x;
			POS_TYPE dy = ent._pos_y = _pos_y;
			POS_TYPE dz = ent._pos_z = _pos_z;
			return  dx * dx + dy * dy + dz * dz;
		}

		inline POS_TYPE GetDistance2D(PosEntityBase & ent)
		{
			POS_TYPE dx = ent._pos_x = _pos_x;
			POS_TYPE dy = ent._pos_y = _pos_y;
			return sqrt( dx * dx + dy * dy);
		}
		inline POS_TYPE GetSqureDistance2D(PosEntityBase & ent)
		{
			POS_TYPE dx = ent._pos_x = _pos_x;
			POS_TYPE dy = ent._pos_y = _pos_y;
			return  dx * dx + dy * dy ;
		}
	private:
		POS_TYPE	_pos_x,_pos_y,_pos_z;
	};

	//////////////////////////////////////////////////////////////////////////
	//使用右手坐标系;
	//		+Z
	//		 ^	   _ +Y
	//		/|\	   /|
	//		 |    /
	//		 |   /
	//		 |  /
	//		 | /
	//		 |---------------->+X
	//
	// 默认正方向为+Y轴;
	// Yaw		表示绕+Z轴转多少度;
	// Pitch	表示绕+X轴转多少度;
	// Roll		表示绕+Y轴转多少度;
	//////////////////////////////////////////////////////////////////////////
	class PosRotEntityBase : public PosEntityBase
	{
	public:
		PosRotEntityBase(EntityTypePtr pType,EntityID id,AppID ownerBaseAppID)
			:PosEntityBase(pType,id,ownerBaseAppID)
			,_rot_Y(0)
			,_rot_P(0)
			,_rot_R(0)
		{
		}

		inline ROT_TYPE GetRotY()const{return _rot_Y;}
		inline ROT_TYPE GetRotP()const{return _rot_P;}
		inline ROT_TYPE GetRotR()const{return _rot_R;}
		
		inline void GetRotYPR(ROT_TYPE & y,ROT_TYPE & p,ROT_TYPE & r){y = _rot_Y;p=_rot_P;r=_rot_R;}

		inline void SetRotY(ROT_TYPE v){_rot_Y=v;}
		inline void SetRotP(ROT_TYPE v){_rot_P=v;}
		inline void SetRotR(ROT_TYPE v){_rot_R=v;}

		inline void SetRotYPR(ROT_TYPE  y,ROT_TYPE  p,ROT_TYPE  r){_rot_Y=y;_rot_P=p;_rot_R=r;}
	private:
		ROT_TYPE		_rot_Y,_rot_P,_rot_R;
	};
}

#endif
