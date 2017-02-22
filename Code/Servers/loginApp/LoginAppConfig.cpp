
#include "pch.h"
#include "LoginAppConfig.h"


using namespace std;

namespace Aurora
{
	LoginAppConfig::LoginAppConfig():ServerAppConfigBase(std::string("LoginApp"))
	{

	}

	LoginAppConfig::~LoginAppConfig()
	{

	}


	bool LoginAppConfig::OnInit( const XMLElementWrapper & xmlEle )
	{
		if(!ServerAppConfigBase::OnInit(xmlEle))
			return false;

		PREPARE_CONFIG();

		BEGIN_CONFIG(PrivateKey);
			CONFIG_ATTRIBUTE(path,m_strPriKeyPath);
		END_CONFIG();

		BEGIN_CONFIG(AppMgr);
			CONFIG_ATTRIBUTE(addr,m_appMgrAddr);
			CONFIG_ATTRIBUTE(port,m_appMgrPort);
		END_CONFIG();

		CONFIG_RETURN();
	}

}
