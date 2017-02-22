#include "pch.h"
#include "PacketBase.h"
#include "binary_stream.h"

namespace Aurora
{
	void SPacketHeader::WriteToStream( BinaryOStream & data )const
	{
		data<<m_nPacketTypeID;
	}

	void SPacketHeader::ReadFromStream( BinaryIStream & data )
	{
		data>>m_nPacketTypeID;
	}

	PacketFactoryManager::~PacketFactoryManager()
	{

	}

	void PacketFactoryManager::RegisterFactory( EPacketInterface eInterface,IPacketFactory * pFactory )
	{
		MY_ASSERT(m_mapFactories.find(eInterface) == m_mapFactories.end());
		MY_ASSERT(pFactory);
		m_mapFactories.insert( std::make_pair(eInterface,pFactory));
	}

	void PacketFactoryManager::DeregisterFactory( IPacketFactory * pFactory )
	{
		MY_ASSERT(pFactory);

		EPacketInterface eInt;
		bool bFind = false;
		for(std::map<EPacketInterface,IPacketFactory * >::const_iterator iter = m_mapFactories.begin();iter != m_mapFactories.end();++iter)
		{
			if(iter->second == pFactory)
			{
				bFind = true;
				eInt = iter->first;
				break;
			}
		}
		if(bFind)
			m_mapFactories.erase(eInt);
	}

	PacketBase * PacketFactoryManager::NewPacket( uint16 typeID )
	{
		EPacketInterface eInt = EPacketInterface(typeID >> 12);
		if(m_mapFactories.find(eInt) == m_mapFactories.end())
			return 0;

		uint16 idx( typeID & 0xfff);
		return m_mapFactories[eInt]->NewPacket(idx);
	}

	void PacketFactoryManager::DeletePacket( PacketBase * pPacket )
	{
		EPacketInterface eInt = EPacketInterface(pPacket->GetHeader().GetInterface());
		MY_ASSERT(m_mapFactories.find(eInt) != m_mapFactories.end());
		m_mapFactories[eInt]->DeletePacket(pPacket);
	}

	void PacketBase::AppendToBuff(const char *pData,uint32 dataLen )
	{
		MY_ASSERT(pData);
		_____buff.append(pData,dataLen);
	}

	void PacketBase::WriteBuffToStream( BinaryOStream & oStream ) const
	{
		oStream<<_____buff;
	}

	void PacketBase::ReadBuffFromStream( BinaryIStream & iStream )
	{
		iStream>>_____buff;
	}

}
