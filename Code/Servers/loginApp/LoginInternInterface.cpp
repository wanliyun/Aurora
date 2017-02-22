
#include "pch.h"
#include "LoginInternInterface.h"
#include <binary_stream.h>

using namespace std;

namespace Aurora
{
	namespace LoginInternInterface
	{
		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_LoginIntern);
			ADD_PACKET_PRODUCT(LoginInternInterface::EPacketIndex_WhoAmIResponse			,LoginInternInterface::PktWhoAmIResponse			);
			ADD_PACKET_PRODUCT(LoginInternInterface::EPacketIndex_LoginResponse				,LoginInternInterface::PktLoginResponse				);
		END_IMPLEMENT_PACKET_FACTORY();

		void PktWhoAmIResponse::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_bSucc<<m_id;
		}

		void PktWhoAmIResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_bSucc>>m_id;
		}

		void PktLoginResponse::WriteToStream( BinaryOStream & oStream )const
		{
			oStream<<m_clntAppID<<m_code<<m_baseAppIp<<m_baseAppPort;
		}

		void PktLoginResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_clntAppID>>m_code>>m_baseAppIp>>m_baseAppPort;
		}
	}

}
