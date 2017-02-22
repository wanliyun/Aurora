
#include "pch.h"
#include "BaseAppConfig.h"


using namespace std;

namespace Aurora
{
	BaseAppConfig::BaseAppConfig():ServerAppConfigBase(std::string("BaseApp"))
	{

	}

	BaseAppConfig::~BaseAppConfig()
	{

	}


	bool BaseAppConfig::OnInit( const XMLElementWrapper & xmlEle )
	{
		if(!ServerAppConfigBase::OnInit(xmlEle))
			return false;

		PREPARE_CONFIG();

		BEGIN_CONFIG(DBConnStr);
		CONFIG_ATTRIBUTE(str,m_dbConnStr);
		END_CONFIG();

		BEGIN_CONFIG(AppMgr);
		CONFIG_ATTRIBUTE(addr,m_appMgrAddr);
		CONFIG_ATTRIBUTE(port,m_appMgrPort);
		END_CONFIG();

		CONFIG_RETURN();
	}

}
