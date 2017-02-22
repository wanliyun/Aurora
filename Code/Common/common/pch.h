

#ifndef _COMMON_STDAFX_H
#define _COMMON_STDAFX_H
#ifndef WIN32
#include <algorithm>
using std::min;
using std::max;
#endif
#ifdef WIN32
#include <WinSock2.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>			// for accept()
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>			// for inet_xxx()
#include <netinet/in.h>
#include <errno.h>				// for errno
#include <termios.h>  
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <set>
#include <vector>
#include <map>
#include <list>
#include <string>
#include "Utils.h"
#include "Debug.h"
#include "Types.h"

#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>

namespace bpy = boost::python;



#endif
