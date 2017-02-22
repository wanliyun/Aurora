// appManager.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"
#include <network/NetworkInit.h>
#include "AppManager.h"
#include "MainMacro.h"


MY_MAIN(Aurora::AppManager,0.3f);


#define INCLUDING_CPP_INTERFACE
#include "../cellApp/CellInterface.cpp"
#include "../baseApp/BaseInternInterface.cpp"
#include "../loginApp/LoginInternInterface.cpp"
#include "../dbManager/DBManagerInterface.cpp"
#undef INCLUDING_CPP_INTERFACE
