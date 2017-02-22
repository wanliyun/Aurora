#ifndef __APPMGR_LOGIN_APP_H_
#define __APPMGR_LOGIN_APP_H_

#include "AppMgr_App.h"
 
namespace Aurora
{
	class AppMgr_LoginApp : public AppMgr_App
	{
	public:
		AppMgr_LoginApp(AppID nAppID,NetworkConn & conn):
			AppMgr_App(nAppID,conn)
		{
	
		}
	private:
		//~IAppPacketHandler;
		virtual void OnRecvPacket(const PacketBase & pkt);
		virtual void OnLoseConn();
		//IAppPacketHandler;

		//////////////////////////////////////////////////////////////////////////
		//处理来自LoginApp的消息;
		void HandlePacket(const AppManagerInterface::PktLoginRquest & pkt);
		//////////////////////////////////////////////////////////////////////////
	};


	class LoginAppManager : public AppMgr_App_Manager ,public boost::serialization::singleton<LoginAppManager>
	{
	public:
		LoginAppManager();
		~LoginAppManager();

	private:
		//override AppMgr_App_Manager;
		virtual bool OnWhoAmIPkt(NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt);
		//~override AppMgr_App_Manager;
	};
}

#endif
