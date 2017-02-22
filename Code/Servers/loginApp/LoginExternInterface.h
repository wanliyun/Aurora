#ifndef _LOGIN_EXTERN_INTERFACE_H_
#define _LOGIN_EXTERN_INTERFACE_H_

#include <network/PacketBase.h>

namespace Aurora
{
	namespace LoginExternInterface
	{
		DECLARE_PACKET_FACTORY();

		enum EPacketIndex
		{
			EPacketIndex_LoginRquest,
		};

		class LoginExternPacketBase : public PacketBase
		{
		protected:
			explicit LoginExternPacketBase(EPacketIndex idx);
		};

		class PktLoginRquest : public LoginExternPacketBase
		{
		public:
			PktLoginRquest():LoginExternPacketBase(LoginExternInterface::EPacketIndex_LoginRquest){}
			virtual void WriteToStream(BinaryOStream & oStream)const;
			virtual void ReadFromStream(BinaryIStream & iStream);

			static const int32 MAX_USERNAME_LEN = 255;
			static const int32 MAX_PASSWORD_LEN = 255;

			std::string m_strUserName;
			std::string m_strPasssWord;
			std::string	m_strBlowfishKey;
		};
	}
}

#endif
