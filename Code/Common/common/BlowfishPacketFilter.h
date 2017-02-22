#ifndef _BLOWFISH_PACKETFILTER_H_
#define _BLOWFISH_PACKETFILTER_H_

#include "BlowfishCipher.h"
#include "interface/IPacketFilter.h"

namespace Aurora
{
	class CBlowfishPacketFilter : public CBlowfishCipher, public IPacketFilter
	{
	public:
		explicit CBlowfishPacketFilter(const std::string & strBlowfishiKey );
		virtual void Send(const void * pData,uint32 nDataLen,BinaryOStream & os);
		virtual void Recv(const void * pData,uint32 nDataLen,BinaryOStream & os);
	private:
		//½öÉùÃ÷;
		CBlowfishPacketFilter(const CBlowfishPacketFilter & );
		CBlowfishPacketFilter & operator = (const CBlowfishPacketFilter &);
	};
}

#endif //_BLOWFISH_PACKETFILTER_H_
