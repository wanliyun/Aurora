#ifndef _DB_MANAGER_INTERFACE_INTERFACE_H_
#define _DB_MANAGER_INTERFACE_INTERFACE_H_

#include <network/PacketBase.h>

namespace Aurora
{
	namespace DBManagerInterface
	{
		DECLARE_PACKET_FACTORY();

		enum EPacketIndex
		{
			//从AppManager发来的消息;
			EPacketIndex_WhoAmIResponse,

			//从BaseApp上发来的消息;
			EPacketIndex_WhoAmI,
			EPacketIndex_Test,
		};

		class DBManagerPacketBase : public PacketBase
		{
		protected:
			explicit DBManagerPacketBase(DBManagerInterface::EPacketIndex idx):PacketBase(EPacketInterface_DBManager,idx){}
		};

		class PktWhoAmIResponse : public DBManagerPacketBase
		{
		public:
			PktWhoAmIResponse():DBManagerPacketBase(DBManagerInterface::EPacketIndex_WhoAmIResponse){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			bool	m_bSucc;
			uint32	m_id;
		};

		class PktWhoAmI: public DBManagerPacketBase
		{
		public:
			PktWhoAmI():DBManagerPacketBase(DBManagerInterface::EPacketIndex_WhoAmI){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			AppID m_baseAppId;
		};

		class PktTest: public DBManagerPacketBase
		{
		public:
			PktTest():DBManagerPacketBase(DBManagerInterface::EPacketIndex_Test){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
		};
	}
}

#endif
