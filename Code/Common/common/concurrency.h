
#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include <string>

void memTrackerThreadFinish();

typedef void (* SimpleThreadFunc)(void*);
/**
 * Helper structure for SimpleThread on both windows and linux.
 */
struct ThreadTrampolineInfo
{
	ThreadTrampolineInfo( SimpleThreadFunc func, void * arg )
	{
		this->func = func;
		this->arg = arg;
	}
	SimpleThreadFunc func;
	void * arg;
};


/**
 *  Dummy mutex class that doesn't actually do anything.
 */
class DummyMutex
{
public:
	void grab() {}
	bool grabTry() { return true; }
	void give() {}
};

#ifdef _WIN32

#ifndef _XBOX360
#include "windows.h"
#else
#include <xtl.h>
#endif

#include <process.h>

inline unsigned long OurThreadID()
{
	return (unsigned long)(GetCurrentThreadId());
}

inline uint getNumCores()
{
	static uint count;

	if ( !count )
	{
		DWORD processAffinity;
		DWORD systemAffinity;
		HRESULT res = GetProcessAffinityMask( GetCurrentProcess(), &processAffinity, &systemAffinity );
		if ( !SUCCEEDED( res ) )
		{
			Log_ErrorF( "Could not get process affinity mask\n" );
			return 1;
		}

		for ( uint i = 0; i < 32; i++ )
		{
			count += ( processAffinity & ( 1 << i ) ) ? 1 : 0;
		}
	}

	return count;
}


/**
 *	This class is a simple mutex.
 *	Note that critical sections are reentrant for the same thread, but this
 *	mutex is not reentrant and will raise an assert if reentered by the same
 *	thread.
 */
class SimpleMutex
{
public:
	SimpleMutex() : gone_( false ) { InitializeCriticalSection( &mutex_ ); }
	~SimpleMutex()	{ DeleteCriticalSection( &mutex_ ); }

	void grab()		{ EnterCriticalSection( &mutex_ ); MY_ASSERT( !gone_ ); gone_ = true; }
	void give()		{ gone_ = false; LeaveCriticalSection( &mutex_ ); }
	bool grabTry()
	{
		if (TryEnterCriticalSection( &mutex_ ))
		{
			// Reentered by same thread
			// Did you forget to give()?
			MY_ASSERT( !gone_ );
			gone_ = true;
			return true;
		}
		return false;
	};

private:
	CRITICAL_SECTION	mutex_;
	bool				gone_;
};


/**
 *	This class is a simple semaphore
 */
class SimpleSemaphore
{
public:
	SimpleSemaphore() : sema_( CreateSemaphore( NULL, 0, 32767, NULL ) ) { }
	~SimpleSemaphore()	{ CloseHandle( sema_ ); }

	void push()			{ ReleaseSemaphore( sema_, 1, NULL ); }
	void pull()			{ WaitForSingleObject( sema_, INFINITE ); }
	bool pullTry()		{ return WaitForSingleObject( sema_, 0 ) == WAIT_OBJECT_0; }

private:
	HANDLE sema_;
};


/**
 *	This class implements a read-write lock
 */
class ReadWriteLock
{
	SimpleMutex lock_;
	HANDLE readEvent_;
	HANDLE writeEvent_;
	volatile LONG readCount_;

public:
	ReadWriteLock()
	{
		readEvent_ = CreateEvent( NULL, TRUE, FALSE, NULL );
		writeEvent_ = CreateEvent( NULL, FALSE, TRUE, NULL );
		readCount_ = 0;
	}

	~ReadWriteLock()
	{
		CloseHandle( readEvent_ );
		CloseHandle( writeEvent_ );
	}

	void beginRead()
	{
		if (InterlockedIncrement( &readCount_ ) == 1)
		{
			WaitForSingleObject( writeEvent_, INFINITE );
			SetEvent( readEvent_ );
		}

		WaitForSingleObject( readEvent_, INFINITE );
	}

	void endRead()
	{
		if (InterlockedDecrement( &readCount_ ) == 0)
		{
			ResetEvent( readEvent_ );
			SetEvent( writeEvent_ );
		}
	}

	void beginWrite()
	{
		lock_.grab();
		WaitForSingleObject( writeEvent_, INFINITE );
	}

	void endWrite()
	{
		SetEvent( writeEvent_ );
		lock_.give();
	}

