
#include "pch.h"
#include "AppManagerConfig.h"

using namespace std;

namespace Aurora
{
	AppManagerConfig::AppManagerConfig():ServerAppConfigBase(std::string("AppManager"))
	{

	}

	AppManagerConfig::~AppManagerConfig()
	{

	}


	bool AppManagerConfig::OnInit( const XMLElementWrapper & xmlEle )
	{
		if(!ServerAppConfigBase::OnInit(xmlEle))
			return false;
		bool bSucc = true;
		return bSucc;
	}

}
