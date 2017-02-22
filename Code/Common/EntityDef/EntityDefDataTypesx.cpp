#include "pch.h"
#include "EntityDefDataTypes.h"

namespace Aurora
{
	EntityDefDataTypes::EntityDefDataTypes()
	{
#define INIT_TYPE_(TYPE) m_mapTypes.insert(std::make_pair(#TYPE,EType_##TYPE))
			INIT_TYPE_(	bool	);
			INIT_TYPE_(	int8	);
			INIT_TYPE_(	int16	);
			INIT_TYPE_(	int32	);
			INIT_TYPE_(	int64	);
			INIT_TYPE_(	uint8	);
			INIT_TYPE_(	uint16	);
			INIT_TYPE_(	uint32	);
			INIT_TYPE_(	uint64	);
			INIT_TYPE_(	f32		);
			INIT_TYPE_(	f64		);
			INIT_TYPE_(	string	);
#undef INIT_TYPE_		
	}

	bool EntityDefDataTypes::GetType( const std::string & strType,EType & eType )const
	{
		std::map<std::string,EType>::const_iterator iter = m_mapTypes.find(strType);
		if(iter == m_mapTypes.end())
			return false;
		eType = iter->second;
		return true;
	}


}
