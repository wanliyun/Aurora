#ifndef _SERVER_APP_CONFIG_BASE_H_
#define _SERVER_APP_CONFIG_BASE_H_

#include <network/MySocketAPI.h>
#include <AppConfigBase.h>


namespace Aurora
{
	class ServerAppConfigBase : public AppConfigBase
	{
	public:
		ServerAppConfigBase(const std::string & prefix);
		~ServerAppConfigBase();
		
		int m_nListenLog;

		bool m_bInternBind;
		bool m_bExternBind;
		std::vector<SocketAddress> m_internBindAddrs;	//可用的绑定地址;
		std::vector<SocketAddress> m_externBindAddrs;	//可用的绑定地址;

	protected:
		virtual bool OnInit( const XMLElementWrapper & xmlEle );
	};
}

#endif
