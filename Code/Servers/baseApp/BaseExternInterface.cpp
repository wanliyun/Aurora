
#include "pch.h"
#include "BaseExternInterface.h"
#include <binary_stream.h>

using namespace std;

namespace Aurora
{
	namespace BaseExternInterface
	{
		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_BaseExtern);
		ADD_PACKET_PRODUCT(BaseExternInterface::EPacketIndex_WhoAmI				,BaseExternInterface::PktWhoAmI				);
		ADD_PACKET_PRODUCT(BaseExternInterface::EPacketIndex_RedirectToCell		,BaseExternInterface::PktRedirectToCell		);
		ADD_PACKET_PRODUCT(BaseExternInterface::EPacketIndex_Echo				,BaseExternInterface::PktEcho				);
		ADD_PACKET_PRODUCT(BaseExternInterface::EPacketIndex_BroadCast			,BaseExternInterface::PktBroadCast			);
		ADD_PACKET_PRODUCT(BaseExternInterface::EPacketIndex_MethodCallFromClient,BaseExternInterface::PktMethodCallFromClient	);
		ADD_PACKET_PRODUCT(BaseExternInterface::EPacketIndex_SyncPosRot			,BaseExternInterface::PktSyncPosRot	);
		END_IMPLEMENT_PACKET_FACTORY();

		void PktWhoAmI::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_strAccountName;
		}

		void PktWhoAmI::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_strAccountName;
		}
		void PktRedirectToCell::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_nEntityID;
			WriteBuffToStream(oStream);
		}

		void PktRedirectToCell::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_nEntityID;
			ReadBuffFromStream(iStream);
		}

		void PktEcho::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_buff;
		}

		void PktEcho::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_buff;
		}


		void PktBroadCast::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_buff;
		}

		void PktBroadCast::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_buff;
		}

		void PktMethodCallFromClient::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eID<<m_mID;
			WriteBuffToStream(oStream);
		}

		void PktMethodCallFromClient::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eID>>m_mID;
			ReadBuffFromStream(iStream);
		}


		void PktSyncPosRot::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eID<<pos_x<<pos_y<<pos_z<<rot_y<<rot_p<<rot_r;
		}

		void PktSyncPosRot::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eID>>pos_x>>pos_y>>pos_z>>rot_y>>rot_p>>rot_r;
		}
	}
}
