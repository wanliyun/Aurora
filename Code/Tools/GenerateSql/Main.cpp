// appManager.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"
#include "EntityDef/EntityType.h"
#include "EntityDef/EntityTypeManager.h"
#include "FileSystem.h"

using namespace Aurora;
int main(int argc,char * argv[])
{
	if(!FileSystem::get_mutable_instance().Init("./Paths.xml"))
	{
		Log_ErrorF("Load Paths.xml");
		return false;
	}

	EntityTypeManager & mgr = EntityTypeManager::get_mutable_instance();
	if(!mgr.Init(std::string(GetClientScriptDir()),false,false))
	{
		Log_ErrorF("Failed to Init EntityTypeManager\n");
		return false;
	}

// 	uint32 nCount = mgr.GetNumEntityDefs();
// 	for(uint32 i =0;i<nCount;++i)
// 	{
// 		const EntityDef * pDef = mgr.GetEntityDef(i);
// 		MY_ASSERT(pDef);
// 		Log_Normal("%s\n",pDef->GetName().c_str());
// 	}
	system("pause");
	return 0;
}

