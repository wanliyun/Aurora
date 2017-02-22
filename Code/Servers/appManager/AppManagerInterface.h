#ifndef _APP_MANAGER_INTERFACE_H_
#define _APP_MANAGER_INTERFACE_H_

#include <network/PacketBase.h>
#include <network/MySocketAPI.h>

namespace Aurora
{
	namespace AppManagerInterface
	{
		DECLARE_PACKET_FACTORY();


		enum EPacketIndex
		{
			EPacketIndex_WhoAmI,
			EPacketIndex_LoginRquest,
			EPacketIndex_NoticeClientLose,
			EPacketIndex_CreatePlayerResponse,
			EPacketIndex_BroadCast,
			EPacketIndex_CreateEntityOnCell,
		};

		class AppManagerPacketBase : public PacketBase
		{
		protected:
			explicit AppManagerPacketBase(AppManagerInterface::EPacketIndex idx):PacketBase(EPacketInterface_AppManager,uint16(idx) ){}
		};

		class PktWhoAmI : public AppManagerPacketBase
		{
		public:
			PktWhoAmI():AppManagerPacketBase(AppManagerInterface::EPacketIndex_WhoAmI){}

			enum EAppType
			{
				EAppType_BaseApp,
				EAppType_CellApp,
				EAppType_LoginApp,
				EAppType_DBManager,
			};

			static const char * GetNameByType(uint8 tp);
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			uint8 m_AppType; 
			SocketAddress m_internBindAddr;
			SocketAddress m_externBindAddr;
		};

		class PktLoginRquest : public AppManagerPacketBase
		{
		public:
			PktLoginRquest():AppManagerPacketBase(AppManagerInterface::EPacketIndex_LoginRquest){}

			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			static const int32 MAX_USERNAME_LEN = 255;
			static const int32 MAX_PASSWORD_LEN = 255;
			AppID m_clntID;
			std::string m_strUserName;
			std::string m_strPassword;
			std::string m_strBlowfishKey;
		};

		//BaseApp发出，通知玩家掉线了;
		class PktNoticeClientLose : public AppManagerPacketBase
		{
		public:
			PktNoticeClientLose():AppManagerPacketBase(AppManagerInterface::EPacketIndex_NoticeClientLose){}

			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			EntityID	m_eID;
		};

		class PktCreatePlayerResponse : public AppManagerPacketBase
		{
		public:
			PktCreatePlayerResponse():AppManagerPacketBase(AppManagerInterface::EPacketIndex_CreatePlayerResponse){}

			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			bool		m_bSucc;
			std::string	m_strAccountName;
		};

		class PktBroadcastMsg : public AppManagerPacketBase
		{
		public:
			PktBroadcastMsg():AppManagerPacketBase(AppManagerInterface::EPacketIndex_BroadCast){}

			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			char m_msg[256];
		};

		class PktCreateEntityOnCell : public AppManagerPacketBase
		{
		public:
			PktCreateEntityOnCell():AppManagerPacketBase(AppManagerInterface::EPacketIndex_CreateEntityOnCell){}

			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			EntityID m_id;
			EntityTypeID m_tid;
		};
	}
}

#endif
