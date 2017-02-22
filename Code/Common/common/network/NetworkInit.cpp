#include "pch.h"
#include "NetworkInit.h"

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
#endif


namespace Aurora
{
	NetworkInit::NetworkInit()
	{
#ifdef WIN32
		WSADATA wsa_data;
		::WSAStartup(0x0201,&wsa_data);
#endif
	}

	NetworkInit::~NetworkInit()
	{
#ifdef WIN32
		::WSACleanup();
#endif
	}

}

