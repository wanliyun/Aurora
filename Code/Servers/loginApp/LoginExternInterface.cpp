
#include "pch.h"
#include "LoginExternInterface.h"
#include <binary_stream.h>

using namespace std;

namespace Aurora
{
	namespace LoginExternInterface
	{
		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_LoginExtern);
			ADD_PACKET_PRODUCT(LoginExternInterface::EPacketIndex_LoginRquest				,LoginExternInterface::PktLoginRquest				);
		END_IMPLEMENT_PACKET_FACTORY();


		LoginExternPacketBase::LoginExternPacketBase(EPacketIndex idx ):PacketBase(EPacketInterface_LoginExtern,idx)
		{

		}

		void PktLoginRquest::WriteToStream( BinaryOStream & oStream )const
		{
			oStream<<m_strUserName<<m_strPasssWord<<m_strBlowfishKey;
		}

		void PktLoginRquest::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_strUserName>>m_strPasssWord>>m_strBlowfishKey;
		}
	}
}
