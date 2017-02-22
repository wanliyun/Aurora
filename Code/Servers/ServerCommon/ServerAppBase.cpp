
#include "pch.h"
#include "ServerAppBase.h"
#include "Debug.h"
#include "Utils.h"
#include "NetworkConn.h"
#include "ServerAppConfigBase.h"
#include <event2/listener.h>
#include <event2/bufferevent.h>
namespace Aurora
{
	ServerAppBase::ServerAppBase(AppConfigBase & appConfig)
		:AppBase(appConfig)
		,m_appId(uint32(-1))
		,m_pInternEvListener(0)
		,m_pExternEvListener(0)
	{

	}
	
	ServerAppBase::~ServerAppBase()
	{

	}

	bool ServerAppBase::InitServer( const ServerAppConfigBase & config )
	{
		LogF("Name=[%s] Version=[%s]\n",GetAppName().c_str(),GetAppVersion().c_str());

		MY_ASSERT(0 == m_pInternEvListener);
		MY_ASSERT(0 == m_pExternEvListener);

		if(config.m_bInternBind)
		{
			bool bBinded = false;
			for(std::vector<SocketAddress>::const_iterator iter = config.m_internBindAddrs.begin();iter!=config.m_internBindAddrs.end();++iter)
			{
				Log_ErrorF("Trying to bind to Intern [%s]\n",(*iter).ToString().c_str());

				sockaddr_in addr_in_;
				iter->Fill(addr_in_);


				m_pInternEvListener = evconnlistener_new_bind(m_pEventBase, func_listen_cb, this,
					LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
					(struct sockaddr*)&addr_in_,
					sizeof(addr_in_));
				if(!m_pInternEvListener)
					continue;

				bBinded = true;
				m_internBindAddr = *iter;
				break;
			}
			if(!bBinded)
			{
				Log_ErrorF("Bind failed\n");
				return false;
			}
			
			Log_OKF("\tIntern Bind Addr = %s\n",m_internBindAddr.ToString().c_str());
			MY_ASSERT(m_pInternEvListener);
			evconnlistener_set_error_cb(m_pInternEvListener,func_listen_error_cb);
		}
	
		if(config.m_bExternBind)
		{
			bool bBinded = false;
			for(std::vector<SocketAddress>::const_iterator iter = config.m_externBindAddrs.begin();iter!=config.m_externBindAddrs.end();++iter)
			{
				Log_ErrorF("Trying to bind to Extern [%s]\n",(*iter).ToString().c_str());

				sockaddr_in addr_in_;
				iter->Fill(addr_in_);


				m_pInternEvListener = evconnlistener_new_bind(m_pEventBase, func_listen_cb, this,
					LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
					(struct sockaddr*)&addr_in_,
					sizeof(addr_in_));
				if(!m_pInternEvListener)
					continue;

				bBinded = true;
				m_externBindAddr = *iter;
				break;
			}
			if(!bBinded)
			{
				Log_ErrorF("Bind secondary failed\n");
				return false;
			}

			Log_OKF("\tExtern bind Addr = %s\n",m_externBindAddr.ToString().c_str());
			MY_ASSERT(m_pInternEvListener);
			evconnlistener_set_error_cb(m_pInternEvListener,func_listen_error_cb);
		}

		Log_OKF("ServerAppBase: Init Succeed\n");
		return true;
	}

	void ServerAppBase::FiniServer()
	{
		if(m_pExternEvListener)
		{
			evconnlistener_free(m_pExternEvListener);
			m_pExternEvListener = 0;
		}

		if(m_pInternEvListener)
		{
			evconnlistener_free(m_pInternEvListener);
			m_pInternEvListener = 0;
		}
	}

	void ServerAppBase::PreRun()
	{
	}

	void ServerAppBase::PostRun()
	{
	}

	void ServerAppBase::OnNewConn( bool isSecondary,SOCKET sock,const SocketAddress & addr )
	{
		NetworkConn * pNewConn = newNetworkConn(sock,addr,isSecondary);
		if(pNewConn)
		{
			this->OnNewConn(*pNewConn);
		}
		else
		{
			Log_ErrorF("newNetworkConn(sock,addr,isSecondary)(%d,%s,%d) FAILED\n",sock,addr.ToString(),isSecondary);
		}
	}

	void ServerAppBase::func_listen_cb( evconnlistener * listener,evutil_socket_t sock,struct sockaddr * addr,int len,void * pUser )
	{
		ServerAppBase * const pThisApp = static_cast<ServerAppBase * >(pUser);
		MY_ASSERT(pThisApp );
		
		pThisApp->OnNewConn(listener == pThisApp->m_pInternEvListener,sock,SocketAddress( * (sockaddr_in *)addr));
	}

	void ServerAppBase::func_listen_error_cb( struct evconnlistener *listener, void *ctx )
	{
		Log_ErrorF("");
	}

}
