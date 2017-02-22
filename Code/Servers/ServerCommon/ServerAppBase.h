#ifndef _SERVER_APP_BASE_H_
#define _SERVER_APP_BASE_H_

#include <AppBase.h>
#include <map>

struct evconnlistener;
namespace Aurora
{
	class ConnBase;
	class ServerAppConfigBase;
	class ServerAppBase : public AppBase
	{
	public:
		ServerAppBase(AppConfigBase & appConfig);
		virtual ~ServerAppBase();

		bool InitServer(const ServerAppConfigBase & config);
		void FiniServer();
		AppID GetAppID()const{return m_appId;}

		const SocketAddress & GetInternBindAddr()const{return m_internBindAddr;}
		const SocketAddress & GetExternBindAddr()const{return m_externBindAddr;}
	private:
		void OnNewConn(bool isSecondary,SOCKET sock,const SocketAddress & addr);
	protected:
		virtual void OnNewConn(NetworkConn & newConn) = 0;
	protected:
		virtual void PreRun();
		virtual void PostRun();
	protected:
		AppID			m_appId;

		SocketAddress	m_internBindAddr;
		evconnlistener *m_pInternEvListener;

		SocketAddress	m_externBindAddr;
		evconnlistener *m_pExternEvListener;

	private:
		static void func_listen_cb(evconnlistener * listener,evutil_socket_t sock,struct sockaddr * addr,int len,void * pUser);
		static void func_listen_error_cb(struct evconnlistener *listener, void *ctx);
	};
}

#endif
