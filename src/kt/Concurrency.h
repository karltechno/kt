#pragma once
#include "kt.h"
#include "Platform.h"
#include <atomic>

namespace kt
{

class Mutex
{
public:
	KT_NO_COPY(Mutex);

	Mutex();
	~Mutex();

	bool TryLock();

	void Lock();
	void Unlock();

private:
#if KT_PLATFORM_WINDOWS || KT_PLATFORM_POSIX
	KT_ALIGNAS(8) char m_mutexData[40];
#endif
};

template <typename T>
struct ScopedLock
{
	KT_NO_COPY(ScopedLock);

	ScopedLock(T& _t) : lk(_t) { lk.Lock(); }
	~ScopedLock() { lk.Unlock(); }

private:
	T& lk;
};

class Thread
{
public:
	KT_NO_COPY(Thread);

	typedef void(*Entry)(Thread*);

	Thread() = default;
	~Thread();

	void Run(Entry _fn, void* _userData, char const* _name = nullptr);

	bool IsRunning() const;
	void Join();
	Entry GetEntry() const { return m_entry; }

	void* GetUserData() const { return m_userData; }

	static void InternalRun(Thread* _pThis);

private:
	Entry m_entry = nullptr;
	void* m_userData = nullptr;
	void* m_threadHandle = nullptr;
	std::atomic<uint32_t> m_running = false;
};

class Event
{
public:
	KT_NO_COPY(Event);

	Event(bool _initialState = false);
	~Event();

	void Wait(uint32_t _waitMillis = 0xFFFFFFFF);
	void Signal();

	void ManualReset();

private:
	void* m_event;
};

uint32_t LogicalCoreCount();

#if KT_ARCH_64BIT
template <typename T>
bool AtomicCompareExchange128(T* _v, T* io_expAndOld, T const& _new);
#endif

}

#include "inl/Concurrency.inl"