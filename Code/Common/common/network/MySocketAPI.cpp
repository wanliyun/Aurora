#include "pch.h"
#include <sstream>
#include "MySocketAPI.h"


#ifndef WIN32
extern int32 errno;
#endif

char Error[_ESIZE] ;


SocketAddress::SocketAddress()
{
	m_addr = 0;
}

SocketAddress::SocketAddress( const char * host,uint16 port,uint16 salt /*= 0*/ )
{
	uint32 ip = inet_addr(host != 0 ? host : 0);
	port = ::htons(port);
	m_addr = ( uint64(ip) <<32 )| ( uint64(salt) << 16) | uint64(port);
}
SocketAddress::SocketAddress(const std::string & host,uint16 port,uint16 salt/* = 0*/)
{
	uint32 ip = inet_addr(host.c_str());
	port = ::htons(port);
	m_addr = ( uint64(ip) <<32 )| ( uint64(salt) << 16) | uint64(port);
}
SocketAddress::SocketAddress( uint32 ip,uint16 port,uint16 salt /*= 0*/ )
{
	port = ::htons(port);
	m_addr = ( uint64(ip) <<32 )| ( uint64(salt) << 16) | uint64(port);
}

SocketAddress::SocketAddress( const sockaddr_in & addr ,uint16 salt /*= 0*/)
{
	m_addr = ( uint64(addr.sin_addr.s_addr) <<32 )| ( uint64(0) << 16) | uint64(addr.sin_port);
}

void SocketAddress::Fill( sockaddr_in & addr )const
{
	addr.sin_family = AF_INET;
	addr.sin_port = uint16(m_addr);
	addr.sin_addr.s_addr = GetIP();
}

std::string SocketAddress::ToString()const
{
	std::stringstream ss;
	union UIP
	{
		uint32 ip;
		uint8 bytes[4];
	};
	UIP uip;
	uip.ip = GetIP();
	for(int i = 0 ;i<4;++i)
	{
		ss<<uint32(uip.bytes[i]);
		if(i!=3)
			ss<<".";
	}
	ss<<":"<<GetPort();
	return ss.str();
}

uint16 SocketAddress::GetPort() const
{
	return ntohs(uint16(m_addr));
}