	class ReadGuard
	{
		ReadWriteLock& rwl_;

	public:
		ReadGuard( ReadWriteLock& rwl )
			: rwl_( rwl )
		{
			rwl_.beginRead();
		}

		~ReadGuard()
		{
			rwl_.endRead();
		}
	};

	class WriteGuard
	{
		ReadWriteLock& rwl_;

	public:
		WriteGuard( ReadWriteLock& rwl )
			: rwl_( rwl )
		{
			rwl_.beginWrite();
		}

		~WriteGuard()
		{
			rwl_.endWrite();
		}
	};
};


/**
 *  This class is a simple thread
 */
class SimpleThread
{
public:
	SimpleThread()
		: thread_( NULL )
	{}

	SimpleThread(
		SimpleThreadFunc threadfunc,
		void * arg )
	{
		init( threadfunc, arg, "Unnamed thread" );
	}

	SimpleThread(
		SimpleThreadFunc threadfunc,
		void * arg,
		const std::string& threadName )
	{
		init( threadfunc, arg, threadName );
	}

	~SimpleThread()
	{
		WaitForSingleObject( thread_, INFINITE );
		CloseHandle( thread_ );
	}

	void init(
		SimpleThreadFunc threadfunc,
		void * arg )
	{
		init( threadfunc, arg, "Unknown simple thread" );
	}

	void init(
		SimpleThreadFunc threadfunc,
		void * arg,
		const std::string& threadName )
	{
		ThreadTrampolineInfo * info = new ThreadTrampolineInfo( threadfunc, arg );
		unsigned threadId;
		thread_ = HANDLE( _beginthreadex( 0, 0, trampoline, info, 0, &threadId ) );
		if ( thread_ )
		{
#ifndef CONSUMER_RELEASE
			SetThreadName( threadId, threadName.c_str() );
#endif
		}
		else
		{
			Log_ErrorF( "Could not create new SimpleThread\n" );
		}
	}

	struct AutoHandle	// Ugly!!
	{
		AutoHandle( HANDLE h ) { HANDLE p = GetCurrentProcess();
			DuplicateHandle( p, h, p, &real_, 0, TRUE, 0 ); }
		~AutoHandle() { CloseHandle( real_ ); }
		operator HANDLE() const { return real_; }
		HANDLE real_;
	};

	HANDLE handle() const	{ return thread_; }	// exposed for more complex ops
	static AutoHandle current()	{ return AutoHandle( GetCurrentThread() ); }

private:
	HANDLE thread_;

	/*
	 * this trampoline function is present so that we can hide the fact that windows
	 * and linux expect different function signatures for thread functions
	 */
	static unsigned __stdcall trampoline( void * arg )
	{
		ThreadTrampolineInfo * info = static_cast<ThreadTrampolineInfo*>(arg);

		info->func(info->arg);
		memTrackerThreadFinish();

		SAFE_DELETE(info);
		return 0;
	}

#ifndef CONSUMER_RELEASE
	// SetThreadName taken from
	// http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
	static const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
	   DWORD dwType; // Must be 0x1000.
	   LPCSTR szName; // Pointer to name (in user addr space).
	   DWORD dwThreadID; // Thread ID (-1=caller thread).
	   DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
#pragma pack(pop)

	static void SetThreadName( DWORD dwThreadID, const char* threadName )
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;

		__try
		{
			RaiseException(
				MS_VC_EXCEPTION,
				0,
				sizeof( info ) / sizeof( ULONG_PTR ),
				( ULONG_PTR* )&info );
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
		}
#endif
	}
};


#if BWCLIENT_AS_PYTHON_MODULE

int tlsGetOffSet(int size);
char * tlsGetData();
void tlsIncRef();
void tlsDecRef();

const char   c_initFalse   = 0;
const char   c_initTrue    = 1;

/**
 *	You are not allowed to use __declspec( thread ) when building
 *	a DLL. The TLS API is the way to go if you require thread local
 *	storage. The ThreadLocal template class encapsulates the
 *	underlying TLS implementation and can be used in a way very
 *	similar to __declspec( thread ).
 */
template<typename T>
class ThreadLocal
{
public:
	ThreadLocal();
	ThreadLocal(const T & value);

	~ThreadLocal();

	T & get();
	const T & get() const;

