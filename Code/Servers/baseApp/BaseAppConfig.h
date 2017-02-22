#ifndef _CELL_APP_CONFIG_H_
#define _CELL_APP_CONFIG_H_

#include "Types.h"

#include <ServerAppConfigBase.h>
#include <boost/serialization/singleton.hpp>


namespace Aurora
{
	class BaseAppConfig : public ServerAppConfigBase ,public boost::serialization::singleton<BaseAppConfig>
	{
	public:
		BaseAppConfig();
		~BaseAppConfig();

		std::string m_appMgrAddr;
		uint16		m_appMgrPort;
		std::string m_dbConnStr;
	private:
		virtual bool OnInit(const XMLElementWrapper & xmlEle);
	};
}

#endif
