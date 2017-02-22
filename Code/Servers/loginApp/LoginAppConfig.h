#ifndef _LOGIN_APP_CONFIG_H_
#define _LOGIN_APP_CONFIG_H_

#include <ServerAppConfigBase.h>
#include <boost/serialization/singleton.hpp>


namespace Aurora
{
	class LoginAppConfig : public ServerAppConfigBase ,public boost::serialization::singleton<LoginAppConfig>
	{
	public:
		LoginAppConfig();
		~LoginAppConfig();

		std::string  m_strPriKeyPath;

		std::string m_appMgrAddr;
		uint16		m_appMgrPort;

	private:
		virtual bool OnInit(const XMLElementWrapper & xmlEle);
	};
}

#endif
