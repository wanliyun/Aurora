#include "pch.h"
#include "Socket.h"

namespace Aurora
{
	Socket::Socket () 
		:m_SocketID(INVALID_SOCKET)
	{ 
	}

	Socket::~Socket () 
	{ 
		close() ;
	}

	bool Socket::create( )
	{
		m_SocketID = SocketAPI::socket( AF_INET , SOCK_STREAM , 0 );
		return isValid();
	}


	void Socket::close () 
	{ 
		if( isValid() && !isSockError() ) 
		{
			SocketAPI::closesocket( m_SocketID );
		}
		m_SocketID = INVALID_SOCKET ;
	}

	bool Socket::connect( const SocketAddress & addr )
	{
		return SocketAPI::connect(m_SocketID,addr);
	}

	int32 Socket::send( const uint8 * buf, int32 len, int32 flags /*= 0*/ )
	{
		return SocketAPI::send( m_SocketID , buf , len , flags );
	}

	int32 Socket::send( const char * buf, int32 len, int32 flags /*= 0*/ )
	{
		return send((const uint8*)buf,len,flags);
	}

	int32 Socket::receive (uint8* buf, int32 len, int32 flags) 
	{ 
		return SocketAPI::recv( m_SocketID , buf , len , flags );
	}

	int32 Socket::receive( char * buf, int32 len, int32 flags /*= 0*/ )
	{
		return receive((uint8*)buf,len,flags);
	}

	int32 Socket::available ()const
	{ 
		return SocketAPI::availablesocket( m_SocketID );
	}

	uint16 Socket::getLinger ()const 
	{ 
		struct linger ling;
		int32 len = sizeof(ling);
		SocketAPI::getsockopt( m_SocketID , SOL_SOCKET , SO_LINGER , &ling , &len );

		return ling.l_linger;
	}

	SOCKET Socket::accept( SocketAddress & addr )
	{
		return SocketAPI::accept( m_SocketID, addr) ;
	}

	bool Socket::setLinger (uint16 lingertime) 
	{
		struct linger ling;

		ling.l_onoff = lingertime > 0 ? 1 : 0;
		ling.l_linger = lingertime;

		return SocketAPI::setsockopt( m_SocketID , SOL_SOCKET , SO_LINGER , &ling , sizeof(ling) );
	}

	uint32 Socket::getSockError()const 
	{ 
		return isSockError(); 
	}

	bool Socket::isNonBlocking ()const 
	{ 
		return SocketAPI::getsocketnonblocking( m_SocketID );
	}

	bool Socket::setNonBlocking (bool on ) 
	{ 
		return SocketAPI::setsocketnonblocking( m_SocketID , on );
	}

	uint32 Socket::getReceiveBufferSize ()const 
	{ 
		uint32 ReceiveBufferSize;
		int32 size = sizeof(ReceiveBufferSize);

		SocketAPI::getsockopt( m_SocketID , SOL_SOCKET , SO_RCVBUF , &ReceiveBufferSize, &size );

		return ReceiveBufferSize;
	}

	bool Socket::setReceiveBufferSize (uint32 size)
	{ 
		return (bool)(SocketAPI::setsockopt( m_SocketID , SOL_SOCKET , SO_RCVBUF , &size, sizeof(uint32) ) ) ;
	}

	uint32 Socket::getSendBufferSize ()const 
	{ 
		uint32 SendBufferSize;
		int32 size = sizeof(SendBufferSize);

		SocketAPI::getsockopt( m_SocketID , SOL_SOCKET , SO_SNDBUF , &SendBufferSize, &size );

		return SendBufferSize;
	}

	bool Socket::setSendBufferSize (uint32 size)
	{ 
		return (bool)(SocketAPI::setsockopt( m_SocketID , SOL_SOCKET , SO_SNDBUF , &size, sizeof(uint32) ) );
	}


	bool Socket::isValid ()const 
	{
		return m_SocketID!=INVALID_SOCKET; 
	}

	SOCKET Socket::getSOCKET ()const 
	{ 
		return m_SocketID; 
	}

	bool Socket::isSockError()const
	{
		int error;
		int32 len = sizeof(error);
		int32 Result = SocketAPI::getsockopt_ex2( m_SocketID , SOL_SOCKET , SO_ERROR , &error, &len );
		return Result !=0;
	}

	bool Socket::bind( const SocketAddress & addr )
	{
		return SocketAPI::bind( m_SocketID , addr);
	}

	bool Socket::listen( int backlog )
	{
		return SocketAPI::listen( m_SocketID , backlog );
	}

	bool Socket::isReuseAddr ()const
	{
		int reuse;
		int32 len = sizeof(reuse);
		SocketAPI::getsockopt( m_SocketID , SOL_SOCKET , SO_REUSEADDR , &reuse , &len );
		return reuse == 1;
	}

	bool Socket::setReuseAddr ( bool on )
	{
		int32 opt = on == true ? 1 : 0;
		return SocketAPI::setsockopt( m_SocketID , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt) );
	}

	void Socket::Attach( SOCKET sock )
	{
		if(isValid())
			close();
		m_SocketID = sock;
	}

	bool Socket::shutdown( int32 how )
	{
		return SocketAPI::shutdown(m_SocketID,how);
	}

	void Socket::Detach()
	{
		m_SocketID=INVALID_SOCKET;
	}

}