SOCKET SocketAPI::socket( int32 domain , int32 type , int32 protocol ) 
{
	SOCKET s = ::socket(domain,type,protocol);

	if ( s == INVALID_SOCKET ) 
	{
#if __LINUX__
		switch ( errno ) 
		{
		case EPROTONOSUPPORT :
		case EMFILE : 
		case ENFILE : 
		case EACCES : 
		case ENOBUFS : 
		default : 
			{
				break;
			}
		}//end of switch
#elif _WIN32
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEAFNOSUPPORT : 
			strncpy_s( Error, "WSAEAFNOSUPPORT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEMFILE : 
			strncpy_s( Error, "WSAEMFILE", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy_s( Error, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAEPROTONOSUPPORT : 
			strncpy_s( Error, "WSAEPROTONOSUPPORT", _ESIZE ) ;
			break ;
		case WSAEPROTOTYPE : 
			strncpy_s( Error, "WSAEPROTOTYPE", _ESIZE ) ;
			break ;
		case WSAESOCKTNOSUPPORT : 
			strncpy_s( Error, "WSAESOCKTNOSUPPORT", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch
#endif
	}

	return s;
}


bool SocketAPI::bind( SOCKET s, const SocketAddress & addr )
{
	sockaddr_in in_addr;
	addr.Fill(in_addr);
	if ( ::bind ( s ,(sockaddr *) &in_addr , sizeof(in_addr) ) == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{
		case EADDRINUSE :
		case EINVAL : 
		case EACCES : 
		case ENOTSOCK : 
		case EBADF : 
		case EROFS : 
		case EFAULT : 
		case ENAMETOOLONG : 
		case ENOENT : 
		case ENOMEM : 
		case ENOTDIR : 
		case ELOOP : 
		default :
			{
				break;
			}
		}//end of switch
#elif _WIN32
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSAESOCKTNOSUPPORT", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEADDRINUSE : 
			strncpy_s( Error, "WSAEADDRINUSE", _ESIZE ) ;
			break ;
		case WSAEADDRNOTAVAIL : 
			strncpy_s( Error, "WSAEADDRNOTAVAIL", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy_s( Error, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch
#endif

		return false ;
	}
	
	return true ;
}

bool SocketAPI::connect( SOCKET s,const SocketAddress & addr )
{
	sockaddr_in in_addr;
	addr.Fill(in_addr);
	if ( ::connect(s,(sockaddr *)&in_addr,sizeof(in_addr)) == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) {
		case EALREADY : 
		case EINPROGRESS : 
		case ECONNREFUSED : 
		case EISCONN : 
		case ETIMEDOUT : 
		case ENETUNREACH : 
		case EADDRINUSE : 
		case EBADF : 
		case EFAULT : 
		case ENOTSOCK : 
		default :
			{
				break;
			}
		}//end of switch
#elif _WIN32
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEADDRINUSE : 
			strncpy_s( Error, "WSAEADDRINUSE", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy_s( Error, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEALREADY : 
			strncpy_s( Error, "WSAEALREADY", _ESIZE ) ;
			break ;
		case WSAEADDRNOTAVAIL : 
			strncpy_s( Error, "WSAEADDRNOTAVAIL", _ESIZE ) ;
			break ;
		case WSAEAFNOSUPPORT : 
			strncpy_s( Error, "WSAEAFNOSUPPORT", _ESIZE ) ;
			break ;
		case WSAECONNREFUSED : 
			strncpy_s( Error, "WSAECONNREFUSED", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEISCONN : 
			strncpy_s( Error, "WSAEISCONN", _ESIZE ) ;
			break ;
		case WSAENETUNREACH : 
			strncpy_s( Error, "WSAENETUNREACH", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy_s( Error, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAETIMEDOUT : 
			strncpy_s( Error, "WSAETIMEDOUT", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK  : 
			strncpy_s( Error, "WSAEWOULDBLOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch
#endif
		return false ;
	}

	return true ;
}

bool SocketAPI::listen( SOCKET s , int32 backlog ) 
{
	if ( ::listen( s , backlog ) == SOCKET_ERROR ) 
	{
#if __LINUX__
		switch ( errno ) 
		{
		case EBADF : 
		case ENOTSOCK :
		case EOPNOTSUPP :
		default :
			{
				break;
			}
		}//end of switch
#elif _WIN32
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEADDRINUSE : 
			strncpy_s( Error, "WSAEADDRINUSE", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEISCONN : 
			strncpy_s( Error, "WSAEISCONN", _ESIZE ) ;
			break ;
		case WSAEMFILE : 
			strncpy_s( Error, "WSAEMFILE", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy_s( Error, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy_s( Error, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch
#endif

		return false ;
	}

	return true ;
}

SOCKET SocketAPI::accept( SOCKET s, SocketAddress & addr )
{
	sockaddr_in inAddr;
	
#ifndef WIN32
	socklen_t addrLen(sizeof(inAddr));
	SOCKET client = ::accept( s , (sockaddr*)&inAddr , &addrLen );
#else 
	int addrLen(sizeof(inAddr));
	SOCKET client = ::accept( s , (sockaddr*)&inAddr , &addrLen );
#endif
	
	if ( client == INVALID_SOCKET ) 
	{
#ifndef WIN32
		switch ( errno ) 
		{

		case EWOULDBLOCK : 

		case ECONNRESET :
		case ECONNABORTED :
		case EPROTO :
		case EINTR :
			// from UNIX Network Programming 2nd, 15.6
			// with nonblocking-socket, ignore above errors

		case EBADF : 
		case ENOTSOCK : 
		case EOPNOTSUPP : 
		case EFAULT : 

		default :
			{
				break;
			}
		}//end of switch
#else
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy_s( Error, "WSAEINTRR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEMFILE : 
			strncpy_s( Error, "WSAEMFILE", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy_s( Error, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy_s( Error, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
			strncpy_s( Error, "WSAEWOULDBLOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch
#endif
	} 
	else 
	{
		addr = SocketAddress(inAddr);
	}

	return client;
}


bool SocketAPI::getsockopt( SOCKET s , int32 level , int32 optname , void * optval , int32 * optlen )
{
#ifndef WIN32
	if ( ::getsockopt( s , level , optname , optval , (socklen_t*)optlen ) == SOCKET_ERROR ) 
	{
		switch ( errno ) 
		{
		case EBADF : 
		case ENOTSOCK : 
		case ENOPROTOOPT : 
		case EFAULT : 
		default :
			{
				break;
			}
		}//end of switch

		return false ;
	}
#else
	if ( ::getsockopt( s , level , optname , (char*)optval , (int32*)optlen ) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENOPROTOOPT : 
			strncpy_s( Error, "WSAENOPROTOOPT", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch

		return false ;
	}
#endif

	return true ;
}

int32 SocketAPI::getsockopt_ex2( SOCKET s , int32 level , int32 optname , void * optval , int32 * optlen )
{
#ifndef WIN32
	if ( ::getsockopt( s , level , optname , optval , (socklen_t*)optlen ) == SOCKET_ERROR ) 
	{
		switch ( errno ) 
		{
		case EBADF : 
			return 1;
		case ENOTSOCK : 
			return 2;
		case ENOPROTOOPT : 
			return 3;
		case EFAULT : 
			return 4;
		default :
			return 5;
		}//end of switch
	}
	return 0;

#else
	if ( ::getsockopt( s , level , optname , (char*)optval , (int32*)optlen ) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED:
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN:
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT:
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS:
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL:
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENOPROTOOPT:
			strncpy_s( Error, "WSAENOPROTOOPT", _ESIZE ) ;
			break ;
		case WSAENOTSOCK:
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		}
	}
#endif

	return 0;
}

bool SocketAPI::setsockopt( SOCKET s , int32 level , int32 optname , const void * optval , int32 optlen )
{
#ifndef WIN32
	if ( ::setsockopt( s , level , optname , optval , optlen ) == SOCKET_ERROR ) 
	{
		switch ( errno ) 
		{
			case EBADF : 
			case ENOTSOCK : 
			case ENOPROTOOPT : 
			case EFAULT : 
			default :
				{
					break;
				}
		}//end of switch

		return false ;
	}
#else
	if ( ::setsockopt( s , level , optname , (char*)optval , optlen ) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAENETRESET : 
			strncpy_s( Error, "WSAENETRESET", _ESIZE ) ;
			break ;
		case WSAENOPROTOOPT : 
			strncpy_s( Error, "WSAENOPROTOOPT", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy_s( Error, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch

		return false ;
	}
#endif

	return true ;
}

int32 SocketAPI::send( SOCKET s , const uint8 * buf , int32 len , int32 flags )
{
	int32 nSent;
	{
#ifndef WIN32
	nSent = ::send(s,(const char *)buf,len,flags);
#else
	nSent = ::send(s,(const char *)buf,len,flags);
#endif

	if ( nSent == SOCKET_ERROR ) 
	{
#ifndef WIN32
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK;

		case ECONNRESET :
		case EPIPE :

		case EBADF : 
		case ENOTSOCK : 
		case EFAULT : 
		case EMSGSIZE : 
		case ENOBUFS : 

		default : 
			{
				break;
			}
		}//end of switch
#else
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEACCES : 
			strncpy_s( Error, "WSAEACCES", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy_s( Error, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAENETRESET : 
			strncpy_s( Error, "WSAENETRESET", _ESIZE ) ;
			break ;
		case WSAENOBUFS : 
			strncpy_s( Error, "WSAENOBUFS", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy_s( Error, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy_s( Error, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		case WSAESHUTDOWN : 
			strncpy_s( Error, "WSAESHUTDOWN", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
//			strncpy_s( Error, "WSAEWOULDBLOCK", _ESIZE ) ;
			return SOCKET_ERROR_WOULDBLOCK ;
			break ;
		case WSAEMSGSIZE : 
			strncpy_s( Error, "WSAEMSGSIZE", _ESIZE ) ;
			break ;
		case WSAEHOSTUNREACH : 
			strncpy_s( Error, "WSAEHOSTUNREACH", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAECONNABORTED : 
			strncpy_s( Error, "WSAECONNABORTED", _ESIZE ) ;
			break ;
		case WSAECONNRESET : 
			strncpy_s( Error, "WSAECONNRESET", _ESIZE ) ;
			break ;
		case WSAETIMEDOUT : 
			strncpy_s( Error, "WSAETIMEDOUT", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch
#endif
	} 
	else if ( nSent == 0 )
	{
	}

	}
	return nSent;

}


int32 SocketAPI::sendto( SOCKET s , const uint8 * buf , int32 len , int32 flags , const struct sockaddr * to , int32 tolen )
{
#ifndef WIN32
	int32 nSent = ::sendto(s,(const char *)buf,len,flags,to,tolen);
#else
	int32 nSent = ::sendto(s,(const char *)buf,len,flags,to,tolen);
#endif

	if ( nSent == SOCKET_ERROR ) 
	{
#ifndef WIN32
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return 0;

		case ECONNRESET :
		case EPIPE :

		case EBADF : 
		case ENOTSOCK : 
		case EFAULT : 
		case EMSGSIZE : 
		case ENOBUFS : 

		default : 
			{
				break;
			}
		}	
#else
#endif
	}

	return nSent;
}

int32 SocketAPI::recv( SOCKET s , uint8 * buf , int32 len , int32 flags )
{
#ifndef WIN32
	int32 nrecv = ::recv(s,buf,len,flags);
#else
	int32 nrecv = ::recv(s,(char*)buf,len,flags);
#endif

	if ( nrecv == SOCKET_ERROR ) 
	{
#ifndef WIN32
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK;

		case ECONNRESET :
		case EPIPE :

		case EBADF : 
		case ENOTCONN : 
		case ENOTSOCK : 
		case EINTR : 
		case EFAULT : 

		default : 
			{
				break;
			}
		}//end of switch

#else
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy_s( Error, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy_s( Error, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENETRESET : 
			strncpy_s( Error, "WSAENETRESET", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEOPNOTSUPP : 
			strncpy_s( Error, "WSAEOPNOTSUPP", _ESIZE ) ;
			break ;
		case WSAESHUTDOWN : 
			strncpy_s( Error, "WSAESHUTDOWN", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
//			strncpy_s( Error, "WSAEWOULDBLOCK", _ESIZE ) ;
			return SOCKET_ERROR_WOULDBLOCK ;
			break ;
		case WSAEMSGSIZE : 
			strncpy_s( Error, "WSAEMSGSIZE", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAECONNABORTED : 
			strncpy_s( Error, "WSAECONNABORTED", _ESIZE ) ;
			break ;
		case WSAETIMEDOUT : 
			strncpy_s( Error, "WSAETIMEDOUT", _ESIZE ) ;
			break ;
		case WSAECONNRESET : 
			strncpy_s( Error, "WSAECONNRESET", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch
#endif
	} 
	else if ( nrecv == 0 )
	{
	}

	return nrecv;
}


int32 SocketAPI::recvfrom( SOCKET s , uint8 * buf , int32 len , int32 flags , struct sockaddr * from , int32 * fromlen )
{
#ifndef WIN32
	int32 nReceived = ::recvfrom(s,buf,len,flags,from,(socklen_t*)fromlen);

#else
	int32 nReceived = ::recvfrom(s,(char*)buf,len,flags,from,(int32*)fromlen);
#endif

	if ( nReceived == SOCKET_ERROR ) 
	{
#ifndef WIN32
		switch ( errno ) 
		{

		case EWOULDBLOCK : 
			return SOCKET_ERROR_WOULDBLOCK;

		case ECONNRESET :
		case EPIPE :

		case EBADF : 
		case ENOTCONN : 
		case ENOTSOCK : 
		case EINTR : 
		case EFAULT : 

		default : 
			{
				break;
			}
		}//end of switch
#else
#endif
	}

	return nReceived;
}


bool SocketAPI::closesocket( SOCKET s )
{
#ifndef WIN32
	::close(s);
#else
	if ( ::closesocket(s) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAEINTR : 
			strncpy_s( Error, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEWOULDBLOCK : 
			strncpy_s( Error, "WSAEWOULDBLOCK", _ESIZE ) ;
			break ;
		default : 
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};//end of switch

		return false ;
	}
#endif

	return true ;
}


bool SocketAPI::ioctlsocket( SOCKET s , long cmd , unsigned long * argp )
{
#ifndef WIN32
#else
	if ( ::ioctlsocket(s,cmd,argp) == SOCKET_ERROR ) 
	{
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN : 
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		case WSAEFAULT : 
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};

		return false ;
	}
#endif

	return true ;
}
 


bool SocketAPI::getsocketnonblocking( SOCKET s )
{
// #ifndef WIN32
// 	int32 flags = FileAPI::fcntl_ex( s , F_GETFL , 0 );
// 
// 	return flags | O_NONBLOCK;
// #else
// 	return false ;
// #endif
	return false ;
}

bool SocketAPI::setsocketnonblocking( SOCKET s , bool on )
{
// #ifndef WIN32
// 	int32 flags = FileAPI::fcntl_ex( s , F_GETFL , 0 );
// 
// 	if ( on )
// 		// make nonblocking fd
// 		flags |= O_NONBLOCK;
// 	else
// 		// make blocking fd
// 		flags &= ~O_NONBLOCK;
// 
// 	FileAPI::fcntl_ex( s , F_SETFL , flags );
// 
// 	return true;
// #else
// 
// 	ULONG argp = ( on == true ) ? 1 : 0;
// 	return ioctlsocket( s,FIONBIO,&argp);
// #endif
	return false ;
}

int32 SocketAPI::availablesocket( SOCKET s )
{
// #ifndef WIN32
// 		return FileAPI::availablefile_ex(s);
// #else
// 	ULONG argp = 0;
// 	ioctlsocket(s,FIONREAD,&argp);
// 	return argp;
// #endif
	return 0 ;
}

bool SocketAPI::shutdown( SOCKET s , int32 how )
{
	if ( ::shutdown(s,how) < 0 ) 
	{
#ifndef WIN32
		switch ( errno ) {
		case EBADF : 
		case ENOTSOCK : 
		case ENOTCONN : 
		default : 
			{
				break;
			}
		}
#else
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAENETDOWN :
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEINVAL : 
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS : 
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENOTCONN : 
			strncpy_s( Error, "WSAENOTCONN", _ESIZE ) ;
			break ;
		case WSAENOTSOCK : 
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif

		return false ;
	}

	return true ;
}

int32 SocketAPI::select( int32 maxfdp1 , fd_set * readset , fd_set * writeset , fd_set * exceptset , struct timeval * timeout )
{

	int32 result;
	{
		result = ::select( maxfdp1 , readset , writeset , exceptset , timeout );
		if( result == SOCKET_ERROR )
		{
#ifndef WIN32

#else
		int32 iErr = WSAGetLastError() ;
		switch ( iErr ) 
		{
		case WSANOTINITIALISED : 
			strncpy_s( Error, "WSANOTINITIALISED", _ESIZE ) ;
			break ;
		case WSAEFAULT:
			strncpy_s( Error, "WSAEFAULT", _ESIZE ) ;
			break ;
		case WSAENETDOWN:
			strncpy_s( Error, "WSAENETDOWN", _ESIZE ) ;
			break ;
		case WSAEINVAL:
			strncpy_s( Error, "WSAEINVAL", _ESIZE ) ;
			break ;
		case WSAEINTR:
			strncpy_s( Error, "WSAEINTR", _ESIZE ) ;
			break ;
		case WSAEINPROGRESS:
			strncpy_s( Error, "WSAEINPROGRESS", _ESIZE ) ;
			break ;
		case WSAENOTSOCK:
			strncpy_s( Error, "WSAENOTSOCK", _ESIZE ) ;
			break ;
		default :
			{
			strncpy_s( Error, "UNKNOWN", _ESIZE ) ;
			break ;
			};
		};
#endif
		}//end if
	}
	return result;

}




