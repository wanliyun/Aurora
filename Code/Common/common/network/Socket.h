#ifndef __NETWORK_SOCKET_H__
#define __NETWORK_SOCKET_H__

#include "Types.h"
#include "MySocketAPI.h"

namespace Aurora
{
	class Socket 
	{
	public :
		Socket() ;
		~Socket();
	public :
		bool create() ;

		void Attach(SOCKET sock);
		void Detach();

		bool bind( const SocketAddress & addr) ;

		bool listen( int32 backlog ) ;

		void close () ;
		bool shutdown(int32 how);

		bool connect (const SocketAddress & addr) ;

		SOCKET accept( SocketAddress & addr) ;

		int32 send (const uint8 * buf, int32 len, int32 flags = 0) ;
		int32 send (const char * buf, int32 len, int32 flags = 0) ;
		int32 receive (uint8 * buf, int32 len, int32 flags = 0) ;
		int32 receive (char * buf, int32 len, int32 flags = 0) ;

		int32 available ()const ;

	public :

		// get/set socket's linger status
		uint16 getLinger ()const ;
		bool setLinger (uint16 lingertime) ;

		bool isReuseAddr ()const ;
		bool setReuseAddr (bool on = true) ;

		// get is Error
		uint32 getSockError()const ;

		// get/set socket's nonblocking status
		bool isNonBlocking ()const ;
		bool setNonBlocking (bool on = true) ;

		// get/set receive buffer size
		uint32 getReceiveBufferSize ()const ;
		bool setReceiveBufferSize (uint32 size) ;

		// get/set send buffer size
		uint32 getSendBufferSize ()const ;
		bool setSendBufferSize (uint32 size) ;

		uint16 getPort ()const ;
		uint32 getHostIP ()const ;

		// check if socket is valid
		bool isValid ()const ;

		// get socket descriptor
		SOCKET getSOCKET ()const ;

		bool isSockError()const ;

	public :
		SOCKET m_SocketID;
	};
}
#endif
