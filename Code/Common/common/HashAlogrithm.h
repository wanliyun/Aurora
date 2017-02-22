#ifndef _Aurora_HASH_ALOGRITHM_H_
#define _Aurora_HASH_ALOGRITHM_H_

namespace Aurora
{
	namespace HashUtil
	{
		uint32 Hash(const std::string & str);
		uint64 Hash64(const std::string & str);
	}
}

#endif //_Aurora_HASH_ALOGRITHM_H_