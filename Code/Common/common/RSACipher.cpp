
#include "pch.h"
#include "RSACipher.h"
#include <openssl/objects.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

namespace Aurora
{


	CRSACipher * CRSACipher::Create( const std::string & keyPath,bool isPub )
	{
		FILE * file = fopen(keyPath.c_str(),"r");
		if(!file)
			return 0;
		RSA * pRSA = isPub ? 
						PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL):
						PEM_read_RSAPrivateKey(file,NULL,NULL,NULL);;
		if(!pRSA)
			return 0;
		return new CRSACipher(pRSA,isPub);
	}

	void CRSACipher::Destroy( CRSACipher * pKey )
	{
		if(pKey)
			delete pKey;
	}

	CRSACipher::CRSACipher( RSA * pKey,bool isPub ):
		m_pKey(pKey),
		m_bIsPub(isPub),
		m_pBuf0(0),
		m_pBuf1(0)
	{
		if(m_pKey)
		{
			int rsa_len = RSA_size(pKey);
			if(rsa_len > 0)
			{
				m_nRSASize = rsa_len;
				m_pBuf0 = new unsigned char[rsa_len];
				m_pBuf1 = new unsigned char[rsa_len];
			}
		}
	}

	CRSACipher::~CRSACipher()
	{
		SAFE_DELETE_ARRAY(m_pBuf0);
		SAFE_DELETE_ARRAY(m_pBuf1);
		if(m_pKey)
			RSA_free(m_pKey);
	}

	bool CRSACipher::Encrypt( const std::string & text,std::string & outText )
	{
		if(!IsValid())
			return false;
		outText.clear();
		int nLeft = int(text.length());

		unsigned char * pFrom = (unsigned char * )text.c_str();

		while(nLeft > 0)
		{
			int nInLen =  nLeft > m_nRSASize - RSA_PKCS1_PADDING_SIZE ? m_nRSASize - RSA_PKCS1_PADDING_SIZE : nLeft;
			memset(m_pBuf0,0,m_nRSASize);
			memset(m_pBuf1,0,m_nRSASize);
			memcpy(m_pBuf0,pFrom,nInLen);
			int nRetLen = IsPub() ?  RSA_public_encrypt(nInLen,pFrom,m_pBuf1,m_pKey,RSA_PKCS1_PADDING):
									 RSA_private_encrypt(nInLen,pFrom,m_pBuf1,m_pKey,RSA_PKCS1_PADDING);

			if( nRetLen != m_nRSASize )
				break;

			outText.append((const char * )m_pBuf1,nRetLen);
			nLeft -= nInLen;
			pFrom+=nInLen;
		}
		return nLeft <=0;
	}

	bool CRSACipher::Decrypt( const std::string & text,std::string & outText )
	{
		if(!IsValid())
			return false;
		outText.clear();
		int nLeft = int(text.length());
		if(nLeft % m_nRSASize != 0)
			return false;

		unsigned char * pFrom = (unsigned char * )text.c_str();

		while(nLeft > 0)
		{
			int nInLen =  nLeft > m_nRSASize - RSA_PKCS1_PADDING_SIZE ? m_nRSASize - RSA_PKCS1_PADDING_SIZE : nLeft;
			memset(m_pBuf0,0,m_nRSASize);
			memset(m_pBuf1,0,m_nRSASize);

			int nRetLen = IsPub() ?  RSA_public_decrypt(m_nRSASize,pFrom,m_pBuf1,m_pKey,RSA_PKCS1_PADDING):
									 RSA_private_decrypt(m_nRSASize,pFrom,m_pBuf1,m_pKey,RSA_PKCS1_PADDING);
			if(nRetLen < 0)
				break;
			memcpy(m_pBuf0,pFrom,m_nRSASize);
			outText.append((const char * )m_pBuf1,nRetLen);
			nLeft -= nInLen;
			pFrom+=nInLen;
		}
		return nLeft <=0;
	}
}
