#ifndef _DB_MANAGER_CONFIG_H_
#define _DB_MANAGER_CONFIG_H_

#include "Types.h"

#include <ServerAppConfigBase.h>
#include <boost/serialization/singleton.hpp>


namespace Aurora
{
	class DBManagerConfig : public ServerAppConfigBase ,public boost::serialization::singleton<DBManagerConfig>
	{
	public:
		DBManagerConfig();
		~DBManagerConfig();

		std::string m_appMgrAddr;
		uint16		m_appMgrPort;

	private:
		virtual bool OnInit(const XMLElementWrapper & xmlEle);
	};
}

#endif
