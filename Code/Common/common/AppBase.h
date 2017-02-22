#ifndef _APP_BASE_H_
#define _APP_BASE_H_

#include <event2/event.h>
#include <network/Socket.h>

namespace Aurora
{
	class NetworkConn;
	class AppConfigBase;
	class PacketBase;

	//管理所有的NetworkConn;
	class AppBase
	{
	public:
		AppBase(AppConfigBase & appConfig);
		virtual ~AppBase(){}
		bool Init(float fTickTime);
		void Run();
		void Fini();

		void ShutDown(int nLeftTicks = 1 ){m_nShutdownTicks = nLeftTicks;}
		bool IsShutingDown()const{return m_nShutdownTicks >=0;}

		AppConfigBase & GetAppConfig(){return m_appConfig;}

		virtual void OnNetworkConnRecvPacket(NetworkConn & conn,const PacketBase & pkt){}
		virtual void OnNetworkConnLose(NetworkConn & conn){}

		event_base *	GetEventBase()const{return m_pEventBase;}

		NetworkConn * DoConnect(const SocketAddress & addr);
	protected:
		virtual bool OnInit() = 0;
		virtual void PreRun() {}
		virtual void PostRun() {}
		virtual void OnFini() = 0;
		virtual void OnTick(float fTimeDelta) = 0;

		virtual std::string GetAppName()const = 0;
		virtual std::string GetAppVersion()const = 0;
		uint32 getCurrentTick()const{return m_nCurrentTick;}

		NetworkConn * newNetworkConn(SOCKET sock,const SocketAddress & addr,bool isSecondary = false);
	private:
		void			AddTickEv();

		void			InitRootDir();

		void			tickConnList(float fDeltaTime);
	protected:
		event_base *	m_pEventBase;
		std::map<SOCKET,NetworkConn * > m_mapConns;		//所有的NetworkConn都在这里;
	private:
		AppConfigBase & m_appConfig;
		event *			m_pEvTickTimer;
		timeval			m_tickDelta;
		uint32			m_nCurrentTick;
		int32			m_nShutdownTicks;
	private:
		static void on_tick(int sock, short event, void* arg);
	};
}

#endif
