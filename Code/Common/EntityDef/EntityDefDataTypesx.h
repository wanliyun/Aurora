#ifndef ENTITY_DEF_DATA_TYPES_H
#define ENTITY_DEF_DATA_TYPES_H

#include "EntityDefDataTypes.h"
#include <boost/serialization/singleton.hpp>

namespace Aurora
{
	class EntityDefDataTypes : public boost::serialization::singleton<EntityDefDataTypes>
	{
	public:
		enum EType
		{
			EType_bool	,
			EType_int8	,
			EType_int16	,
			EType_int32	,
			EType_int64	,
			EType_uint8	,
			EType_uint16,
			EType_uint32,
			EType_uint64,
			EType_f32	,
			EType_f64	,
			EType_string,
			EType_Count,
		};

		EntityDefDataTypes();
		~EntityDefDataTypes(){}
		bool GetType(const std::string & strType,EType & eType)const;
	private:
		 std::map<std::string,EType> m_mapTypes;
	};
}

#endif
