#ifndef _DATA_TYPE_CLASS_META_H
#define _DATA_TYPE_CLASS_META_H

#include "MetaDataTypeBase.h"

namespace Aurora
{
	class DataTypeEntityMeta : public MetaDataTypeBase
	{
	public:
		DataTypeEntityMeta():MetaDataTypeBase("ENTITY__"){}

		//override MetaDataTypeBase;
		virtual DataTypeBase * NewType();
		virtual DataTypeBase * NewType(tinyxml2::XMLHandle & xml);
		//~override MetaDataTypeBase;
	};
}

#endif
