// baseApp.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "pch.h"
#include "BaseApp.h"
#include <network/NetworkInit.h>
#include "MainMacro.h"
#include <pyscript/PyScriptAutoRegister.h>

#ifdef _WIN32
extern "C"
{
#include <openssl/ms/applink.c>
};
#endif

PY_MY_MAIN(Aurora::BaseApp,0.1f);


#define INCLUDING_CPP_INTERFACE
#include "../appManager/AppManagerInterface.cpp"
#include "../cellApp/CellInterface.cpp"
#undef INCLUDING_CPP_INTERFACE
