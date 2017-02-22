
#include "pch.h"
#include "CellAppConfig.h"


using namespace std;

namespace Aurora
{
	CellAppConfig::CellAppConfig():ServerAppConfigBase(std::string("CellApp"))
	{

	}

	CellAppConfig::~CellAppConfig()
	{

	}


	bool CellAppConfig::OnInit( const XMLElementWrapper & xmlEle )
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
