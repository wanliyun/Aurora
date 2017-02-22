#ifndef _UNIQUE_ID_POOL_H_
#define _UNIQUE_ID_POOL_H_

#include "interface/INoneCopyable.h"

namespace Aurora
{
	template<typename STD_Container,typename IDType>
	class UniqueIDPool : public INoneCopyable
	{
	public:
		explicit UniqueIDPool(STD_Container & container,IDType startID,IDType stopID,IDType badID):
			m_container(container),
			m_nCurrentID(startID),
			m_nStopID(stopID),
			m_nBadID(badID)
		{

		}
		bool GenerateUniqueID(IDType &nId)
		{
			const IDType nStart = m_nCurrentID;

			if(m_nCurrentID == m_nStopID)
				m_nCurrentID = nStart;

			while(m_nCurrentID == m_nBadID || m_container.find(m_nCurrentID) != m_container.end())
			{
				++m_nCurrentID;
				
				if(m_nCurrentID == m_nStopID)
					m_nCurrentID = nStart;

				if(nStart == m_nCurrentID)
					return false;
			}
			nId = m_nCurrentID;
			++m_nCurrentID;
			return true;
		}
	private:
		STD_Container & m_container;
		const IDType m_nStopID;
		const IDType m_nBadID;
		IDType m_nCurrentID;
		std::map<IDType,class LoginApp_Client * >	m_mapClints;
	};
}

#endif 