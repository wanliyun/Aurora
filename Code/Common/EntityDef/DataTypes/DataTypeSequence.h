#ifndef _DATA_TYPE_FIXED_DICT_META_H
#define _DATA_TYPE_FIXED_DICT_META_H

#include "DataTypeBase.h"

namespace Aurora
{
	class DataTypeSequence : public DataTypeBase
	{
	public:
		DataTypeSequence():DataTypeBase("FIXED_DICT"){}

		//override MetaDataTypeBase;
		virtual DataTypeBase * BuildType(tinyxml2::XMLHandle & xml);
		//~override MetaDataTypeBase;
	};
}

#endif
