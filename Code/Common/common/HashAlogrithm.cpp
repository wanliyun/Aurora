
#include "pch.h"
#include "HashAlogrithm.h"

namespace Aurora
{

	// BKDR;
	uint32 HashUtil::Hash( const std::string & str )
	{
		uint32 seed = 131313; // 31 131 1313 13131 131313 etc..
		uint32 hash = 0;

		const size_t nCount = str.size();
		for(size_t i = 0;i<nCount;++i)
		{
			hash = hash * seed + uint32(str[i]);
		}
		return hash;
	}

	// BKDR;
	uint64 HashUtil::Hash64( const std::string & str )
	{
		uint64 seed = 131313; // 31 131 1313 13131 131313 etc..
		uint64 hash = 0;

		const size_t nCount = str.size();
		for(size_t i = 0;i<nCount;++i)
		{
			hash = hash * seed + uint64(str[i]);
		}

		return hash;
	}
}
