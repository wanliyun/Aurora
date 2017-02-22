#ifndef _PY_TYPE_LONG_INTERGER_H_
#define _PY_TYPE_LONG_INTERGER_H_

#include <boost/lexical_cast.hpp>
#include <iostream>

namespace PyExTypes
{
	template<typename T>
	struct LongInt : public boost::integer_arithmetic< LongInt<T> >
	{
		T m_val;

		explicit LongInt(T val):m_val(val){}
		operator T()const{return m_val;}
		
		static std::string MyClass__str__(const LongInt<T>  & self) 
		{
			return boost::lexical_cast<std::string>(self.m_val);
		}


		//////////////////////////////////////////////////////////////////////////
		template<typename RT>
		bool operator == (const RT & r)const
		{
			return m_val == T(r);
		}

		template<typename RT>
		bool operator != (const RT & r)const
		{
			return m_val != T(r);
		}

		template<typename RT>
		bool operator < (const RT & r)const
		{
			return m_val < T(r);
		}

		template<typename RT>
		bool operator > (const RT & r)const
		{
			return m_val > T(r);
		}

		template<typename RT>
		bool operator <= (const RT & r)const
		{
			return m_val <= T(r);
		}

		template<typename RT>
		bool operator >= (const RT & r)const
		{
			return m_val >= T(r);
		}

		//////////////////////////////////////////////////////////////////////////
		template<typename RT>
		LongInt<T> & operator += (const RT & r)
		{
			m_val += T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator -= (const RT & r)
		{
			m_val -= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator *= (const RT & r)
		{
			m_val *= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator /= (const RT & r)
		{
			m_val /= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator %= (const RT & r)
		{
			m_val %= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator >>= (const RT & r)
		{
			m_val >>= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator <<= (const RT & r)
		{
			m_val <<= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator &= (const RT & r)
		{
			m_val &= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator |= (const RT & r)
		{
			m_val |= T(r);
			return *this;
		}

		template<typename RT>
		LongInt<T> & operator ^= (const RT & r)
		{
			m_val ^= T(r);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		LongInt<T> operator + ()
		{
			return LongInt<T>(+ m_val);
		}

		LongInt<T> operator - ()
		{
			return LongInt<T>(- m_val);
		}

		LongInt<T> operator ~ ()
		{
			return LongInt<T>(~ m_val);
		}

		LongInt<T> operator ! ()
		{
			return LongInt<T>(! m_val);
		}
	};


	template<typename T,typename RT>
	LongInt<T> operator + (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val + T(b));
	}

	template<typename T,typename RT>
	LongInt<T> operator - (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val - T(b));
	}

	template<typename T,typename RT>
	LongInt<T> operator * (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val * T(b));
	}

	template<typename T,typename RT>
	LongInt<T> operator / (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val / T(b));
	}


	template<typename T,typename RT>
	LongInt<T> operator % (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val % T(b));
	}


	template<typename T,typename RT>
	LongInt<T> operator >> (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val >> T(b));
	}

	template<typename T,typename RT>
	LongInt<T> operator << (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val << T(b));
	}

	template<typename T,typename RT>
	LongInt<T> operator & (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val & T(b));
	}

	template<typename T,typename RT>
	LongInt<T> operator | (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val | T(b));
	}

	template<typename T,typename RT>
	LongInt<T> operator ^ (const LongInt<T> & a,const RT & b)
	{
		return LongInt<T>(a.m_val ^ T(b));
	}
}

#endif
