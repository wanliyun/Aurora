#include "pch.h"
#include "CellManager.h"
#include <memory_stream.h>
#include "Cell.h"
#include "CellApp.h"

namespace Aurora
{
	void CellManager::OnRecvPacket( const PacketBase & pkt )
	{
		BEGINE_HANDLE_PACKET(EPacketInterface_Cell);
		HADNLE_PACKET_(CellInterface::EPacketIndex_SyncAllSpaceInfo, CellInterface::PktSyncAllSpaceInfo);
		END_HANDLE_PACKET();
	}

	void CellManager::HandlePacket( const CellInterface::PktSyncAllSpaceInfo & pkt )
	{
		LogF("PktSyncAllSpaceInfo\n");
		MemoryIStream is(pkt.GetBuff(),pkt.GetBuffLen());
		m_spaceInfo.ReadFromStream(is);

		//删掉不属于自己管理的Cell;
		{
			std::vector<SFullCellID> toDel;
			for(MapCellsT::iterator iter = m_mapCells.begin();iter != m_mapCells.end();++iter)
			{
				if(!m_spaceInfo.GetCellInfo(iter->first))
					toDel.push_back(iter->first);
			}
			for(std::vector<SFullCellID>::const_iterator iter = toDel.begin();iter != toDel.end();++iter)
			{
				SAFE_DELETE(m_mapCells[ *iter ]);
				m_mapCells.erase( *iter );
			}
		}
		
		//添加自己管理的Cell;
		{
			for(SAllServerSpaceInfo::MapSpaceT::const_iterator iter_space = m_spaceInfo.m_mapSpaces.begin();iter_space != m_spaceInfo.m_mapSpaces.end();++iter_space)
			{
				SpaceID sid = iter_space->first;
				const SSpaceInfo & space_info = iter_space->second;
				for(SSpaceInfo::MapChannelT::const_iterator iter_channel = space_info.m_mapChannels.begin();iter_channel != space_info.m_mapChannels.end();++iter_channel)
				{
					SpaceChannelID channelId = iter_channel->first;
					const SChannelInfo & channel_info = iter_channel->second;
					for(SChannelInfo::MapCellInfoT::const_iterator iter_cell = channel_info.m_mapCells.begin();iter_cell != channel_info.m_mapCells.end();++iter_cell)
					{
						CellID cellId = iter_cell->first;
						const SCellInfo & cell_info = iter_cell->second;

						if(cell_info.m_cellAppId != CellApp::get_mutable_instance().GetAppID())
							continue;
						SFullCellID fullCellID(sid,channelId,cellId);
						if(m_mapCells.find(fullCellID) != m_mapCells.end())
							continue;

						m_mapCells.insert( std::make_pair(fullCellID, new Cell(fullCellID)));
					}
				}
			}
		}
	}

	Cell * CellManager::GetCell( SFullCellID fullCellID )
	{
		std::map<SFullCellID,Cell * >::iterator iter = m_mapCells.find(fullCellID);
		if(iter == m_mapCells.end())
			return 0;
		return iter->second;
	}

	void CellManager::OnTick( float fDeltaTime )
	{
		for(MapCellsT::iterator iter = m_mapCells.begin();iter != m_mapCells.end();++iter)
		{
			MY_ASSERT(iter->second);
			iter->second->OnTick(fDeltaTime);
		}
	}

}