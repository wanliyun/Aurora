
#include "pch.h"
#include "ClientInterface.h"
#include <binary_stream.h>
#include <boost/serialization/singleton.hpp>

using namespace std;

namespace Aurora
{
	namespace ClientInterface
	{

		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_Client);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_LoginResponse			,ClientInterface::PktLoginResponse					);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_WhoAmIResponse			,ClientInterface::PktWhoAmIResponse					);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_EchoBack				,ClientInterface::PktEchoBack						);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_BroadCast				,ClientInterface::PktBroadCast						);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_BasePlayerCreate		,ClientInterface::PktBasePlayerCreate				);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_OneAttributeChange		,ClientInterface::PktOneAttributeChange				);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_PlayerEnterSpace		,ClientInterface::PktPlayerEnterSpace				);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_PlayerLeaveSpace		,ClientInterface::PktPlayerLeaveSpace				);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_EntityEnterAOI			,ClientInterface::PktEntityEnterAOI					);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_EntityLeaveAOI			,ClientInterface::PktEntityLeaveAOI					);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_MethodCallFromBase		,ClientInterface::PktMethodCallFromBase				);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_MethodCallFromCell		,ClientInterface::PktMethodCallFromCell				);
			ADD_PACKET_PRODUCT(ClientInterface::EPacketIndex_SyncPosRot				,ClientInterface::PktSyncPosRot				);
		END_IMPLEMENT_PACKET_FACTORY();

		ClientPacketBase::ClientPacketBase( EPacketIndex idx  ): PacketBase( EPacketInterface_Client, uint16(idx)  )
		{

		}

		PktLoginResponse::PktLoginResponse():ClientPacketBase(EPacketIndex_LoginResponse)
		{

		}

		void PktLoginResponse::WriteToStream( BinaryOStream & oStream )const
		{
			oStream<<m_code<<m_baseAppIp<<m_baseAppPort<<m_strAccountName;
		}

		void PktLoginResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_code>>m_baseAppIp>>m_baseAppPort>>m_strAccountName;
		}

		void PktWhoAmIResponse::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_bSucc<<m_accuntEntityID;
		}

		void PktWhoAmIResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_bSucc>>m_accuntEntityID;
		}

		void PktEchoBack::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_buff;
		}

		void PktEchoBack::ReadFromStream( BinaryIStream & iStream )
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


		void PktBasePlayerCreate::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_id<<m_tid;
			WriteBuffToStream(oStream);
		}

		void PktBasePlayerCreate::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_id>>m_tid;
			ReadBuffFromStream(iStream);
		}
		void PktOneAttributeChange::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eID<<m_path<<m_attrID;
			WriteBuffToStream(oStream);
		}

		void PktOneAttributeChange::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eID>>m_path>>m_attrID;
			ReadBuffFromStream(iStream);
		}

		void PktPlayerEnterSpace::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_spaceId<<m_channleID<<pos_x<<pos_y<<pos_z<<rot_y<<rot_p<<rot_r;
		}

		void PktPlayerEnterSpace::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_spaceId>>m_channleID>>pos_x>>pos_y>>pos_z>>rot_y>>rot_p>>rot_r;
		}


		void PktPlayerLeaveSpace::WriteToStream( BinaryOStream & /*oStream*/ ) const
		{

		}

		void PktPlayerLeaveSpace::ReadFromStream( BinaryIStream & /*iStream*/ )
		{

		}


		void PktEntityEnterAOI::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eid<<m_tid<<pos_x<<pos_y<<pos_z<<rot_y<<rot_p<<rot_r;
		}

		void PktEntityEnterAOI::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eid>>m_tid>>pos_x>>pos_y>>pos_z>>rot_y>>rot_p>>rot_r;
		}

		void PktEntityLeaveAOI::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eid;
		}

		void PktEntityLeaveAOI::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eid;
		}


		void PktMethodCallFromBase::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eid<<m_strFunName<<m_strFunArg;
		}

		void PktMethodCallFromBase::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eid>>m_strFunName>>m_strFunArg;
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

