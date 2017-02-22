#ifndef _CLIENT_INTERFACE_H_
#define _CLIENT_INTERFACE_H_

#include <Types.h>
#include <network/PacketBase.h>

namespace Aurora
{
	namespace ClientInterface
	{
		DECLARE_PACKET_FACTORY();

		enum EPacketIndex
		{
			//来自LoginApp;
			EPacketIndex_LoginResponse,

			//来自BaseApp和CellApp;
			EPacketIndex_WhoAmIResponse,
			EPacketIndex_EchoBack,
			EPacketIndex_BroadCast,
			EPacketIndex_BasePlayerCreate,
			EPacketIndex_OneAttributeChange,
			EPacketIndex_PlayerEnterSpace,
			EPacketIndex_PlayerLeaveSpace,
			EPacketIndex_EntityEnterAOI,
			EPacketIndex_EntityLeaveAOI,
			EPacketIndex_MethodCallFromBase,
			EPacketIndex_MethodCallFromCell,
			EPacketIndex_SyncPosRot
		};

		class ClientPacketBase : public PacketBase
		{
		protected:
			explicit ClientPacketBase(EPacketIndex idx);
		};

		class PktLoginResponse : public ClientPacketBase
		{
		public:
			PktLoginResponse();
			~PktLoginResponse(){}
			enum {
				PacketDataLen = sizeof(uint8) + sizeof(uint32) + sizeof(uint16),
				PacketID
			};

			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			enum ECode
			{
				EC_Sucess,
				EC_Failed,
			};

			uint8				m_code;	//参见ECode;
			
			uint32				m_baseAppIp;
			uint16				m_baseAppPort;
			std::string			m_strAccountName;
		private:
			static const uint16 NET_DATA_LEN = sizeof(uint8) + sizeof(uint32) + sizeof(uint16);
		};

		class PktWhoAmIResponse : public ClientPacketBase
		{
		public:
			PktWhoAmIResponse():ClientPacketBase(ClientInterface::EPacketIndex_WhoAmIResponse){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			bool	m_bSucc;
			XEntityID m_accuntEntityID;
		};

		class PktEchoBack : public ClientPacketBase
		{
		public:
			PktEchoBack():ClientPacketBase(ClientInterface::EPacketIndex_EchoBack){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			char m_buff[256];
		};

		class PktBroadCast : public ClientPacketBase
		{
		public:
			PktBroadCast():ClientPacketBase(ClientInterface::EPacketIndex_BroadCast){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			char m_buff[256];
		};

		class PktBasePlayerCreate : public ClientPacketBase
		{
		public:
			PktBasePlayerCreate():ClientPacketBase(ClientInterface::EPacketIndex_BasePlayerCreate){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityTypeID	m_tid;
			XEntityID		m_id;
		};

		class PktOneAttributeChange : public ClientPacketBase
		{
		public:
			PktOneAttributeChange():ClientPacketBase(ClientInterface::EPacketIndex_OneAttributeChange){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID		m_eID;
			VAttributesT	m_path;
			AttributeID		m_attrID;
		};

		class PktPlayerEnterSpace : public ClientPacketBase
		{
		public:
			PktPlayerEnterSpace():ClientPacketBase(ClientInterface::EPacketIndex_PlayerEnterSpace){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			
			SpaceID m_spaceId;
			SpaceChannelID m_channleID;
			POS_TYPE pos_x,pos_y,pos_z;
			ROT_TYPE rot_y,rot_p,rot_r;
		};

		class PktPlayerLeaveSpace : public ClientPacketBase
		{
		public:
			PktPlayerLeaveSpace():ClientPacketBase(ClientInterface::EPacketIndex_PlayerLeaveSpace){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
		};

		class PktEntityEnterAOI : public ClientPacketBase
		{
		public:
			PktEntityEnterAOI():ClientPacketBase(ClientInterface::EPacketIndex_EntityEnterAOI){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eid;
			EntityTypeID m_tid;
			
			POS_TYPE pos_x,pos_y,pos_z;
			ROT_TYPE rot_y,rot_p,rot_r;
		};

		class PktEntityLeaveAOI : public ClientPacketBase
		{
		public:
			PktEntityLeaveAOI():ClientPacketBase(ClientInterface::EPacketIndex_EntityLeaveAOI){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eid;
		};

		class PktMethodCallFromBase : public ClientPacketBase
		{
		public:
			PktMethodCallFromBase():ClientPacketBase(ClientInterface::EPacketIndex_MethodCallFromBase){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eid;
			std::string m_strFunName;
			std::string m_strFunArg;
		};

		class PktMethodCallFromCell : public ClientPacketBase
		{
		public:
			PktMethodCallFromCell():ClientPacketBase(ClientInterface::EPacketIndex_MethodCallFromCell){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eID;
			MethodID m_mID;
		};

		class PktSyncPosRot : public ClientPacketBase
		{
		public:
			PktSyncPosRot():ClientPacketBase(ClientInterface::EPacketIndex_SyncPosRot){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eID;
			POS_TYPE pos_x,pos_y,pos_z;
			ROT_TYPE rot_y,rot_p,rot_r;
		};
	}
}

#endif
