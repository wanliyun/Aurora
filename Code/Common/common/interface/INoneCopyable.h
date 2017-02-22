#ifndef _I_NONE_COPYABLE_H_
#define _I_NONE_COPYABLE_H_


namespace Aurora
{
	struct INoneCopyable
	{
	public:
		INoneCopyable(){}
		~INoneCopyable(){}
	private:
		INoneCopyable(const INoneCopyable &);
		INoneCopyable & operator = (const INoneCopyable & );
	};
}

#endif
