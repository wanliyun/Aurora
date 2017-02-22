
#include "pch.h"
#include "DBManagerInterface.h"
#include <binary_stream.h>

using namespace std;

namespace Aurora
{
	namespace DBManagerInterface
	{
		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_DBManager);
			ADD_PACKET_PRODUCT(DBManagerInterface::EPacketIndex_WhoAmIResponse		,DBManagerInterface::PktWhoAmIResponse				);
			ADD_PACKET_PRODUCT(DBManagerInterface::EPacketIndex_WhoAmI				,DBManagerInterface::PktWhoAmI						);
			ADD_PACKET_PRODUCT(DBManagerInterface::EPacketIndex_Test				,DBManagerInterface::PktTest						);
		END_IMPLEMENT_PACKET_FACTORY();

		void PktWhoAmIResponse::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_bSucc<<m_id;
		}

		void PktWhoAmIResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_bSucc>>m_id;
		}

		void PktWhoAmI::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_baseAppId;
		}

		void PktWhoAmI::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_baseAppId;
		}


		void PktTest::WriteToStream( BinaryOStream & oStream ) const
		{

		}

		void PktTest::ReadFromStream( BinaryIStream & iStream )
		{

		}

	}

}
