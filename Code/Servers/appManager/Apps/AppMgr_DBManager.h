#ifndef _APP_MGR_DBMGR_H_
#define _APP_MGR_DBMGR_H_

#include "NetworkConn.h"
#include "AppManagerInterface.h"

namespace Aurora
{
	class AppManager;
	class DBManagerManager;
	class AppMgr_DBManager  : public OneNetworkConnOwner<AppID>
	{
		friend class DBManagerManager;
	private:
		AppMgr_DBManager(NetworkConn & conn)
			:OneNetworkConnOwner<AppID>(0,conn)
		{
	
		}

		//~IAppPacketHandler;
		virtual void OnRecvPacket(const PacketBase & pkt);
		virtual void OnLoseConn();
		//IAppPacketHandler;
	};

	class DBManagerManager : public IPacketHandler ,public boost::serialization::singleton<DBManagerManager>
	{
	public:
		DBManagerManager();
		~DBManagerManager();
		
		void HandleWhoAmIPkt(NetworkConn & conn,const AppManagerInterface::PktWhoAmI & pkt);

		bool HasDBManager()const{return 0 != m_pDBManager;}
		AppMgr_DBManager * GetDBManager(){return m_pDBManager;}

	private:
		//override IPacketHandler;
		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt) ;
		virtual void OnLoseConn(NetworkConn & conn) ;
		//~override IPacketHandler;

	private:
		AppManager & m_theApp;
		AppMgr_DBManager * m_pDBManager;
	};
}

#endif
