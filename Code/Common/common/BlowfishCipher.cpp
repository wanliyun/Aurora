
#include "pch.h"
#include "BlowfishCipher.h"
#include <openssl/rand.h>
#include <openssl/blowfish.h>

namespace Aurora
{
	bool CBlowfishCipher::GenerateKey( std::string & strKey,uint32 keyLen )
	{
		strKey.resize(keyLen);
		return RAND_pseudo_bytes((unsigned char *)strKey.c_str(),keyLen) >=0;
	}

	CBlowfishCipher::CBlowfishCipher():m_pKey(new BF_KEY())
	{

	}

	CBlowfishCipher::~CBlowfishCipher()
	{
		SAFE_DELETE(m_pKey);
	}

	void CBlowfishCipher::InitKey( const std::string & strKey )
	{
		assert(m_pKey);
		BF_set_key(m_pKey,strKey.size(),(const unsigned char *)strKey.c_str());
	}


	void CBlowfishCipher::Encrypt( const std::string & text,std::string & outText )
	{
		const size_t textLen = text.size();
		if(textLen == 0)
			return;
		outText.resize(textLen);
		Encrypt(text.c_str(),(void*)outText.c_str(),textLen);
	}

	void CBlowfishCipher::Encrypt(const void * pSrc,void * pDst,uint32 nLen)
	{
		assert(m_pKey && pSrc && pDst && nLen > 0);
		if(!m_pKey || !pSrc || !pDst || nLen == 0)
			return;
		unsigned char iv[8] = {0};
		int num = 0;
		BF_cfb64_encrypt((const unsigned char *)pSrc,(unsigned char *)pDst,nLen,m_pKey,iv,&num,BF_ENCRYPT);
	}

	void CBlowfishCipher::Decrypt( const std::string & text,std::string & outText )
	{
		const size_t textLen = text.size();
		if(textLen == 0)
			return;
		outText.resize(textLen);
		Decrypt(text.c_str(),(void*)outText.c_str(),textLen);
	}

	void CBlowfishCipher::Decrypt(const void * pSrc,void * pDst,uint32 nLen)
	{
		assert(m_pKey && pSrc && pDst && nLen > 0);
		if(!m_pKey || !pSrc || !pDst || nLen == 0)
			return;
		unsigned char iv[8] = {0};
		int num = 0;
		BF_cfb64_encrypt((const unsigned char *)pSrc,(unsigned char *)pDst,nLen,m_pKey,iv,&num,BF_DECRYPT);
	}
}
