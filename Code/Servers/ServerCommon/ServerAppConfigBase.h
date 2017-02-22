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
		std::vector<SocketAddress> m_internBindAddrs;	//���õİ󶨵�ַ;
		std::vector<SocketAddress> m_externBindAddrs;	//���õİ󶨵�ַ;

	protected:
		virtual bool OnInit( const XMLElementWrapper & xmlEle );
	};
}

#endif
