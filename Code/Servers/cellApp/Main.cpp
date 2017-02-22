// cellApp.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"
#include <network/NetworkInit.h>
#include "CellApp.h"
#include "MainMacro.h"
#include <pyscript/PyScriptAutoRegister.h>

PY_MY_MAIN(Aurora::CellApp,0.1f);

#define INCLUDING_CPP_INTERFACE
#include "../baseApp/BaseInternInterface.cpp"
#include "../appManager/AppManagerInterface.cpp"
#include "../../Clients/Client/ClientInterface.cpp"
#undef INCLUDING_CPP_INTERFACE
