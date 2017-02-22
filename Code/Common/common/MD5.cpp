
#include "pch.h"
#include "MD5.h"
#include "binary_stream.h"


#define MD5 MD5Hash // to rename the MD5 function declaration in md5.h
#include "openssl/md5.h"
#undef MD5

namespace Aurora
{

	class MD5Data
	{
	public:
		MD5_CTX state_;
	};

	MD5::MD5() :
		pData_( new MD5Data )
	{
		MD5_Init( &pData_->state_ );
	}

	MD5::~MD5()
	{
		SAFE_DELETE(pData_);
	}

	void MD5::Append( const void * data, int numBytes )
	{
		MD5_Update( &pData_->state_, (const unsigned char*)data, numBytes );
	}

	void MD5::GetDigest( MD5::Digest & digest )
	{
		MD5_Final( digest.bytes, &pData_->state_ );
	}


	void MD5::Digest::Clear()
	{
		memset( this, 0, sizeof(*this) );
	}

	bool MD5::Digest::operator==( const Digest & other ) const
	{
		return memcmp( this->bytes, other.bytes, sizeof( Digest ) ) == 0;
	}

	bool MD5::Digest::operator<( const Digest & other ) const
	{
		return memcmp( this->bytes, other.bytes, sizeof( Digest ) ) < 0;
	}

	std::string MD5::Digest::Quote() const
	{
		const char hexTbl[17] = "0123456789ABCDEF";

		char buf[32];
		for (uint32 i = 0; i < 16; i++)
		{
			buf[(i<<1)|0] = hexTbl[ bytes[i]>>4 ];
			buf[(i<<1)|1] = hexTbl[ bytes[i]&0x0F ];
		}

		return std::string( buf, 32 );
	}

	static inline unsigned char UnquoteNibble( char c )
	{
		return c > '9' ? c - ('A'-10) : c - '0';
	}

	bool MD5::Digest::Unquote( const std::string & quotedDigest )
	{
		if (quotedDigest.length() == 32)
		{
			const char * buf = quotedDigest.c_str();
			for (uint32 i = 0; i < 16; i++)
			{
				bytes[i] = (UnquoteNibble( buf[(i<<1)|0] ) << 4) |
					UnquoteNibble( buf[(i<<1)|1] );
			}
			return true;
		}
		else
		{
			this->Clear();
			return false;
		}
	}


	BinaryIStream& operator>>( BinaryIStream &is, MD5::Digest &d )
	{
		memcpy( d.bytes, is.retrieve( sizeof( d.bytes ) ), sizeof( d.bytes ) );
		return is;
	}

	BinaryOStream& operator<<( BinaryOStream &os, const MD5::Digest &d )
	{
		os.addBlob( d.bytes, sizeof( d.bytes ) );
		return os;
	}
}