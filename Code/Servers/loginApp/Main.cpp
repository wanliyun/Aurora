// loginApp.cpp : 定义控制台应用程序的入口点。
//

#include "pch.h"
#include "LoginApp.h"
#include <network/NetworkInit.h>
#include "MainMacro.h"

#ifdef _WIN32
extern "C"
{
#include <openssl/ms/applink.c>
};
#endif

MY_MAIN(Aurora::LoginApp,0.3f);

#include "../appManager/AppManagerInterface.cpp"
#include "../../Clients/Client/ClientInterface.cpp"


