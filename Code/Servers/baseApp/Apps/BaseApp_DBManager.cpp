
#include "pch.h"
#include "BaseApp_AppManager.h"
#include "BaseApp_DBManager.h"
#include "BaseApp.h"
#include "BaseApp_Client.h"
#include "entity/BaseEntity.h"
#include "entity/BaseEntityManager.h"
#include "../appManager/AppManagerInterface.h"
#include "BaseApp_CellApp.h"
#include "BaseApp_BaseApp.h"
#include <memory_stream.h>

namespace Aurora
{

	BaseApp_DBManager::BaseApp_DBManager()
		:m_theApp(BaseApp::get_mutable_instance())
	{

	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void BaseApp_DBManager::OnRecvPacket( NetworkConn & conn, const PacketBase & pkt )
	{

	}

	void BaseApp_DBManager::OnLoseConn( NetworkConn & conn )
	{

	}
}
