
#include "pch.h"
#include "NetworkConn.h"
#include "AppBase.h"
#include "network/PacketBase.h"
#include "binary_stream.h"
#include "memory_stream.h"
#include <interface/IPacketFilter.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>

namespace Aurora
{
	bool NetworkConnHolder::DoConnect(AppBase & app, const SocketAddress & addr )
	{
		m_pNetworkConn = app.DoConnect(addr);
		if(m_pNetworkConn != 0)
		{
			m_remoteAddr = addr;
			return true;
		}

		return false;
	}

	void NetworkConnHolder::DoDisconnect()
	{
		if(m_pNetworkConn)
		{
			m_pNetworkConn->Close();
			m_pNetworkConn = 0;
		}
	}

	void NetworkConnHolder::SendPacket(const PacketBase & pkt)
	{
		MY_ASSERT(m_pNetworkConn);
		m_pNetworkConn->SendPacket(pkt);
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	NetworkConn::NetworkConn( AppBase & app,bool bExtern /*= false*/ )
		:m_theApp(app)
		,m_bIsExtern(bExtern)
		,m_bError(false)
		,m_bReadRegisted(false)
		,m_bWriteRegisted(false)
		,m_bClosing(false)
		,m_bCloseWhileAllSent(false)
		,m_pHandler(0)
		,m_bClosed(false)
		,m_bEnableCloseCountingDown(false)
		,m_pIPacketFilter(0)
		,m_pBufferEvent(0)
	{

	}

	NetworkConn::~NetworkConn()
	{
		SAFE_DELETE(m_pIPacketFilter);
		m_pHandler = 0;
		if(m_pBufferEvent)
		{
			bufferevent_free(m_pBufferEvent);
			m_pBufferEvent = 0;
		}
	}

	bool NetworkConn::Attach( SOCKET sock,const SocketAddress & addr )
	{
		MY_ASSERT(m_pBufferEvent == 0);
		MY_ASSERT(m_theApp.GetEventBase());

		evutil_make_socket_nonblocking(sock);
		m_pBufferEvent = bufferevent_socket_new(m_theApp.GetEventBase(),sock,BEV_OPT_CLOSE_ON_FREE);
		if(0==m_pBufferEvent)
		{
			Log_ErrorF("bufferevent_socket_new() FAILED");
			return false;
		}
		m_remoteAddr = addr;
		bufferevent_setcb(m_pBufferEvent,func_bufferevent_read_cb,0,func_fufferevent_error_cb,this);
		bufferevent_enable(m_pBufferEvent,EV_READ | EV_WRITE);
		
		return true;
	}

	bool NetworkConn::SendPacket( const PacketBase & pkt )
	{
		MemoryOStream os;
		if(!WritePacketToStream(pkt,os))
			return false;
		return SendRawPacket((int8 *)os.data(),os.size());
	}

	bool NetworkConn::SendRawPacket(const int8 * pPacketData,uint16 nPacketLen )
	{
		bool bRet;
		if(m_pIPacketFilter && nPacketLen > sizeof(uint16))
		{
			MemoryOStream os;
			os.addBlob(pPacketData,sizeof(uint16));
			m_pIPacketFilter->Send(pPacketData + sizeof(uint16),nPacketLen - sizeof(uint16),os);
			bRet = doSend((int8 *)os.data(),os.size());
		}
		else
		{
			bRet = doSend(pPacketData,nPacketLen);;
		}

		return bRet;
	}


	void NetworkConn::SetPacketHandler( IPacketHandler * pHandler )
	{
		MY_ASSERT(m_pHandler == 0);
		m_pHandler = pHandler;
	}

	bool NetworkConn::Tick( float fDeltaTime )
	{
		if(m_bClosed)
			return false;
		if(m_bEnableCloseCountingDown)
		{
			m_fTimeElapse += fDeltaTime;
			m_bClosing = m_fTimeElapse > m_fTargetCloseTime;
		}
		if(m_bClosing || m_bError)
		{
			if(m_pBufferEvent)
			{
				bufferevent_free(m_pBufferEvent);
				m_pBufferEvent = 0;
			}
			m_bClosed = true;

			if(m_pHandler)
				m_pHandler->OnLoseConn(*this);
			else
				m_theApp.OnNetworkConnLose(*this);
		}

		return true;
	}

	bool NetworkConn::WritePacketToStream( const PacketBase & pkt,BinaryOStream & os )
	{
		MemoryOStream tmpOs;
		//填写Packet;
		{
			pkt.GetHeader().WriteToStream(tmpOs);//将Header写入缓冲区;
			pkt.WriteToStream(tmpOs);//将Packet写入缓冲区;
		}

		//填写长度;
		{
			uint16 nPktLen = uint16(tmpOs.size());
			nPktLen = ::htons(nPktLen);
			os.addBlob(&nPktLen,sizeof(uint16));
			os.addBlob(tmpOs.data(),tmpOs.size());
		}
		return true;
	}

	void NetworkConn::SetPacketFilter( IPacketFilter * pIPacketFilter )
	{
		SAFE_DELETE(m_pIPacketFilter);
		m_pIPacketFilter = pIPacketFilter;
	}

	void NetworkConn::func_bufferevent_read_cb( struct bufferevent *bev, void *ctx )
	{
		MY_ASSERT(ctx);
		NetworkConn & theConn = *( ( NetworkConn * )(ctx) );
		MY_ASSERT( theConn.GetBufferEvent() == bev);
		theConn.onRead();
	}

	void NetworkConn::func_fufferevent_error_cb( struct bufferevent *bev, short error, void *ctx )
	{
		MY_ASSERT(ctx);
		NetworkConn & theConn = *( ( NetworkConn * )(ctx) );
		MY_ASSERT( theConn.GetBufferEvent() == bev);
		theConn.m_bError = true;
	}

	void NetworkConn::onRead()
	{
		MY_ASSERT(m_pBufferEvent != 0);
		evbuffer * pBuffer = bufferevent_get_input(m_pBufferEvent);
		MY_ASSERT(pBuffer);

		for(;;)
		{
			// Step1.准备数据;
			size_t nLen = evbuffer_get_length(pBuffer);

			uint16 nPacketLen;

			const size_t HEADER_LEN = sizeof(nPacketLen);
			if(nLen < HEADER_LEN)
				break;
		
			MY_ASSERT( HEADER_LEN == evbuffer_copyout(pBuffer,&nPacketLen,HEADER_LEN) );
			nPacketLen = ntohs(nPacketLen);
			if(PacketBase::MAX_PACKET_LEN < nPacketLen)
			{
				m_bError = true;
				Log_ErrorF("PacketBase::MAX_PACKET_LEN < nPacketLen(%u)",nPacketLen);
				break;
			}

			if( nLen - HEADER_LEN < nPacketLen )
				break;
		
			evbuffer_drain(pBuffer,HEADER_LEN);
			static int8 s_recvBuff[PacketBase::MAX_PACKET_LEN];
			MY_ASSERT( nPacketLen == evbuffer_remove(pBuffer,s_recvBuff,nPacketLen) );


			//Step2.解析\处理数据包;
			std::auto_ptr<BinaryIStream> pIs;
			if(m_pIPacketFilter)
			{
				pIs = std::auto_ptr<BinaryIStream>(new MemoryOStream());
				m_pIPacketFilter->Recv(s_recvBuff,nPacketLen, *(MemoryOStream *)(pIs.get()));
			}
			else
			{
				pIs = std::auto_ptr<BinaryIStream>(new MemoryIStream(s_recvBuff,nPacketLen));
			}
			BinaryIStream & is = *pIs;

			SPacketHeader header(EPacketInterface_Dummy,uint16(-1));
			header.ReadFromStream(is);

			if(!is.error())
			{
				PacketBase * pNewPkt = PacketFactoryManager::get_mutable_instance().NewPacket(header.GetTypeID());
				if(pNewPkt)
				{
					pNewPkt->ReadFromStream(is);
					if(!is.error())
					{
						if(m_pHandler)
							m_pHandler->OnRecvPacket(*this,*pNewPkt);
						else
							m_theApp.OnNetworkConnRecvPacket(*this,*pNewPkt);
					}
					else
					{
						Log_ErrorF("NetworkConn::onRead() bad packet interface=%d type=%d idx=%d\n",header.GetInterface(),header.GetTypeID(),header.GetIndex());
					}
					PacketFactoryManager::get_mutable_instance().DeletePacket(pNewPkt);
				}
				else if(!pNewPkt)
				{
					Log_ErrorF("NetworkConn::onRead() bad packet interface=%d type=%d idx=%d\n",header.GetInterface(),header.GetTypeID(),header.GetIndex());
				}
			}
			else
			{
				Log_ErrorF("Read Header Failed\n");
			}
		}

	}

	bool NetworkConn::doSend( const int8 * pData,uint16 nDataLen )
	{
		MY_ASSERT(m_pBufferEvent != 0);
		evbuffer * pBuffer = bufferevent_get_output(m_pBufferEvent);
		MY_ASSERT(pBuffer);
		return 0 == evbuffer_add(pBuffer,pData,nDataLen);
	}



}
