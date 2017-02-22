#ifndef _DATA_TYPE_SEQUENCE_META_H
#define _DATA_TYPE_SEQUENCE_META_H


#include "MetaDataTypeBase.h"

namespace Aurora
{
	class MetaDataTypeSequence : public MetaDataTypeBase
	{
	public:
		DataTypeFixedDictMeta():MetaDataTypeBase("FIXED_DICT"){}

		//override MetaDataTypeBase;
		virtual DataTypeBase * BuildType(tinyxml2::XMLHandle & xml);
		//~override MetaDataTypeBase;
	};
}

#endif
