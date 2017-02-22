#include "pch.h"
#include "binary_stream.h"

#include <sstream>
#include <iomanip>
#include <stdlib.h>

namespace Aurora
{

	// Static return buffer returned from retrieve() when not enough data for
	// request.
	char BinaryIStream::errBuf[ BS_BUFF_MAX_SIZE ];


	/**
	*	This method creates a debug string for outputing to logs. It is a hex
	*	representation of the remaining bytes in the stream.
	*/
	std::string BinaryIStream::remainingBytesAsDebugString( int maxBytes )
	{
		std::stringstream output;
		int numBytes = min( this->remainingLength(), maxBytes );
		output << "Bytes (" <<
			numBytes << " of " << this->remainingLength() << "):";

		output << std::hex << std::setfill( '0' );

		for (int i = 0; i < numBytes; ++i)
		{
			output << " " << std::setw( 2 ) << int(*(uint8 *)this->retrieve( 1 ) );
		}

		output << std::ends;

		this->finish();

		return output.str();
	}


}
// binary_stream.cpp
