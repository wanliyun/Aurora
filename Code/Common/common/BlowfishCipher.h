#ifndef _BLOWFISH_CIPHER_H_
#define _BLOWFISH_CIPHER_H_

struct bf_key_st;
typedef struct bf_key_st BF_KEY;
namespace Aurora
{
	class CBlowfishCipher
	{
	public:
		static bool GenerateKey(std::string & strKey,uint32 keyLen);

		CBlowfishCipher();
		~CBlowfishCipher();
		void InitKey(const std::string & strKey);
		
		void Encrypt(const std::string & text,std::string & outText);
		void Decrypt(const std::string & text,std::string & outText);
		
		void Encrypt(const void * pSrc,void * pDst,uint32 nLen);
		void Decrypt(const void * pSrc,void * pDst,uint32 nLen);
	private:
		//½öÉùÃ÷;
		CBlowfishCipher(const CBlowfishCipher & );
		CBlowfishCipher & operator = (const CBlowfishCipher &);
	private:
		BF_KEY * m_pKey;
	};
}

#endif //_RSA_CIPHER_H_
