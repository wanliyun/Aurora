#ifndef _NETWORK_CONN_H_
#define _NETWORK_CONN_H_

#include "network/Socket.h"
#include "network/PacketBase.h"
#include <deque>
#include <map>

struct bufferevent;
namespace Aurora
{
	class AppBase;
	class NetworkConn;

	struct IPacketHandler
	{
		virtual ~IPacketHandler(){}

		virtual void OnRecvPacket(NetworkConn & conn, const PacketBase & pkt) = 0;
		virtual void OnLoseConn(NetworkConn & conn) = 0;
	};

	template<typename IDType>
	class OneNetworkConnOwner
	{
	public:
		NetworkConn & GetConn(){return m_conn;}
		IDType GetOwnerID()const{return m_id;}
		bool SendPacket(const PacketBase & pkt){return m_conn.SendPacket(pkt);}
	protected:
		explicit OneNetworkConnOwner(IDType id,NetworkConn & conn):m_conn(conn),m_id(id){}
		~OneNetworkConnOwner(){}
	private:
		NetworkConn & m_conn;
		const IDType m_id;
	};

	template<typename IDType>
	class MultiNetworkConnOwnerManager
	{
	public:
		uint32 GetNumConns(){return m_mapId2Owner.size();}
	protected:
		MultiNetworkConnOwnerManager(){}
		~MultiNetworkConnOwnerManager(){}
 
		typedef OneNetworkConnOwner<IDType>	Owner_T;
		typedef std::map<IDType, Owner_T * > Id2Owner_T;
		typedef std::map<NetworkConn * ,Owner_T *  > Conn2Owner_T;

		void addConn(IDType id,NetworkConn & conn,Owner_T & owner)
		{
			bool bRet = true;
			bRet = bRet && m_mapId2Owner.insert( std::make_pair(id,&owner) ).second;
			bRet = bRet && m_mapConn2Owner.insert( std::make_pair(&conn,&owner) ).second;
			MY_ASSERT(bRet);
		}

		void delConn(NetworkConn & conn)
		{
			Conn2Owner_T::iterator it = m_mapConn2Owner.find(&conn);
			if(it == m_mapConn2Owner.end())
				return;
			
			IDType id = it->second->GetOwnerID();
			MY_ASSERT(m_mapId2Owner.find(id) != m_mapId2Owner.end());

			m_mapConn2Owner.erase(&conn);
			m_mapId2Owner.erase(id);
		}

		void delConn(IDType id)
		{
			Id2Owner_T::iterator it = m_mapId2Owner.find(id);
			if(it == m_mapId2Owner.end())
				return;
			delConn(it->second->GetConn());
		}

		Owner_T * getOwner(IDType id)const
		{
			Id2Owner_T::const_iterator it = m_mapId2Owner.find(id);
			if(it == m_mapId2Owner.end())
				return 0;
			return it->second;
		}

		Owner_T * getOwner(NetworkConn & conn)const
		{
			Conn2Owner_T::const_iterator it = m_mapConn2Owner.find(& conn);
			if(it == m_mapConn2Owner.end())
				return 0;
			return it->second;
		}

		Id2Owner_T m_mapId2Owner;
		Conn2Owner_T m_mapConn2Owner;
	};

	class PacketBase;
	class NetworkConnHolder
	{
	public:
		NetworkConn * GetConn()const{return m_pNetworkConn;}
		const SocketAddress & GetRemoteAddr()const{return m_remoteAddr;}

		bool DoConnect(AppBase & app,const SocketAddress & addr);
		void DoDisconnect();

		void SendPacket(const PacketBase & pkt);
	protected:
		explicit NetworkConnHolder()
			:m_pNetworkConn(0)
		{

		}
		~NetworkConnHolder()
		{
			DoDisconnect();
		}

		

	private:
		NetworkConn * m_pNetworkConn;
		SocketAddress m_remoteAddr;
	};

// 	class MultiNetworkConnHolder
// 	{
// 	public:
// 		NetworkConn * GetConn()const{return m_pNetworkConn;}
// 		const SocketAddress & GetRemoteAddr()const{return m_remoteAddr;}
// 
// 		void SendPacket(const PacketBase & pkt);
// 	protected:
// 		explicit MultiNetworkConnHolder()
// 			:m_pNetworkConn(0)
// 		{
// 
// 		}
// 		~MultiNetworkConnHolder()
// 		{
// 			DoDisconnect();
// 		}
// 
// 		bool DoConnect(AppBase & app,const SocketAddress & addr);
// 		void DoDisconnect();
// 
// 	private:
// 		NetworkConn * m_pNetworkConn;
// 		SocketAddress m_remoteAddr;
// 	};

	struct IPacketFilter;

	class NetworkConn
	{
		friend AppBase;
		explicit NetworkConn(AppBase & app,bool bExtern = false);
		~NetworkConn();

	public:
		bool Attach(SOCKET sock,const SocketAddress & addr);

		bool Tick(float fDeltaTime);

		static bool WritePacketToStream(const  PacketBase & pkt,BinaryOStream & os);
		bool SendPacket(const  PacketBase & pkt);
		bool SendRawPacket(const int8 * pPacketData,uint16 nPacketLen);
		bool SendRawPacket(const void * pPacketData,uint16 nPacketLen){SendRawPacket((int8*)pPacketData,nPacketLen);}
		
		bool IsError()const{return m_bError;}

		void Close(){m_bClosing = true;}
		bool IsClosing()const{return m_bClosing;}
		
		inline void SetCloseWhileAllSent(){m_bCloseWhileAllSent = true;}
		inline void SetCloseCountingDown(float ftime)
		{
			m_bEnableCloseCountingDown = true;
			m_fTargetCloseTime = ftime;
			m_fTimeElapse = 0;
		}
		void SetPacketHandler(IPacketHandler * pHandler);
		IPacketHandler * GetPacketHandler()const{return m_pHandler;}

		const SocketAddress & GetRemoteAddr()const{return m_remoteAddr;}
		
		void SetPacketFilter(IPacketFilter * pIPacketFilter);

		bool IsValid()const{return 0!= m_pBufferEvent;}
		bufferevent  * GetBufferEvent()const{return m_pBufferEvent;}

		bool IsExtern()const{return m_bIsExtern;}

	protected:
		IPacketHandler * m_pHandler;	//不拥有Handler;
		IPacketFilter * m_pIPacketFilter;	//拥有该指针;
	private:
		bool doSend(const int8 * pData,uint16 nDataLen);
		void onRead();
	private:
		AppBase &		m_theApp;
		const bool		m_bIsExtern;		//是不是外网连接;
		bool			m_bReadRegisted;
		bool			m_bWriteRegisted;
		bool			m_bError;
		bool			m_bClosing;
		bool			m_bClosed;
		bool			m_bCloseWhileAllSent;

		bool			m_bEnableCloseCountingDown;
		float			m_fTargetCloseTime;
		float			m_fTimeElapse;
		SocketAddress	m_remoteAddr;
		bufferevent  *	m_pBufferEvent;

	private:
		NetworkConn(const NetworkConn &other);
		NetworkConn & operator = (const NetworkConn & other);
	private:
		static void func_bufferevent_read_cb(struct bufferevent *bev, void *ctx);  
		static void func_fufferevent_error_cb(struct bufferevent *bev, short error, void *ctx );
	};
}

#endif
