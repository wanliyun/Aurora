#ifndef _BASE_EXTERN_INTERFACE_H_
#define _BASE_EXTERN_INTERFACE_H_

#include <network/PacketBase.h>

namespace Aurora
{
	namespace BaseExternInterface
	{
		DECLARE_PACKET_FACTORY();


		enum EPacketIndex
		{
			EPacketIndex_WhoAmI,

			EPacketIndex_RedirectToCell,

			EPacketIndex_MethodCallFromClient,

			EPacketIndex_Echo,
			EPacketIndex_BroadCast,

			EPacketIndex_SyncPosRot,
		};

		class BaseExternPacketBase : public PacketBase
		{
		protected:
			explicit BaseExternPacketBase(BaseExternInterface::EPacketIndex idx):PacketBase(EPacketInterface_BaseExtern,idx){}
		};

		class PktWhoAmI : public BaseExternPacketBase
		{
		public:
			PktWhoAmI():BaseExternPacketBase(BaseExternInterface::EPacketIndex_WhoAmI){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			
			std::string m_strAccountName;
		};

		class PktRedirectToCell : public BaseExternPacketBase
		{
		public:
			PktRedirectToCell():BaseExternPacketBase(BaseExternInterface::EPacketIndex_RedirectToCell){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityID	m_nEntityID;
		};

		class PktEcho : public BaseExternPacketBase
		{
		public:
			PktEcho():BaseExternPacketBase(BaseExternInterface::EPacketIndex_Echo){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			char m_buff[256];
		};
		class PktBroadCast : public BaseExternPacketBase
		{
		public:
			PktBroadCast():BaseExternPacketBase(BaseExternInterface::EPacketIndex_BroadCast){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			char m_buff[256];
		};

		class PktMethodCallFromClient : public BaseExternPacketBase
		{
		public:
			PktMethodCallFromClient():BaseExternPacketBase(BaseExternInterface::EPacketIndex_MethodCallFromClient){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityID m_eID;
			MethodID m_mID;
		};

		class PktSyncPosRot : public BaseExternPacketBase
		{
		public:
			PktSyncPosRot():BaseExternPacketBase(BaseExternInterface::EPacketIndex_SyncPosRot){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eID;
			POS_TYPE pos_x,pos_y,pos_z;
			ROT_TYPE rot_y,rot_p,rot_r;
		};
	}
}

#endif
