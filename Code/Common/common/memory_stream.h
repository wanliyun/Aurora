
#ifndef MEMORY_STREAM_HPP
#define MEMORY_STREAM_HPP

#include "binary_stream.h"
#include "debug.h"

namespace Aurora
{

	class MemoryOStream : public BinaryOStream, public BinaryIStream
	{
	public: 
		MemoryOStream( int size = 64 );
		virtual ~MemoryOStream();

	protected:
		MemoryOStream( const MemoryOStream & );
		MemoryOStream & operator=( const MemoryOStream & );

	public: // overrides from BinaryOStream
		virtual void * reserve( int nBytes );
		virtual int size() const;

	public: // overrides from BinaryIStream
		virtual const void * retrieve( int nBytes );
		virtual int remainingLength() const;
		virtual char peek();

	public: // own methods
		bool shouldDelete() const;
		void shouldDelete( bool value );
		void * data();
		void reset();
		void rewind();


	protected:
		char * pBegin_;
		char * pCurr_;
		char * pEnd_;
		char * pRead_;
		bool shouldDelete_;
	};

	class MemoryIStream : public BinaryIStream
	{
	public: // constructors and destructor
		MemoryIStream( const char * pData, int length ) :
			BinaryIStream(),
			pCurr_( pData ),
			pEnd_( pCurr_ + length )
		{}

		MemoryIStream( const void * pData, int length ) :
			pCurr_( reinterpret_cast< const char * >( pData ) ),
			pEnd_( pCurr_ + length )
		{}

		virtual ~MemoryIStream();


	public: // overrides from BinaryIStream
		virtual const void * retrieve( int nBytes );

		virtual int remainingLength() const { return (int)(pEnd_ - pCurr_); }

		virtual char peek();

		virtual void finish()	{ pCurr_ = pEnd_; }

	public: // new methods
		const void * data() { return pCurr_; }

	protected:
		MemoryIStream() :
			pCurr_( NULL ),
			pEnd_( NULL )
		{}

		void init( const char * pData, int length )
		{
			pCurr_ = pData;
			pEnd_ = pCurr_ + length;
		}

	private:
		const char * pCurr_;
		const char * pEnd_;
	};


	class MessageStream : public MemoryOStream
	{
	public: // constructors and destructor
		MessageStream( int size = 64 );

		virtual ~MessageStream() {}

	public: // own methods
		bool addToStream( BinaryOStream & stream, uint8 messageID );
		bool getMessage( BinaryIStream & stream,
			int & messageID, int & length );
	};
}

#ifndef __APPLE__
#define MY_INLINE inline
#include "memory_stream.inc"
#endif

#endif // MEMORY_STREAM_HPP
