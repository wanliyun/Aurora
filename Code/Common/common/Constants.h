#ifndef _APP_CONSTANTS_H_
#define _APP_CONSTANTS_H_

#include <string.h>

namespace Aurora
{
	inline static const char * CommonPath() { return "Common"; }
	inline static const char * GetEntityDefDir() { return "Scripts/EntityDef/"; }

	inline static const char * GetCommonScriptDir() { return "Scripts/Common/"; }
	inline static const char * GetClientScriptDir() { return "Scripts/Client/"; }
	inline static const char * GetBaseScriptDir() { return "Scripts/Base/"; }
	inline static const char * GetCellScriptDir() { return "Scripts/Cell/"; }

	inline static const char * GetSpaceDir() { return "Spaces/"; }

	inline static const char * GetDataTypesFileName() { return "_DataTypes.xml"; }
	inline static const char * GetEntitiesFileName() { return "_Entities.xml"; }

	inline static const char * GetAccountTypeName(){return "Account";}
}

#endif
