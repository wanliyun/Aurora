#include "pch.h"
#include "ServerSpace.h"
#include "binary_stream.h"

namespace Aurora
{
	SAllServerSpaceInfo::SAllServerSpaceInfo()
	{

	}

	SAllServerSpaceInfo::~SAllServerSpaceInfo()
	{

	}

	void SAllServerSpaceInfo::WriteToStream( BinaryOStream & os ) const
	{
		uint16 nSpaces = uint16(m_mapSpaces.size());
		os<<nSpaces;
		for(MapSpaceT::const_iterator iter_space = m_mapSpaces.begin();iter_space != m_mapSpaces.end();++iter_space)
		{
			//Ð´ÈëSpaceID;
			os<<iter_space->first;

			iter_space->second.WriteToStream(os);
		}
	}

	bool SAllServerSpaceInfo::ReadFromStream( BinaryIStream & is )
	{
		m_mapSpaces.clear();
		uint16 nSpaces;
		is>>nSpaces;
		for(uint16 spaceIdx = 0 ;spaceIdx < nSpaces;++spaceIdx)
		{
			SpaceID sid;
			is>>sid;
			if(m_mapSpaces.find(sid) != m_mapSpaces.end())
			{
				Log_ErrorF("duplex sid = %u\n",sid);
				return false;
			}
			m_mapSpaces.insert(std::make_pair(sid,SSpaceInfo()));
			m_mapSpaces[sid].ReadFromStream(is);
		}
		return !is.error();
	}

	const SCellInfo * SAllServerSpaceInfo::GetCellInfo( SpaceID sid,SpaceChannelID cid,CellID cellId )const
	{
		MapSpaceT::const_iterator iter = m_mapSpaces.find(sid);
		if(iter == m_mapSpaces.end())
			return 0;
		return iter->second.GetCellInfo(cid,cellId);
	}

	void SCellInfo::WriteToStream( BinaryOStream & os ) const
	{
		os<<m_cellAppId<<m_nMinX<<m_nMinY<<m_nMaxX<<m_nMaxY;
		os<<uint16(m_vNeighbourCellIDs.size());
		for(std::vector<CellID>::const_iterator iter = m_vNeighbourCellIDs.begin();iter != m_vNeighbourCellIDs.end();++iter)
		{
			os<< (*iter);
		}
	}

	bool SCellInfo::ReadFromStream( BinaryIStream & is )
	{
		is>>m_cellAppId>>m_nMinX>>m_nMinY>>m_nMaxX>>m_nMaxY;
		uint16 nNeighbourCells;
		is>>nNeighbourCells;
		m_vNeighbourCellIDs.clear();
		for(uint16 i = 0;i<nNeighbourCells;++i)
		{
			CellID cid;
			is>>cid;
			m_vNeighbourCellIDs.push_back(cid);
		}
		return !is.error();
	}


	void SChannelInfo::WriteToStream( BinaryOStream & os ) const
	{
		os<<uint16(m_mapCells.size());
		for(MapCellInfoT::const_iterator iter = m_mapCells.begin();iter!= m_mapCells.end();++iter)
		{
			os<<iter->first;
			iter->second.WriteToStream(os);
		}
	}

	bool SChannelInfo::ReadFromStream( BinaryIStream & is )
	{
		m_mapCells.clear();
		uint16 nCells;
		is>>nCells;

		for(uint16 i =0;i<nCells;++i)
		{
			CellID cid;
			is>>cid;
			m_mapCells.insert(std::make_pair(cid,SCellInfo()));
			m_mapCells[cid].ReadFromStream(is);
		}
		return true;
	}

	const SCellInfo * SChannelInfo::GetCellInfo( CellID cellId ) const
	{
		MapCellInfoT::const_iterator iter = m_mapCells.find(cellId);
		if(iter == m_mapCells.end())
			return 0;
		return &(iter->second);
	}


	void SSpaceInfo::WriteToStream( BinaryOStream & os ) const
	{
		os<<uint16(m_mapChannels.size());
		for(MapChannelT::const_iterator iter = m_mapChannels.begin();iter!= m_mapChannels.end();++iter)
		{
			os<<iter->first;
			iter->second.WriteToStream(os);
		}
		os<<m_strName<<m_nMaxChannels<<m_nMaxCells<<m_bIsMain;
	}

	bool SSpaceInfo::ReadFromStream( BinaryIStream & is )
	{
		m_mapChannels.clear();
		uint16 nChannels;
		is>>nChannels;

		for(uint16 i =0;i<nChannels;++i)
		{
			CellID cid;
			is>>cid;
			m_mapChannels.insert(std::make_pair(cid,SChannelInfo()));
			m_mapChannels[cid].ReadFromStream(is);
		}
		is>>m_strName>>m_nMaxChannels>>m_nMaxCells>>m_bIsMain;
		return !is.error();
	}

	const SCellInfo * SSpaceInfo::GetCellInfo( SpaceChannelID cid,CellID cellId ) const
	{
		MapChannelT::const_iterator iter = m_mapChannels.find(cid);
		if(iter == m_mapChannels.end())
			return 0;
		return iter->second.GetCellInfo(cellId);
	}

}
