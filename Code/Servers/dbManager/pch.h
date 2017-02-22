
#ifndef INCLUDING_CPP_INTERFACE

#ifndef _SERVER_COMMON_PCH_H
#define _SERVER_COMMON_PCH_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <WinSock2.h>
#include <conio.h>
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
#define _getch getchar
#endif


#include <set>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <Utils.h>
#include <Debug.h>
#include <Types.h>
#include <MD5.h>
#include <Constants.h>
#include <XMLWarpper.h>
#include <pyscript/PyImportPaths.h>
#include <pyscript/PyScript.h>
#include <UniqueIDPool.h>

#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>

namespace bpy = boost::python;

#ifndef WIN32
#include <algorithm>
using std::min;
using std::max;
#endif

#endif

#endif //INCLUDING_CPP_INTERFACE
