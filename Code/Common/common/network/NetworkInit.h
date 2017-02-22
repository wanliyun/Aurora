

#ifndef __NETWORK_INIT_H__
#define __NETWORK_INIT_H__

#include "Types.h"
#include <boost/serialization/singleton.hpp>
namespace Aurora
{
	class NetworkInit  : public boost::serialization::singleton<NetworkInit>
	{
	public :
		NetworkInit();
		~NetworkInit();
	};
}

#endif
