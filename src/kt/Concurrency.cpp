#include "Concurrency.h"

#if KT_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif KT_PLATFORM_POSIX
	#include <pthread.h>
#endif

namespace kt
{
#if KT_PLATFORM_WINDOWS
	#define GET_MUTEX(_mu) ((CRITICAL_SECTION*)_mu)
#elif KT_PLATFORM_POSIX
	#define GET_MUTEX(_mu) ((pthread_mutex_t*)_mu)
#endif

Mutex::Mutex()
{
#if KT_PLATFORM_WINDOWS
	static_assert(sizeof(m_mutexData) >= sizeof(CRITICAL_SECTION), "Sizeof mutex issue");
	::InitializeCriticalSectionAndSpinCount(GET_MUTEX(this), 4000u);
#elif KT_PLATFORM_POSIX
	static_assert(sizeof(m_mutexData) >= sizeof(pthread_mutex_t), "Sizeof mutex issue");
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(GET_MUTEX(this), &attr);
#endif
}

Mutex::~Mutex()
{
#if KT_PLATFORM_WINDOWS
	::DeleteCriticalSection(GET_MUTEX(this));
#elif KT_PLATFORM_POSIX
	pthread_mutex_destroy(GET_MUTEX(this));
#else
#error Not implemented
#endif
}

bool Mutex::TryLock()
{
#if KT_PLATFORM_WINDOWS
	return ::TryEnterCriticalSection(GET_MUTEX(this));
#elif KT_PLATFORM_POSIX
	return pthread_mutex_trylock(GET_MUTEX(this)) == 0;
#else
#error Not implemented
#endif
}

void Mutex::Lock()
{
#if KT_PLATFORM_WINDOWS
	return ::EnterCriticalSection(GET_MUTEX(this));
#elif KT_PLATFORM_POSIX
	pthread_mutex_lock(GET_MUTEX(this));
#else
#error Not implemented
#endif
}

void Mutex::Unlock()
{
#if KT_PLATFORM_WINDOWS
	return ::LeaveCriticalSection(GET_MUTEX(this));
#elif KT_PLATFORM_POSIX
	pthread_mutex_unlock(GET_MUTEX(this));
#else
#error Not implemented
#endif
}


#if KT_PLATFORM_WINDOWS
static DWORD WINAPI Win32ThreadEntry(void* _param)
{
	Thread* t = (Thread*)_param;
	t->InternalRun(t);
	return 0;
}

static void NameThreadWin32(char const* _name, HANDLE _thread)
{
	const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)  
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; 
		LPCSTR szName;
		DWORD dwThreadID;   
		DWORD dwFlags;
	} THREADNAME_INFO;
#pragma pack(pop)  
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = _name;
	info.dwThreadID = ::GetThreadId(_thread);
	info.dwFlags = 0;
#pragma warning(push)  
#pragma warning(disable: 6320 6322)  
	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
#pragma warning(pop)  

}
#endif

#if KT_PLATFORM_POSIX
void* POSIXThreadEntry(void* _param)
{
	Thread* t = (Thread*)_param;
	t->InternalRun(t);
	return 0;
}
#endif

Thread::~Thread()
{
	Join();
}

void Thread::Run(Entry _fn, void* _userData, char const* _name)
{
	KT_UNUSED(_name); // todo on posix

	KT_ASSERT(!m_threadHandle);
	m_userData = _userData;
	m_entry = _fn;
#if KT_PLATFORM_WINDOWS
	m_threadHandle = ::CreateThread(nullptr, 0, Win32ThreadEntry, this, 0, 0);
	KT_ASSERT(m_threadHandle);
	if (_name)
	{
		NameThreadWin32(_name, m_threadHandle);
	}
#elif KT_PLATFORM_POSIX
	pthread_attr_t attr;
	int res = pthread_attr_init(&attr);
	KT_ASSERT(res == 0);
	pthread_create((pthread_t*)&m_threadHandle, &attr, POSIXThreadEntry, this);
#else
#error Not implemented
#endif
}

bool Thread::IsRunning() const
{
	return std::atomic_load_explicit(&m_running, std::memory_order_relaxed);
}

void Thread::Join()
{
	if (!m_threadHandle)
	{
		return;
	}
#if KT_PLATFORM_WINDOWS
	HRESULT const res =::WaitForSingleObject(m_threadHandle, INFINITE);
	KT_ASSERT(res == WAIT_OBJECT_0);
	::CloseHandle(m_threadHandle);
	m_threadHandle = nullptr;
#elif KT_PLATFORM_POSIX
	void* er;
	pthread_join(*(pthread_t*)&m_threadHandle, &er);
	KT_ASSERT(er == 0);
	m_threadHandle = nullptr;
#else
#error Not implemented
#endif
}

void Thread::InternalRun(Thread* _pThis)
{
	std::atomic_store_explicit(&_pThis->m_running, 1, std::memory_order_relaxed);
	_pThis->GetEntry()(_pThis);
	std::atomic_store_explicit(&_pThis->m_running, 0, std::memory_order_release);
}

uint32_t LogicalCoreCount()
{
#if KT_PLATFORM_WINDOWS
	SYSTEM_INFO sysinfo;
	::GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#endif
}

Event::Event(bool _initialState /*= false*/)
{
#if KT_PLATFORM_WINDOWS
	m_event = ::CreateEvent(nullptr, FALSE, _initialState, nullptr);
	KT_ASSERT(m_event);
#endif
}

Event::~Event()
{
#if KT_PLATFORM_WINDOWS
	BOOL ok = ::CloseHandle(m_event);
	KT_UNUSED(ok);
	KT_ASSERT(ok != 0);
#endif
}

void Event::Wait(uint32_t _waitMillis)
{
#if KT_PLATFORM_WINDOWS
	::WaitForSingleObject(m_event, _waitMillis);
#endif
}

void Event::Signal()
{
#if KT_PLATFORM_WINDOWS
	BOOL ok = ::SetEvent(m_event);
	KT_UNUSED(ok);
	KT_ASSERT(ok != 0);
#endif
}

void Event::ManualReset()
{

}

}