	void set(const T & value);

	operator T & ()
	{
		return get();
	}

	T & operator -> ()
	{
		return get();
	}

	T* operator&()
	{
		return &(get());
	}

	const T & operator = ( const T & other )
	{
		this->set(other);
		return this->get();
	}

	bool operator == ( const T & other ) const
	{
		return this->get() == other;
	}

	bool operator != ( const T & other ) const
	{
		return this->get() != other;
	}

	const T & operator ++ ()
	{
		return ++this->get();
	}

	T operator ++ (int)
	{
		return this->get()++;
	}

	const T & operator--()
	{
		return --this->get();
	}

	T operator--(int)
	{
		return this->get()--;
	}

	const T & operator+=( const T & inc )
	{
		return this->get() += inc;
	}

	const T & operator-=( const T & dec )
	{
		return this->get() -= dec;
	}

private:
	int dataOffset_;
	T   initValue_;
};

template<typename T>
ThreadLocal<T>::ThreadLocal()
{
	this->dataOffset_ = tlsGetOffSet(sizeof(T) + 1);
	tlsIncRef();
}

template<typename T>
ThreadLocal<T>::ThreadLocal(const T & value)
{
	char * tlsData    = tlsGetData();
	this->dataOffset_ = tlsGetOffSet(sizeof(T) + 1);
	this->initValue_  = value;
	this->get();
	tlsIncRef();
}

template<typename T>
ThreadLocal<T>::~ThreadLocal()
{
	tlsDecRef();
}

template<typename T>
T & ThreadLocal<T>::get()
{
	char * tlsChar = tlsGetData();
	char * initialised = tlsChar + this->dataOffset_;
	T & value = *(T*)(tlsChar + this->dataOffset_ + 1);
	if (*initialised == c_initFalse)
	{
		value = this->initValue_;
		*initialised = c_initTrue;
	}
	return value;
}

template<typename T>
const T & ThreadLocal<T>::get() const
{
	return const_cast<ThreadLocal<T>*>(this)->get();
}

template<typename T>
void ThreadLocal<T>::set(const T & value)
{
	T & data = this->get();
	data = value;
}

// Reflect == to use the version in the ThreadLocal class template
template<typename T>
int operator== (const T & left, const ThreadLocal<T> & right)
{
	return right == left;
}

template<typename T>
std::ostream& operator<<(std::ostream & o, const ThreadLocal<T> & t)
{
	o << t.get();
	return o;
}

// Using "ThreadLocal<type>" as opposed to "__declspec( thread ) type" occurs
// in approximatly a 20% performance penalty. But because thread local variables
// are rarely used in time critical code this shouldn't be a problem. Anyway,
// "ThreadLocal<type>" is only used when building <bwclient>.dll so this will
// never be a problem in a production release.
#define THREADLOCAL(type) ThreadLocal<type>

#else // BWCLIENT_AS_PYTHON_MODULE

/// thread local declaration - ie "static THREADLOCAL(type) blah"
#if !defined(MF_SINGLE_THREADED)
	#define THREADLOCAL(type) __declspec( thread ) type
#else
	#define THREADLOCAL(type) type
#endif

#endif // BWCLIENT_AS_PYTHON_MODULE


#else // !_WIN32

#ifndef MF_SINGLE_THREADED
#include <pthread.h>
#include <errno.h>

/**
 *	This class is a simple mutex
 */
class SimpleMutex
{
public:
	SimpleMutex()	{ pthread_mutex_init( &mutex_, NULL); }
	~SimpleMutex()	{ pthread_mutex_destroy( &mutex_ ); }

	void grab()		{ pthread_mutex_lock( &mutex_ ); }
	bool grabTry()	{ return (pthread_mutex_trylock( &mutex_ ) == 0); }
	void give()		{ pthread_mutex_unlock( &mutex_ ); }

private:
	pthread_mutex_t mutex_;
};


/**
 *	This class is a simple semaphore, implemented with a condition variable
 */
class SimpleSemaphore
{
public:
	SimpleSemaphore() :
		value_( 0 )
	{
		pthread_mutex_init( &lock_, NULL );
		pthread_cond_init( &wait_, NULL );
	}
	~SimpleSemaphore()
	{
		pthread_cond_destroy( &wait_ );
		pthread_mutex_destroy( &lock_ );
	}

