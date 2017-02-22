
#ifndef __MY_SOCKET_API_H__
#define __MY_SOCKET_API_H__

// include files
#include "Types.h"
#include <string.h>

#define _ESIZE 256


#ifndef WIN32
typedef		int32		SOCKET;
#define     INVALID_SOCKET   -1
#define		SOCKET_ERROR	 -1
#endif

const static int SOCKET_ERROR_WOULDBLOCK = -100;

struct SocketAddress
{
	SocketAddress();
	SocketAddress(const char * host,uint16 port,uint16 salt = 0);
	SocketAddress(const std::string & host,uint16 port,uint16 salt = 0);
	SocketAddress(uint32 ip,uint16 port,uint16 salt = 0);
	explicit SocketAddress(const sockaddr_in & addr,uint16 salt = 0);
	~SocketAddress(){}

	uint32 GetIP()const{return uint32(m_addr>>32);}
	uint16 GetPort()const;
	uint16 GetSalt()const{return uint16(m_addr>>16);}

	void Fill(sockaddr_in & addr)const;
	std::string ToString()const;
private:
	uint64 m_addr;
};

namespace SocketAPI 
{
	SOCKET socket(int32 domain, int32 type, int32 protocol) ;
	bool bind(SOCKET s, const SocketAddress & addr ) ;

	bool connect(SOCKET s,const SocketAddress & addr ) ;

	bool listen(SOCKET s, int32 backlog) ;

	SOCKET accept(SOCKET s,  SocketAddress & addr ) ;

	bool getsockopt(SOCKET s, int32 level, int32 optname, void* optval, int32* optlen) ;

	int32 getsockopt_ex2 (SOCKET s, int32 level, int32 optname, void* optval, int32* optlen) ;

	bool setsockopt(SOCKET s, int32 level, int32 optname, const void* optval, int32 optlen) ;

	int32 send(SOCKET s, const uint8* buf, int32 len, int32 flags) ;

	int32 sendto(SOCKET s, const uint8* buf, int32 len, int32 flags, const struct sockaddr* to, int32 tolen) ;

	int32 recv(SOCKET s, uint8* buf, int32 len, int32 flags) ;

	int32 recvfrom(SOCKET s, uint8* buf, int32 len, int32 flags, struct sockaddr* from, int32* fromlen) ;
	 
	bool closesocket(SOCKET s) ;

	bool ioctlsocket(SOCKET s, long cmd, unsigned long* argp) ;

	bool getsocketnonblocking(SOCKET s) ;

	bool setsocketnonblocking(SOCKET s, bool on) ;

	int32 availablesocket(SOCKET s) ;

	bool shutdown(SOCKET s, int32 how) ;

	int32 select(int32 maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout) ;


};//end of namespace 


#endif
