#ifndef _LOGIN_INTERN_INTERFACE_H_
#define _LOGIN_INTERN_INTERFACE_H_

#include <network/PacketBase.h>

namespace Aurora
{
	namespace LoginInternInterface
	{
		DECLARE_PACKET_FACTORY();


		enum EPacketIndex
		{
			EPacketIndex_WhoAmIResponse,
			EPacketIndex_LoginResponse,
		};

		class LoginInternPacketBase : public PacketBase
		{
		protected:
			explicit LoginInternPacketBase(EPacketIndex idx):PacketBase(EPacketInterface_LoginIntern,idx){}
		};

		class PktWhoAmIResponse : public LoginInternPacketBase
		{
		public:
			PktWhoAmIResponse():LoginInternPacketBase(LoginInternInterface::EPacketIndex_WhoAmIResponse){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);
			bool	m_bSucc;
			uint32	m_id;
		};

		class PktLoginResponse : public LoginInternPacketBase
		{
		public:
			PktLoginResponse():LoginInternPacketBase(LoginInternInterface::EPacketIndex_LoginResponse){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			enum ECode
			{
				EC_Sucess,
				EC_Failed,
			};

			AppID				m_clntAppID;
			uint8				m_code;	//²Î¼ûECode;

			uint32				m_baseAppIp;
			uint16				m_baseAppPort;
		};
	}
}

#endif