	void pull()
	{
		pthread_mutex_lock( &lock_ );
		value_--;
		if (value_ < 0)
			pthread_cond_wait( &wait_, &lock_ );
		pthread_mutex_unlock( &lock_ );
	}
	bool pullTry()
	{
		bool gotit = true;
		pthread_mutex_lock( &lock_ );
		if (value_ <= 0)
			gotit = false;
		else
			value_--;
		pthread_mutex_unlock( &lock_ );
		return gotit;
	}
	void push()
	{
		pthread_mutex_lock( &lock_ );
		value_++;
		if (value_ <= 0)
			pthread_cond_signal( &wait_ );
		pthread_mutex_unlock( &lock_ );
	}

private:
	pthread_mutex_t	lock_;
	pthread_cond_t	wait_;
	int			value_;
};


/**
 *	This class implements a read-write lock
 */
class ReadWriteLock
{
	pthread_rwlock_t lock_;

public:
	ReadWriteLock()
	{
		MY_ASSERT( pthread_rwlock_init( &lock_, NULL ) == 0 );
	}

	~ReadWriteLock()
	{
		MY_ASSERT( pthread_rwlock_destroy( &lock_ ) == 0 );
	}

	void beginRead()
	{
		MY_ASSERT( pthread_rwlock_rdlock( &lock_ ) == 0 );
	}

	void endRead()
	{
		MY_ASSERT( pthread_rwlock_unlock( &lock_ ) == 0 );
	}

	void beginWrite()
	{
		MY_ASSERT( pthread_rwlock_wrlock( &lock_ ) == 0 );
	}

	void endWrite()
	{
		MY_ASSERT( pthread_rwlock_unlock( &lock_ ) == 0 );
	}

	class ReadGuard
	{
		ReadWriteLock& rwl_;

	public:
		ReadGuard( ReadWriteLock& rwl )
			: rwl_( rwl )
		{
			rwl_.beginRead();
		}

		~ReadGuard()
		{
			rwl_.endRead();
		}
	};

	class WriteGuard
	{
		ReadWriteLock& rwl_;

	public:
		WriteGuard( ReadWriteLock& rwl )
			: rwl_( rwl )
		{
			rwl_.beginWrite();
		}

		~WriteGuard()
		{
			rwl_.endWrite();
		}
	};
};

inline unsigned long OurThreadID()
{
	return (unsigned long)(pthread_self());
}

/**
 *  This class is a simple thread
 */
class SimpleThread
{
public:
	SimpleThread() : thread_( 0 )
	{
	}

	SimpleThread(
		SimpleThreadFunc threadfunc,
		void * arg )
	{
		init( threadfunc, arg, "Unnamed thread" );
	}

	SimpleThread(
		SimpleThreadFunc threadfunc,
		void * arg,
		const std::string& threadName )
	{
		init( threadfunc, arg, threadName );
	}

	void init( SimpleThreadFunc threadfunc,
			   void * arg,
			   const std::string& threadName )
	{
		ThreadTrampolineInfo * info = new ThreadTrampolineInfo( threadfunc, arg );
		pthread_create( &thread_, NULL, trampoline, info );
	}
	~SimpleThread()
	{
		pthread_join( thread_, NULL );
	}

	pthread_t handle() const	{ return thread_; }
	static pthread_t current()	{ return pthread_self(); }

private:
	pthread_t thread_;

	/*
	 * this trampoline function is present so that we can hide the fact that windows
	 * and linux expect different function signatures for thread functions
	 */

	static void * trampoline( void * arg )
	{
		ThreadTrampolineInfo * info = static_cast<ThreadTrampolineInfo*>(arg);
		info->func( info->arg );
		SAFE_DELETE(info);
		return NULL;
	}
};
#else // MF_SINGLE_THREADED

/**
 *	No-op version of SimpleMutex.
 */
class SimpleMutex
{
public:
	SimpleMutex() {}
	~SimpleMutex() {}

	void grab() {}
	void give() {}
	bool grabTry() { return true; }
};

/**
 *	No-op version of SimpleSemaphore.
 */
class SimpleSemaphore
{
public:
	SimpleSemaphore() : value_( 0 ) {}
	~SimpleSemaphore() {}

