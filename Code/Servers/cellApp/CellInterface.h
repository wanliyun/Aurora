#ifndef _CELL_INTERFACE_H_
#define _CELL_INTERFACE_H_

#include <network/PacketBase.h>

namespace Aurora
{
	namespace CellInterface
	{
		DECLARE_PACKET_FACTORY();

		enum EPacketIndex
		{
			EPacketIndex_WhoAmIResponse,
			EPacketIndex_SyncAllSpaceInfo,
			EPacketIndex_WhoAmIOfBaseApp,
			EPacketIndex_CreateEntityOnCell,
			EPacketIndex_DestroyEntityOnCell,

			EPacketIndex_MethodCallFromClient,
			EPacketIndex_OneAttributeChange,
			EPacketIndex_SyncPosRot,
		};

		class CellPacketBase : public PacketBase
		{
		protected:
			explicit CellPacketBase(CellInterface::EPacketIndex idx):PacketBase(EPacketInterface_Cell,uint16(idx)){}
		};

		class PktWhoAmIResponse : public CellPacketBase
		{
		public:
			PktWhoAmIResponse():CellPacketBase(CellInterface::EPacketIndex_WhoAmIResponse){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			bool	m_bSucc;
			uint32	m_id;
		};


		class PktSyncAllSpaceInfo : public CellPacketBase
		{
		public:
			PktSyncAllSpaceInfo():CellPacketBase(CellInterface::EPacketIndex_SyncAllSpaceInfo){}
			virtual void WriteToStream(BinaryOStream & oStream)const{WriteBuffToStream(oStream);}
			virtual void ReadFromStream(BinaryIStream & iStream){ReadBuffFromStream(iStream);}
		};

		class PktWhoAmIOfBaseApp : public CellPacketBase
		{
		public:
			PktWhoAmIOfBaseApp():CellPacketBase(CellInterface::EPacketIndex_WhoAmIOfBaseApp){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			AppID m_baseAppID;
		};

		class PktCreateEntityOnCell : public CellPacketBase
		{
		public:
			PktCreateEntityOnCell():CellPacketBase(CellInterface::EPacketIndex_CreateEntityOnCell){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_id;
			EntityTypeID m_tid;
			SFullCellID m_fullCellID;
		};

		class PktDestroyEntityOnCell : public CellPacketBase
		{
		public:
			PktDestroyEntityOnCell():CellPacketBase(CellInterface::EPacketIndex_DestroyEntityOnCell){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_id;
		};

		class PktMethodCallFromClient : public CellPacketBase
		{
		public:
			PktMethodCallFromClient():CellPacketBase(CellInterface::EPacketIndex_MethodCallFromClient){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eID;
			MethodID m_mID;
		};

		class PktOneAttributeChange : public CellPacketBase
		{
		public:
			PktOneAttributeChange():CellPacketBase(CellInterface::EPacketIndex_OneAttributeChange){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eID;
			AttributeID m_attrID;
		};

		class PktSyncPosRot : public CellPacketBase
		{
		public:
			PktSyncPosRot():CellPacketBase(CellInterface::EPacketIndex_SyncPosRot){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_eID;
			POS_TYPE pos_x,pos_y,pos_z;
			ROT_TYPE rot_y,rot_p,rot_r;
		};
	}
}

#endif
