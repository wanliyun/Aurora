
#include "pch.h"
#include "AppBase.h"
#include "Debug.h"
#include "Utils.h"
#include "NetworkConn.h"
#include <FileSystem.h>
#include "AppConfigBase.h"
#include <ctime>

#ifdef WIN32
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#endif

namespace Aurora
{
	AppBase::AppBase( AppConfigBase & appConfig )
		:m_appConfig(appConfig)
		,m_pEventBase(0)
		,m_nCurrentTick(0)
		,m_nShutdownTicks(-1)
		,m_pEvTickTimer(0)
	{
	}

	bool AppBase::Init( float fTickTime)
	{
		::srand(unsigned(time(0)));
		InitRootDir();
		
		if(!FileSystem::get_mutable_instance().Init("./Paths.xml"))
		{
			Log_ErrorF("Load Paths.xml");
			return false;
		}

		m_tickDelta.tv_sec = uint32(fTickTime);
		m_tickDelta.tv_usec = uint32( (fTickTime - float(m_tickDelta.tv_sec)) * 1000 * 1000);

		m_pEventBase  = event_base_new();
		if(!m_pEventBase)
		{
			Log_ErrorF("event_base_new() FAILED");
			return false;
		}

		if(!m_appConfig.Init())
		{
			Log_ErrorF("m_appConfig.Init() FAILED");
			return false;
		}
		m_pEvTickTimer = evtimer_new(m_pEventBase,AppBase::on_tick,this);
		if(!m_pEvTickTimer)
		{
			Log_ErrorF("evtimer_new() FAILED");
			return false;
		}

		return this->OnInit();
	}

	void AppBase::Run()
	{
		MY_ASSERT(m_pEventBase);

		PreRun();
		//添加Tick事件;
		AddTickEv();

		//主循环,知道程序结束;
		event_base_dispatch(m_pEventBase);

		PostRun();
	}

	void AppBase::Fini()
	{
		this->OnFini();

		std::map<SOCKET,NetworkConn * > tmp;
		tmp.swap(m_mapConns);
		for(std::map<SOCKET,NetworkConn * >::iterator iter = tmp.begin();iter != tmp.end();++iter)
		{
			SAFE_DELETE(iter->second);
		}

		if(m_pEvTickTimer)
		{
			evtimer_del(m_pEvTickTimer);
			m_pEvTickTimer = 0;
		}

		if(m_pEventBase)
		{
			event_base_free(m_pEventBase);
			m_pEventBase = 0;
		}
	}

	void AppBase::AddTickEv()
	{
		MY_ASSERT(m_pEventBase && m_pEvTickTimer);
		evtimer_add(m_pEvTickTimer,&m_tickDelta);
		++m_nCurrentTick;
	}

	void AppBase::on_tick( int sock, short event, void* arg )
	{
		AppBase * const pThisApp = static_cast<AppBase * >(arg);
		MY_ASSERT(pThisApp );

		float lastTicktime = 1.0f; //TODO;

		//执行任务;
		pThisApp->tickConnList(lastTicktime);	


		//调用回调;
		pThisApp->OnTick(lastTicktime);

		//退出处理;
		if(pThisApp->m_nShutdownTicks >= 0)
		{
			pThisApp->m_nShutdownTicks -= 1;
			if(pThisApp->m_nShutdownTicks == 0)
			{
				event_base_loopbreak(pThisApp->m_pEventBase);
				return;	//函数返回;
			}
		}

		//重新添加Tick事件;
		pThisApp->AddTickEv();
	}

	void AppBase::InitRootDir()
	{
#ifdef WIN32
		WCHAR szExeFileName[_MAX_PATH];

		GetModuleFileNameW( GetModuleHandle(NULL), szExeFileName, sizeof(szExeFileName));
		PathRemoveFileSpecW(szExeFileName);
		SetCurrentDirectoryW( szExeFileName );
#endif
	}

	NetworkConn * AppBase::DoConnect( const SocketAddress & addr )
	{
		Socket sock;
		if(!sock.create())
			return false;
		if(!sock.connect(addr))
			return false;
		SOCKET sock_ = sock.getSOCKET();
		sock.Detach();

		return newNetworkConn(sock_,addr);
	}

	NetworkConn * AppBase::newNetworkConn( SOCKET sock ,const SocketAddress & addr, bool isSecondary /*= false*/)
	{
		MY_ASSERT(m_mapConns.find(sock) == m_mapConns.end());
		NetworkConn * pNewConn = new NetworkConn(*this,isSecondary);
		if(!pNewConn->Attach(sock,addr))
		{
			Log_ErrorF("pNewConn->Attach(%d,%s) FAILED",sock,addr.ToString());
			SAFE_DELETE(pNewConn);
			return 0;
		}
		m_mapConns.insert(std::make_pair(sock,pNewConn));
		return pNewConn;
	}

	void AppBase::tickConnList( float fDeltaTime )
	{
		std::vector<SOCKET> vDelSockets;
		for(std::map<SOCKET,NetworkConn * >::iterator iter = m_mapConns.begin();iter != m_mapConns.end();++iter)
		{
			NetworkConn * pConn = iter->second;
			MY_ASSERT(pConn);
			if( !pConn->Tick(fDeltaTime))
			{
				vDelSockets.push_back(iter->first);
				continue;
			}
		}

		for(std::vector<SOCKET>::const_iterator iter = vDelSockets.begin();iter != vDelSockets.end();++iter)
		{
			const SOCKET sock = *iter;
			MY_ASSERT(m_mapConns.find(sock) != m_mapConns.end());
			SAFE_DELETE(m_mapConns[sock]);
			m_mapConns.erase(sock);
		}
	}

}