	void push() { value_++; }
	void pull() { value_--; }
	bool pullTry()
	{
		if (value_ <= 0)
			return false;
		value_--;
		return true;
	}

private:
	int			value_;
};


/**
 *	No-op version of ReadWriteLock
 */
class ReadWriteLock
{
public:
	ReadWriteLock() {}
	~ReadWriteLock() {}

	void beginRead() {}
	void endRead() {}
	void beginWrite() {}
	void endWrite() {}

	class ReadGuard
	{
	public:
		ReadGuard( ReadWriteLock& rwl ) {}
		~ReadGuard() {}
	};

	class WriteGuard
	{
	public:
		WriteGuard( ReadWriteLock& rwl ) {}
		~WriteGuard() {}
	};
};

inline unsigned long OurThreadID()
{
	return 0;
}

/**
 *	No-op version of SimpleThread.
 */
class SimpleThread
{
public:

	SimpleThread() {}

	SimpleThread(
		SimpleThreadFunc threadfunc,
		void * arg )
	{
		init( threadfunc, arg, "Unnamed thread" );
	}

	SimpleThread(
		SimpleThreadFunc threadfunc,
		void * arg,
		const std::string& threadName )
	{
		this->init( threadfunc, arg, threadName );
	}

	void init(
		SimpleThreadFunc threadfunc,
		void * arg,
		const std::string& threadName )
	{
		CRITICAL_MSG( "SimpleThread being initialised when "
			"MF_SINGLE_THREADED!" );
		// threadfunc( arg );
	}


	~SimpleThread(){}

	int handle() const { return 0; }
	static int current() { return 0; }

};
#endif // MF_SINGLE_THREADED

/// thread local declaration - ie "static THREADLOCAL(type) blah"
#ifndef MF_SINGLE_THREADED
# define THREADLOCAL(type) __thread type
#else
# define THREADLOCAL(type) type
#endif

#endif // _WIN32

/**
 *	This class grabs and holds a mutex for as long as it is around
 */
class SimpleMutexHolder
{
public:
	SimpleMutexHolder( SimpleMutex & sm ) : sm_( sm )	{ sm_.grab(); }
	~SimpleMutexHolder()								{ sm_.give(); }
private:
	SimpleMutex & sm_;
};

// Temporary fix to enable compilation under linux for pedantic compilers 
#ifdef _WIN32
 #define ARGTYPE  uint64
#else
 #define ARGTYPE  intptr_t
#endif

/**
 *	This class is a matrix mutex holder
 */
class MatrixMutexHolder
{
	static const int MUTEX_MATRIX_SIZE = 271;
	static SimpleMutex mutex_[MUTEX_MATRIX_SIZE];
	static const unsigned long INVALID_THREAD_ID = 0;
	static volatile unsigned long threadId_[MUTEX_MATRIX_SIZE];
	void grab( ARGTYPE hashInput )
	{
#ifdef _WIN32
		uint64 ui = hashInput;
#else
		uint64 ui = (int)hashInput;
#endif
		index_ = ((ui * ui) >> 3) % MUTEX_MATRIX_SIZE;
		grab_ = threadId_[ index_ ] != OurThreadID();
		if( grab_ )
		{
			mutex_[ index_ ].grab();
			threadId_[ index_ ] = OurThreadID();
		}
	}
	ARGTYPE index_;
	bool grab_;
public:
	MatrixMutexHolder( const void* p )
	{
		grab( (ARGTYPE)p );
	}
	MatrixMutexHolder( unsigned int i )
	{
		grab( i );
	}
	~MatrixMutexHolder()
	{
		if( grab_ )
		{
			threadId_[ index_ ] = INVALID_THREAD_ID;
			mutex_[ index_ ].give();
		}
	}
};

#undef ARGTYPE

extern void (*pMainThreadIdleStartFunc)();
extern void (*pMainThreadIdleEndFunc)();

namespace BWConcurrency
{
inline void startMainThreadIdling()
{
	(*pMainThreadIdleStartFunc)();
}

inline void endMainThreadIdling()
{
	(*pMainThreadIdleEndFunc)();
}

inline
void setMainThreadIdleFunctions( void (*pStartFunc)(), void (*pEndFunc)() )
{
	pMainThreadIdleStartFunc = pStartFunc;
	pMainThreadIdleEndFunc = pEndFunc;
}

}

#endif // CONCURRENCY_HPP
