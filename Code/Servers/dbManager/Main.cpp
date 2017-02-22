// dbManager.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"
#include "DBManager.h"
#include <network/NetworkInit.h>
#include "MainMacro.h"

MY_MAIN(Aurora::DBManager,0.3f);


#define INCLUDING_CPP_INTERFACE
#include "../baseApp/BaseInternInterface.cpp"
#include "../appManager/AppManagerInterface.cpp"
#undef INCLUDING_CPP_INTERFACE
