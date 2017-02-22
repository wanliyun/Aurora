#ifndef _SPACE_MANAGER_H_
#define _SPACE_MANAGER_H_

#include <boost/serialization/singleton.hpp>
#include <ServerSpace.h>

namespace Aurora
{
	class XMLElementWrapper;
	class SpaceManager : public boost::serialization::singleton<SpaceManager>
	{
	public:
		SpaceManager();
		~SpaceManager();
		SAllServerSpaceInfo & GetAllSpaceInfo(){return m_spaceInfo;} 
		bool Init();
		void OnTick(float fDelta);
	private:
		void init_space(XMLElementWrapper & ele);
	private:
		SAllServerSpaceInfo m_spaceInfo;
	};
}

#endif
