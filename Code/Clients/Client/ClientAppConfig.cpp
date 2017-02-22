
#include "pch.h"
#include "ClientAppConfig.h"
#include <XMLWarpper.h>

using namespace std;

namespace Aurora
{
	ClientAppConfig::ClientAppConfig():AppConfigBase("Client/ClientConfig.xml",std::string("Client"))
	{

	}

	ClientAppConfig::~ClientAppConfig()
	{

	}

	bool ClientAppConfig::OnInit( const XMLElementWrapper & xmlEle )
	{
		PREPARE_CONFIG();

		BEGIN_CONFIG(Account);
			CONFIG_ATTRIBUTE(EntityType,m_strPlayerType);
		END_CONFIG();

		BEGIN_CONFIG(PublicKey);
			CONFIG_ATTRIBUTE(path,m_strPubKeyPath);
		END_CONFIG();

		BEGIN_CONFIG(LoginApps);
			std::string addr;
			uint16 port;
			BEGIN_CONFIG_LIST(LoginApp);
				CONFIG_ATTRIBUTE(addr,addr);
				CONFIG_ATTRIBUTE(port,port);
				if(bSucc)
					m_loginAppAddrs.push_back(SocketAddress(addr.c_str(),port));
			END_CONFIG_LIST();
		END_CONFIG();

		CONFIG_RETURN();
	}

}
