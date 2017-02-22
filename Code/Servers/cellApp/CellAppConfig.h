#ifndef _CELL_APP_CONFIG_H_
#define _CELL_APP_CONFIG_H_

#include "Types.h"

#include <ServerAppConfigBase.h>
#include <boost/serialization/singleton.hpp>


namespace Aurora
{
	class CellAppConfig : public ServerAppConfigBase ,public boost::serialization::singleton<CellAppConfig>
	{
	public:
		CellAppConfig();
		~CellAppConfig();

		std::string m_appMgrAddr;
		uint16		m_appMgrPort;
	private:
		virtual bool OnInit(const XMLElementWrapper & xmlEle);
	};
}

#endif
