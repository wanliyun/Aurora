#ifndef _SERVER_SPACE_H_
#define _SERVER_SPACE_H_

namespace Aurora
{
	class BinaryIStream;
	class BinaryOStream;

	struct IPrivateData
	{
		virtual ~IPrivateData(){}
	};

	struct SpaceUnitInfoBase
	{
		SpaceUnitInfoBase():m_privateData(0){}
		~SpaceUnitInfoBase(){SAFE_DELETE(m_privateData);}
		IPrivateData * m_privateData;//拥有这个指针;
	};

	struct SCellInfo : public SpaceUnitInfoBase
	{
		AppID m_cellAppId;

		int32 m_nMinX;
		int32 m_nMinY;
		int32 m_nMaxX;
		int32 m_nMaxY;

		std::vector<CellID> m_vNeighbourCellIDs;

		void WriteToStream(BinaryOStream & os)const;
		bool ReadFromStream(BinaryIStream & is);
	};

	struct SChannelInfo : public SpaceUnitInfoBase
	{
		typedef std::map<CellID,SCellInfo>  MapCellInfoT;
		MapCellInfoT m_mapCells;

		void WriteToStream(BinaryOStream & os)const;
		bool ReadFromStream(BinaryIStream & is);

		const SCellInfo * GetCellInfo( CellID cellId )const;
	};

	struct SSpaceInfo : public SpaceUnitInfoBase
	{
		typedef std::map<SpaceChannelID,SChannelInfo> MapChannelT;
		MapChannelT m_mapChannels;
		std::string m_strName;
		uint16 m_nMaxChannels;
		uint16 m_nMaxCells;

		bool m_bIsMain;

		SSpaceInfo():m_nMaxChannels(0),m_nMaxCells(0){}
		SSpaceInfo & operator = (const SSpaceInfo & other){copyfrom(other);}
		SSpaceInfo(const SSpaceInfo & other){copyfrom(other);}
		void WriteToStream(BinaryOStream & os)const;
		bool ReadFromStream(BinaryIStream & is);

		const SCellInfo * GetCellInfo(SpaceChannelID cid,CellID cellId)const;

	private:
		void copyfrom(const SSpaceInfo & other)
		{
			m_strName = other.m_strName;
			m_nMaxChannels = other.m_nMaxChannels;
			m_nMaxCells = other.m_nMaxCells;
			m_bIsMain = other.m_bIsMain;
		}
	};

	struct SAllServerSpaceInfo
	{
		SAllServerSpaceInfo();
		~SAllServerSpaceInfo();

		const SCellInfo * GetCellInfo(const SFullCellID & fid)const{return GetCellInfo(fid.m_spaceId,fid.m_channelId,fid.m_cellId);}
		const SCellInfo * GetCellInfo(SpaceID sid,SpaceChannelID cid,CellID cellId)const;

		void WriteToStream(BinaryOStream & os)const;
		bool ReadFromStream(BinaryIStream & is);

		typedef std::map<SpaceID,SSpaceInfo> MapSpaceT;

		MapSpaceT m_mapSpaces;
	};
}

#endif
