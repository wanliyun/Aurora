#ifndef _CLIENT_APP_CONFIG_H_
#define _CLIENT_APP_CONFIG_H_

#include <network/MySocketAPI.h>
#include <Types.h>
#include <AppConfigBase.h>
#include <boost/serialization/singleton.hpp>


namespace Aurora
{
	class ClientAppConfig : public AppConfigBase ,public boost::serialization::singleton<ClientAppConfig>
	{
	public:
		ClientAppConfig();
		~ClientAppConfig();

		std::vector<SocketAddress> m_loginAppAddrs;
		std::string m_strPlayerType;
		std::string m_strPubKeyPath;
	private:
		virtual bool OnInit(const XMLElementWrapper & xmlEle);
	};
}

#endif
