namespace Aurora
{


	MY_INLINE MemoryOStream::MemoryOStream( int size )
	{
		pBegin_ = new char[ size ];
		pCurr_ = pBegin_;
		pEnd_ = pBegin_ + size;
		shouldDelete_ = true;
		pRead_ = pBegin_;
	}

	/**
	* 	This is the destructor. If the stream owns the data buffer, it
	* 	deletes it.
	*
	* 	@see shouldDelete
	*/
	MY_INLINE MemoryOStream::~MemoryOStream()
	{
		if (shouldDelete_)
		{
			SAFE_DELETE_ARRAY(pBegin_);
		}
	}

	/**
	* 	This method returns the number of bytes that have been
	* 	added to this stream.
	*
	* 	@return The number of bytes in the stream.
	*/
	MY_INLINE int MemoryOStream::size() const
	{
		return (int)(pCurr_ - pBegin_);
	}

	/**
	*	This method returns true if the memory stream owns the
	*	data buffer associated with it. If this is the case, the
	*	buffer will be deleted when the stream is destroyed.
	*
	*	@return true if the stream owns the buffer, false otherwise.
	*/
	MY_INLINE bool MemoryOStream::shouldDelete() const
	{
		return shouldDelete_;
	}

	/**
	* 	This method sets the buffer ownership flag. If this is true,
	* 	the buffer will be deleted when this stream's destructor is
	* 	called.
	*/
	MY_INLINE void MemoryOStream::shouldDelete( bool value )
	{
		// Make sure we don't try to set this to false twice.
		MY_ASSERT( shouldDelete_ || value );
		shouldDelete_ = value;
	}

	/**
	* 	This method returns a pointer to the data associated with
	* 	this stream.
	*
	* 	@return Pointer to stream data.
	*/
	MY_INLINE void * MemoryOStream::data()
	{
		return pBegin_;
	}

	/**
	*	This method reserves a given number of bytes on the stream.
	*	All other stream write operations use this method.
	*
	*	@param nBytes	Number of bytes to reserve.
	*
	*	@return Pointer to the reserved data in the stream.
	*/
	MY_INLINE void * MemoryOStream::reserve( int nBytes )
	{
		char * pOldCurr = pCurr_;
		pCurr_ += nBytes;

		if (pCurr_ > pEnd_)
		{
			int multiplier = (int)((pCurr_ - pBegin_)/(pEnd_ - pBegin_) + 1);
			int newSize = multiplier * (int)(pEnd_ - pBegin_);
			char * pNewData = new char[ newSize ];
			memcpy( pNewData, pBegin_, pOldCurr - pBegin_ );
			pCurr_ = pCurr_ - pBegin_ + pNewData;
			pOldCurr = pOldCurr - pBegin_ + pNewData;
			pRead_ = pRead_ - pBegin_ + pNewData;
			MY_ASSERT( shouldDelete_ );
			SAFE_DELETE_ARRAY(pBegin_);
			pBegin_ = pNewData;
			pEnd_ = pBegin_ + newSize;
		}

		return pOldCurr;
	}

	/**
	* 	This method retrieves a given number of bytes from the stream.
	* 	All other stream read operations use this method.
	*
	* 	@param nBytes	Number of bytes to retrieve.
	*
	* 	@return Pointer to the retrieved data in the stream.
	*/
	MY_INLINE const void * MemoryOStream::retrieve( int nBytes )
	{
		char * pOldRead = pRead_;

		IF_NOT_MY_ASSERT_DEV( pRead_ + nBytes <= pCurr_ )
		{
			error_ = true;

			pRead_ = pCurr_;

			// If someone has asked for more data than can fit on a packet this is a
			// pretty dire error
			return nBytes <= int( sizeof( errBuf ) ) ? errBuf : NULL;
		}

		pRead_ += nBytes;

		return pOldRead;
	}


	/**
	*	This method resets this stream.
	*/
	MY_INLINE void MemoryOStream::reset()
	{
		pRead_ = pCurr_ = pBegin_;
	}


	/**
	*	This method rewinds the read point
	*/
	MY_INLINE void MemoryOStream::rewind()
	{
		pRead_ = pBegin_;
	}


	/**
	*	This method returns the number of bytes remaining to be read.
	*/
	MY_INLINE int MemoryOStream::remainingLength() const
	{
		return (int)(pCurr_ - pRead_);
	}

	/**
	*  Returns the next character to be read without removing it from the stream.
	*/
	MY_INLINE char MemoryOStream::peek()
	{
		IF_NOT_MY_ASSERT_DEV( pRead_ < pCurr_ )
		{
			error_ = true;
			return char(-1);
		}

		return *(char*)pRead_;
	}


	// -----------------------------------------------------------------------------
	// Section: MemoryIStream
	// -----------------------------------------------------------------------------

	MY_INLINE MemoryIStream::~MemoryIStream()
	{
		if (pCurr_ != pEnd_)
		{
			LogF( "There are still %d bytes left\n", (int)(pEnd_ - pCurr_) );
		}
	}

	MY_INLINE const void * MemoryIStream::retrieve( int nBytes )
	{
		const char * pOldRead = pCurr_;

		// This is deliberately non-fatal (unlike the same method in MemoryOStream)
		// because machined uses these objects (exclusively, AFAIK) and we don't
		// want machined to MY_ASSERT on bogus packets
		if (pCurr_ + nBytes > pEnd_)
		{
			pCurr_ = pEnd_;
			error_ = true;
			return nBytes <= int( sizeof( errBuf  ) ) ? errBuf : NULL;
		}

		pCurr_ += nBytes;
		return pOldRead;
	}

	/**
	*  Returns the next character to be read without removing it from the stream.
	*/
	MY_INLINE char MemoryIStream::peek()
	{
		if (pCurr_ >= pEnd_)
		{
			error_ = true;
			return char(-1);
		}

		return *pCurr_;
	}

	// -----------------------------------------------------------------------------
	// Section: MessageStream
	// -----------------------------------------------------------------------------

	/**
	*	Constructor.
	*/
	MY_INLINE MessageStream::MessageStream( int size ) : MemoryOStream( size )
	{
	}


	/**
	*	This method adds a message to the input stream.
	*
	*	@return True if successful, otherwise false.
	*/
	MY_INLINE
		bool MessageStream::addToStream( BinaryOStream & stream, uint8 messageID )
	{
		int size = this->size();

		if (size >= 256)
		{
			// Size is too big
			return false;
		}

		stream << messageID << (uint8)size;

		memcpy( stream.reserve( size ), this->retrieve( size ), size );

		this->reset();

		return true;
	}


	/**
	*	This method retrieves a message from the bundle.
	*/
	MY_INLINE bool MessageStream::getMessage( BinaryIStream & stream,
		int & messageID, int & length )
	{
		this->reset();

		IF_NOT_MY_ASSERT_DEV( length >= 2 )
		{
			// Length is too short
			return false;
		}

		uint8 size;
		uint8 id;

		stream >> id >> size;
		length -= sizeof( id ) + sizeof( size );

		messageID = id;

		IF_NOT_MY_ASSERT_DEV( length >= (int)size )
		{
			return false;
		}

		length -= size;

		memcpy( this->reserve( size ), stream.retrieve( size ), size );

		return true;
	}
}
// memory_stream.ipp
