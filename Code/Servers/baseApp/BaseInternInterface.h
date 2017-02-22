#ifndef _BASE_INTERN_INTERFACE_H_
#define _BASE_INTERN_INTERFACE_H_

#include <network/PacketBase.h>

namespace Aurora
{
	namespace BaseInternInterface
	{
		DECLARE_PACKET_FACTORY();

		enum EPacketIndex
		{
			EPacketIndex_WhoAmIResponse,
			EPacketIndex_SyncAllSpaceInfo,
			EPacketIndex_CreateAccountRquest,
			EPacketIndex_NewCellApp,
			EPacketIndex_BroadCast,
			EPacketIndex_RedirectToClient,
			EPacketIndex_RedirectBoradcastToClient,

			EPacketIndex_MethodCallFromCell,
			EPacketIndex_OneAttributeChangeByCell,
			EPacketIndex_SyncPosRot,

			EPacketIndex_NewBaseApp,

			//////////////////////////////////////////////////////////////////////////
			//BaseApp发来的数据包;
			EPacketIndex_BaseApp_WhoAmI,
			//~BaseApp发来的数据包;
			//////////////////////////////////////////////////////////////////////////
		};

		class BaseInternPacketBase : public PacketBase
		{
		protected:
			explicit BaseInternPacketBase(BaseInternInterface::EPacketIndex idx):PacketBase(EPacketInterface_BaseIntern,uint16(idx)){}
		};

		class PktWhoAmIResponse : public BaseInternPacketBase
		{
		public:
			PktWhoAmIResponse():BaseInternPacketBase(BaseInternInterface::EPacketIndex_WhoAmIResponse){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			bool	m_bSucc;
			uint32	m_id;
		};
		class PktSyncAllSpaceInfo : public BaseInternPacketBase
		{
		public:
			PktSyncAllSpaceInfo():BaseInternPacketBase(BaseInternInterface::EPacketIndex_SyncAllSpaceInfo){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
		};

		class PktCreateAccountRquest : public BaseInternPacketBase
		{
		public:
			PktCreateAccountRquest():BaseInternPacketBase(BaseInternInterface::EPacketIndex_CreateAccountRquest){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			std::string		m_strAccountName;
			std::string		m_strBlowfishKey;
		};

		class PktNewCellApp : public BaseInternPacketBase
		{
		public:
			PktNewCellApp():BaseInternPacketBase(BaseInternInterface::EPacketIndex_NewCellApp){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			AppID m_appID;
			SocketAddress m_internBindAddr;
		};

		class PktBroadcastMsg : public BaseInternPacketBase
		{
		public:
			PktBroadcastMsg():BaseInternPacketBase(BaseInternInterface::EPacketIndex_BroadCast){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			char m_msg[256];
		};

		class PktRedirectToClient : public BaseInternPacketBase
		{
		public:
			PktRedirectToClient():BaseInternPacketBase(BaseInternInterface::EPacketIndex_RedirectToClient){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityID m_targetClientEntityId;
		};

		class PktRedirectBroadcastToClient : public BaseInternPacketBase
		{
		public:
			PktRedirectBroadcastToClient():BaseInternPacketBase(BaseInternInterface::EPacketIndex_RedirectBoradcastToClient){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
		};

		class PktMethodCallFromCell : public BaseInternPacketBase
		{
		public:
			PktMethodCallFromCell():BaseInternPacketBase(BaseInternInterface::EPacketIndex_MethodCallFromCell){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityID m_eID;
			MethodID m_mID;
		};

		class PktOneAttributeChangeByCell : public BaseInternPacketBase
		{
		public:
			PktOneAttributeChangeByCell():BaseInternPacketBase(BaseInternInterface::EPacketIndex_OneAttributeChangeByCell){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityID m_eID;
			AttributeID m_attrID;
		};
		
		class PktSyncPosRot : public BaseInternPacketBase
		{
		public:
			PktSyncPosRot():BaseInternPacketBase(BaseInternInterface::EPacketIndex_SyncPosRot){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityID m_eID;
			POS_TYPE pos_x,pos_y,pos_z;
			ROT_TYPE rot_y,rot_p,rot_r;
		};


		class PktNewBaseApp : public BaseInternPacketBase
		{
		public:
			PktNewBaseApp():BaseInternPacketBase(BaseInternInterface::EPacketIndex_NewBaseApp){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			AppID m_appID;
			SocketAddress m_internBindAddr;
		};

		class Pkt_BaseApp_WhoAmI : public BaseInternPacketBase
		{
		public:
			Pkt_BaseApp_WhoAmI():BaseInternPacketBase(BaseInternInterface::EPacketIndex_BaseApp_WhoAmI){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			AppID m_appID;
		};
	}
}

#endif
