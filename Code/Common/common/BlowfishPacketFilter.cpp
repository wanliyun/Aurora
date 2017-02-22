
#include "pch.h"
#include "BlowfishPacketFilter.h"
#include "binary_stream.h"

namespace Aurora
{
	CBlowfishPacketFilter::CBlowfishPacketFilter(const std::string & strBlowfishiKey)
	{
		InitKey(strBlowfishiKey);
	}

	void CBlowfishPacketFilter::Send( const void * pData,uint32 nDataLen,BinaryOStream & os )
	{
		if(!pData || nDataLen == 0)
			return;

		Encrypt(pData,os.reserve(nDataLen),nDataLen);
	}

	void CBlowfishPacketFilter::Recv( const void * pData,uint32 nDataLen,BinaryOStream & os )
	{
		if(!pData || nDataLen == 0)
			return;
		Decrypt(pData,os.reserve(nDataLen),nDataLen);
	}
}
