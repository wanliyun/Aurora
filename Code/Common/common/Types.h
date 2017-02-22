#ifndef Aurora_TYPES_H_
#define Aurora_TYPES_H_

#ifdef _WIN32
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef unsigned int uint;

#else
typedef char		int8;
typedef short		int16;
typedef int			int32;
typedef long long	int64;

typedef unsigned char				uint8;
typedef unsigned short				uint16;
typedef unsigned int				uint32;
typedef unsigned long long			uint64;
typedef unsigned int uint;
#endif

typedef float	f32;
typedef double	f64;

#define BIT_(x)	 (1<<(x))
#define BIT64_(x) (uint64(1)<<(x))

typedef uint16 AppID;
const AppID		BAD_APP_ID = 0;

typedef uint16 EntityTypeID;
const EntityTypeID BAD_ENTITY_TYPE_ID = 0;

typedef uint32 EntityID;
const EntityID	BAD_ENTITY_ID = 0;

typedef uint64 XEntityID;
const XEntityID	BAD_XENTITY_ID = 0;

inline EntityID XID2ID(XEntityID id){return EntityID(id);}
inline AppID XID2AppID(XEntityID id){return AppID( id >> 32);}
inline XEntityID XID(AppID aid,EntityID eid){return (XEntityID(aid) <<32) | XEntityID(eid);}

typedef uint16 AttributeID;
const AttributeID BAD_ATTRIBUTE_ID =  AttributeID(-1);

typedef std::vector<AttributeID>  VAttributesT;

typedef uint32 AttributeFlagT;

typedef uint16 MethodID;
const MethodID BAD_METHOD_ID =  MethodID(-1);

typedef uint16 SpaceID;
const SpaceID BAD_SPACE_ID = SpaceID(-1);

typedef uint16 SpaceChannelID;
const SpaceChannelID BAD_SPACE_CHANNEL_ID = SpaceChannelID(-1);


typedef uint16 CellID;
const CellID BAD_CELL_ID = CellID(-1);

struct SFullCellID
{
	explicit SFullCellID(SpaceID sid = BAD_SPACE_ID,SpaceChannelID cnId= BAD_SPACE_CHANNEL_ID,CellID cid = BAD_CELL_ID)
		:m_spaceId(sid),m_channelId(cnId),m_cellId(cid)
	{

	}
	SpaceID m_spaceId;
	SpaceChannelID m_channelId;
	CellID m_cellId;
};

inline bool operator == (const SFullCellID & a,const SFullCellID & b)
{
	return a.m_spaceId == b.m_spaceId && a.m_channelId == b.m_channelId && a.m_cellId == b.m_cellId;
}
inline bool operator != (const SFullCellID & a,const SFullCellID & b)
{
	return ! (a== b);
}

inline bool operator < (const SFullCellID & a,const SFullCellID & b)
{
	return a.m_spaceId < b.m_spaceId || a.m_channelId < b.m_channelId || a.m_cellId < b.m_channelId;
}

typedef f64 POS_TYPE;
typedef f64 ROT_TYPE;

typedef uint16 VOLATILE_STAMP;

#endif
