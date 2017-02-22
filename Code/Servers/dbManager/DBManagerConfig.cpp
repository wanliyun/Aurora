
#include "pch.h"
#include "DBManagerConfig.h"


using namespace std;

namespace Aurora
{
	DBManagerConfig::DBManagerConfig():ServerAppConfigBase(std::string("DBManager"))
	{

	}

	DBManagerConfig::~DBManagerConfig()
	{

	}


	bool DBManagerConfig::OnInit( const XMLElementWrapper & xmlEle )
	{
		if(!ServerAppConfigBase::OnInit(xmlEle))
			return false;

		PREPARE_CONFIG();

		BEGIN_CONFIG(AppMgr);
			CONFIG_ATTRIBUTE(addr,m_appMgrAddr);
			CONFIG_ATTRIBUTE(port,m_appMgrPort);
		END_CONFIG();

		CONFIG_RETURN();
	}

}
