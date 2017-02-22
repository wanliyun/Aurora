
#include "pch.h"
#include "BaseInternInterface.h"
#include <binary_stream.h>
#include <memory_stream.h>
using namespace std;

namespace Aurora
{
	namespace BaseInternInterface
	{
		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_BaseIntern);

		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_WhoAmIResponse	,BaseInternInterface::PktWhoAmIResponse	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_SyncAllSpaceInfo,BaseInternInterface::PktSyncAllSpaceInfo	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_NewCellApp	,BaseInternInterface::PktNewCellApp	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_CreateAccountRquest	,BaseInternInterface::PktCreateAccountRquest	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_BroadCast	,BaseInternInterface::PktBroadcastMsg	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_RedirectToClient	,BaseInternInterface::PktRedirectToClient	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_RedirectBoradcastToClient	,BaseInternInterface::PktRedirectBroadcastToClient	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_MethodCallFromCell	,BaseInternInterface::PktMethodCallFromCell	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_OneAttributeChangeByCell	,BaseInternInterface::PktOneAttributeChangeByCell	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_SyncPosRot	,BaseInternInterface::PktSyncPosRot	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_NewBaseApp	,BaseInternInterface::PktNewBaseApp	);
		ADD_PACKET_PRODUCT(BaseInternInterface::EPacketIndex_BaseApp_WhoAmI	,BaseInternInterface::Pkt_BaseApp_WhoAmI	);

		END_IMPLEMENT_PACKET_FACTORY();

		void PktWhoAmIResponse::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_bSucc<<m_id;
			WriteBuffToStream(oStream);
		}

		void PktWhoAmIResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_bSucc>>m_id;
			ReadBuffFromStream(iStream);
		}


		void PktSyncAllSpaceInfo::WriteToStream( BinaryOStream & oStream ) const
		{
			WriteBuffToStream(oStream);
		}

		void PktSyncAllSpaceInfo::ReadFromStream( BinaryIStream & iStream )
		{
			ReadBuffFromStream(iStream);
		}

		void PktCreateAccountRquest::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_strAccountName<<m_strBlowfishKey;
		}

		void PktCreateAccountRquest::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_strAccountName>>m_strBlowfishKey;
		}

		void PktNewCellApp::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_appID<<m_internBindAddr;
		}

		void PktNewCellApp::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_appID>>m_internBindAddr;
		}

		void PktBroadcastMsg::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_msg;
		}

		void PktBroadcastMsg::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_msg;
		}

		void PktRedirectToClient::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_targetClientEntityId;
			WriteBuffToStream(oStream);
		}

		void PktRedirectToClient::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_targetClientEntityId;
			ReadBuffFromStream(iStream);
		}

		void PktRedirectBroadcastToClient::WriteToStream( BinaryOStream & oStream ) const
		{
			WriteBuffToStream(oStream);
		}

		void PktRedirectBroadcastToClient::ReadFromStream( BinaryIStream & iStream )
		{
			ReadBuffFromStream(iStream);
		}


		void PktMethodCallFromCell::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eID<<m_mID;
			WriteBuffToStream(oStream);
		}

		void PktMethodCallFromCell::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eID>>m_mID;
			ReadBuffFromStream(iStream);
		}


		void PktOneAttributeChangeByCell::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eID<<m_attrID;
			WriteBuffToStream(oStream);
		}

		void PktOneAttributeChangeByCell::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eID>>m_attrID;
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


		void PktNewBaseApp::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_appID<<m_internBindAddr;
		}

		void PktNewBaseApp::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_appID>>m_internBindAddr;
		}

		void Pkt_BaseApp_WhoAmI::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_appID;
		}

		void Pkt_BaseApp_WhoAmI::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_appID;
		}
	}
}
