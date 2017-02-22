#ifndef _FFB_H_
#define _FFB_H_


namespace Aurora
{
	namespace BitUtil
	{
		//查找最低有效位;
		//@return n为0时 uint32(-1)  ,否则返回[0,63];
		inline uint32 FFB(uint64 n)
		{
			if(n == 0)
				return uint32(-1);

			uint32 ret(0);
			if(0 == (n & 0x00000000FFFFFFFF))
			{
				n>>=32;
				ret += 32;
			}

			if(0 == (n & 0x000000000000FFFF))
			{
				n>>=16;
				ret += 16;
			}

			if(0 == (n & 0x00000000000000FF))
			{
				n>>=8;
				ret += 8;
			}

			if(0 == (n & 0x00000000000000FF))
			{
				n>>=8;
				ret += 8;
			}

			if(0 == (n & 0x000000000000000F))
			{
				n>>=4;
				ret += 4;
			}

			if(0 == (n & 0x0000000000000003))
			{
				n>>=2;
				ret += 2;
			}

			if(0 == (n & 0x0000000000000001))
			{
				ret += 1;
			}

			return ret;
		}

		//查找最高有效位;
		//@return n为0时 uint32(-1)  ,否则返回[0,63];
		inline uint32 FLB(uint64 n)
		{
			if(n == 0)
				return uint32(-1);
			uint32 ret(0);

			if(0 != (n & 0xFFFFFFFF00000000))
			{
				ret += 32;
				n>>=32;
			}

			if(0 != (n & 0xFFFF0000))
			{
				ret += 16;
				n>>=16;
			}

			if(0 != (n & 0xFF00))
			{
				ret += 8;
				n>>=8;
			}

			if(0 != (n & 0xF0))
			{
				ret += 4;
				n>>=4;
			}

			if(0 != (n & 0xC))
			{
				ret += 2;
				n>>=2;
			}

			if(0 != (n & 0x2))
			{
				ret += 1;
			}

			return ret;
		}
	}
}

#endif 