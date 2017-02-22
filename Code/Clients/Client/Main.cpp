// Client.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"
#include <iostream>
#include "ClientApp.h"
#include <network/NetworkInit.h>
#include <stdlib.h>
#include <MainMacro.h>
#include <pyscript/PyScriptAutoRegister.h>

#ifdef _WIN32
extern "C"
{
#include <openssl/ms/applink.c>
};
#endif

PY_MY_MAIN(Aurora::ClientApp,0.3f);

#define INCLUDING_CPP_INTERFACE
#include "../../Servers/baseApp/BaseExternInterface.cpp"
#include "../../Servers/loginApp/LoginExternInterface.cpp"
#include "../../Servers/cellApp/CellInterface.cpp"
#undef INCLUDING_CPP_INTERFACE

