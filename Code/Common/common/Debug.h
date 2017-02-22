#ifndef _DEBUG_H_
#define _DEBUG_H_

namespace Aurora
{
#define MY_ASSERT(x) assert(x)


#define IF_NOT_MY_ASSERT_DEV( exp )\
	if( !(exp) )

}

#endif //_UTILS_H_