#ifndef _CELL_APP_CELL_MANAGER_H
#define _CELL_APP_CELL_MANAGER_H

#include <boost/serialization/singleton.hpp>
#include "ServerSpace.h"
#include "CellInterface.h"

namespace Aurora
{
	class Cell;
	class PacketBase;
	class CellManager : public boost::serialization::singleton<CellManager>
	{
	public:
		CellManager(){}
		~CellManager(){}

		void OnTick(float fDeltaTime);
		SAllServerSpaceInfo &  GetAllSpaceInfo(){return m_spaceInfo;}

		void OnRecvPacket( const PacketBase & pkt );

		Cell * GetCell(SFullCellID fullCellID);

		size_t GetNumCells()const{return m_mapCells.size();}
	private:
		void HandlePacket(const CellInterface::PktSyncAllSpaceInfo & pkt);
	private:
		SAllServerSpaceInfo m_spaceInfo;

		typedef std::map<SFullCellID,Cell * > MapCellsT;	//”µ”–÷∏’Î;
		MapCellsT m_mapCells;
	};
}

#endif
