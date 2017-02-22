
#include "pch.h"
#include "ServerAppConfigBase.h"

namespace Aurora
{
	ServerAppConfigBase::ServerAppConfigBase( const std::string & prefix )
		:AppConfigBase(std::string("Server/ServerConfig.xml"),prefix)
		,m_bInternBind(false)
		,m_bExternBind(false)
	{

	}

	ServerAppConfigBase::~ServerAppConfigBase()
	{

	}

	bool ServerAppConfigBase::OnInit( const XMLElementWrapper & xmlEle )
	{
		PREPARE_CONFIG();

		BEGIN_CONFIG(listen);
			CONFIG_ATTRIBUTE(internBind,m_bInternBind);
			CONFIG_ATTRIBUTE(externBind,m_bExternBind);
		END_CONFIG();

		if(m_bInternBind)
		{
			BEGIN_CONFIG(internBindAddrs);
			std::string addr;
			uint16 port;
			BEGIN_CONFIG_LIST(bindAddr);
			CONFIG_ATTRIBUTE(addr,addr);
			CONFIG_ATTRIBUTE(port,port);
			if(bSucc)
				m_internBindAddrs.push_back(SocketAddress(addr.c_str(),port));
			END_CONFIG_LIST();
			END_CONFIG();
		}


		if(m_bExternBind)
		{
			BEGIN_CONFIG(externBindAddrs);
			std::string addr;
			uint16 port;
			BEGIN_CONFIG_LIST(bindAddr);
			CONFIG_ATTRIBUTE(addr,addr);
			CONFIG_ATTRIBUTE(port,port);
			if(bSucc)
				m_externBindAddrs.push_back(SocketAddress(addr.c_str(),port));
			END_CONFIG_LIST();
			END_CONFIG();
		}


		CONFIG_RETURN();
	}
}

