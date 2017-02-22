#ifndef _Aurora_MD5_H_
#define _Aurora_MD5_H_

namespace Aurora
{
	class BinaryOStream;
	class BinaryIStream;
	class MD5Data;

	class MD5
	{
	public:
		struct Digest
		{
			unsigned char bytes[16];

			Digest() { }
			Digest( MD5 & md5 ) { *this = md5; }
			explicit Digest( const char * quotedDigest )
			{ 
				this->Unquote( quotedDigest ); 
			}
			Digest & operator=( MD5 & md5 )
			{ 
				md5.GetDigest( *this ); return *this; 
			}
			void Clear();

			bool operator==( const Digest & other ) const;
			bool operator!=( const Digest & other ) const
			{ 
				return !(*this == other); 
			}

			bool operator<( const Digest & other ) const;

			std::string Quote() const;
			bool Unquote( const std::string & quotedDigest );

			bool IsEmpty() const
			{
				for (size_t i = 0; i < sizeof( bytes ); ++i)
				{
					if (bytes[i] != '\0')
					{
						return false;
					}
				}

				return true;
			}
		};

		MD5();
		~MD5();

		void Append( const void * data, int numBytes );
		void GetDigest( Digest & digest );

	private:
		MD5Data * pData_;
	};

	BinaryIStream& operator>>( BinaryIStream &is, MD5::Digest &d );
	BinaryOStream& operator<<( BinaryOStream &is, const MD5::Digest &d );

}

#endif //_UTILS_H_