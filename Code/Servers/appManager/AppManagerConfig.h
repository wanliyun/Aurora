#ifndef _APP_MANAGER_CONFIG_H_
#define _APP_MANAGER_CONFIG_H_

#include "Types.h"
#include <ServerAppConfigBase.h>
#include <boost/serialization/singleton.hpp>


namespace Aurora
{
	class AppManagerConfig : public ServerAppConfigBase ,public boost::serialization::singleton<AppManagerConfig>
	{
	public:
		AppManagerConfig();
		~AppManagerConfig();

		std::string m_bindAddr;
		uint16		m_bindPort;
	private:
		virtual bool OnInit(const XMLElementWrapper & xmlEle);
	};
}

#endif
