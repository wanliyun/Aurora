

#include "pch.h"
#include "CellInterface.h"
#include <binary_stream.h>

using namespace std;

namespace Aurora
{
	namespace CellInterface
	{
		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_Cell);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_WhoAmIResponse				,CellInterface::PktWhoAmIResponse				);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_SyncAllSpaceInfo				,CellInterface::PktSyncAllSpaceInfo				);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_WhoAmIOfBaseApp				,CellInterface::PktWhoAmIOfBaseApp				);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_CreateEntityOnCell			,CellInterface::PktCreateEntityOnCell			);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_DestroyEntityOnCell			,CellInterface::PktDestroyEntityOnCell			);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_MethodCallFromClient			,CellInterface::PktMethodCallFromClient			);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_OneAttributeChange			,CellInterface::PktOneAttributeChange			);
			ADD_PACKET_PRODUCT(CellInterface::EPacketIndex_SyncPosRot					,CellInterface::PktSyncPosRot					);
		END_IMPLEMENT_PACKET_FACTORY();


		void PktWhoAmIResponse::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_bSucc<<m_id;
		}

		void PktWhoAmIResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_bSucc>>m_id;
		}


		void PktWhoAmIOfBaseApp::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_baseAppID;
		}

		void PktWhoAmIOfBaseApp::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_baseAppID;
		}

		void PktCreateEntityOnCell::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_id<<m_tid<<m_fullCellID;
		}

		void PktCreateEntityOnCell::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_id>>m_tid>>m_fullCellID;
		}


		void PktDestroyEntityOnCell::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_id;
		}

		void PktDestroyEntityOnCell::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_id;
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

		void PktOneAttributeChange::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eID<<m_attrID;
			WriteBuffToStream(oStream);
		}

		void PktOneAttributeChange::ReadFromStream( BinaryIStream & iStream )
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

	}

}
