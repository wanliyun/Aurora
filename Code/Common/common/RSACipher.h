#ifndef _RSA_CIPHER_H_
#define _RSA_CIPHER_H_

struct rsa_st;
typedef struct rsa_st RSA;
namespace Aurora
{
	class CRSACipher
	{
	public:
		static CRSACipher * Create(const std::string & keyPath,bool isPub);
		static void Destroy(CRSACipher * pKey);

		inline bool IsValid()const{return m_pKey != 0 && m_pBuf0 != 0 && m_pBuf1 != 0 && m_nRSASize>0;}
		inline bool IsPub()const{return m_bIsPub;}

		bool Encrypt(const std::string & text,std::string & outText);
		bool Decrypt(const std::string & text,std::string & outText);

	private:
		CRSACipher(RSA * pKey,bool isPub);
		~CRSACipher();
		CRSACipher(const CRSACipher & );
		CRSACipher & operator = (const CRSACipher &);
	private:
		const bool m_bIsPub;
		RSA * const m_pKey;
		unsigned char * m_pBuf0;
		unsigned char * m_pBuf1;
		int			m_nRSASize;
	};
}

#endif //_RSA_CIPHER_H_
