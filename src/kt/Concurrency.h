#pragma once
#include "kt.h"
#include "Platform.h"

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
	bool m_running = false;
};

class Event
{
public:
	enum class ResetType { Manual, Auto };

	Event(ResetType _reset = ResetType::Manual, bool _initialState = false);
	~Event();

	void Wait();
	void Wait(uint32_t _waitMillis);

	void Signal();
	void Reset();

private:
	void* m_event;
};

uint32_t LogicalCoreCount();

KT_FORCEINLINE void AcquireFence();
KT_FORCEINLINE void ReleaseFence();
KT_FORCEINLINE void AcquireReleaseFence();
KT_FORCEINLINE void MemoryFence();

KT_FORCEINLINE int8_t  AtomicLoad8(int8_t const* _ptr);
KT_FORCEINLINE int16_t AtomicLoad16(int16_t const* _ptr);
KT_FORCEINLINE int32_t AtomicLoad32(int32_t const* _ptr);
KT_FORCEINLINE int64_t AtomicLoad64(int64_t const* _ptr);

KT_FORCEINLINE void AtomicStore8(int8_t* _ptr, int8_t _val);
KT_FORCEINLINE void AtomicStore16(int16_t* _ptr, int16_t _val);
KT_FORCEINLINE void AtomicStore32(int32_t* _ptr, int32_t _val);
KT_FORCEINLINE void AtomicStore64(int64_t* _ptr, int64_t _val);

KT_FORCEINLINE int8_t  AtomicFetchAdd8(int8_t* _ptr, int8_t _v);
KT_FORCEINLINE int16_t AtomicFetchAdd16(int16_t* _ptr, int16_t _v);
KT_FORCEINLINE int32_t AtomicFetchAdd32(int32_t* _ptr, int32_t _v);
KT_FORCEINLINE int64_t AtomicFetchAdd64(int64_t* _ptr, int64_t _v);

KT_FORCEINLINE int16_t AtomicCompareAndSwap16(int16_t* _ptr, int16_t _old, int16_t _new);
KT_FORCEINLINE int32_t AtomicCompareAndSwap32(int32_t* _ptr, int32_t _old, int32_t _new);
KT_FORCEINLINE int64_t AtomicCompareAndSwap64(int64_t* _ptr, int64_t _old, int64_t _new);

KT_FORCEINLINE uint8_t  AtomicLoad8(uint8_t const* _ptr);
KT_FORCEINLINE uint16_t AtomicLoad16(uint16_t const* _ptr);
KT_FORCEINLINE uint32_t AtomicLoad32(uint32_t const* _ptr);
KT_FORCEINLINE uint64_t AtomicLoad64(uint64_t const* _ptr);

KT_FORCEINLINE void AtomicStore8(uint8_t* _ptr, uint8_t _val);
KT_FORCEINLINE void AtomicStore16(uint16_t* _ptr, uint16_t _val);
KT_FORCEINLINE void AtomicStore32(uint32_t* _ptr, uint32_t _val);
KT_FORCEINLINE void AtomicStore64(uint64_t* _ptr, uint64_t _val);

KT_FORCEINLINE uint8_t  AtomicFetchAdd8(uint8_t* _ptr, uint8_t _v);
KT_FORCEINLINE uint16_t AtomicFetchAdd16(uint16_t* _ptr, uint16_t _v);
KT_FORCEINLINE uint32_t AtomicFetchAdd32(uint32_t* _ptr, uint32_t _v);
KT_FORCEINLINE uint64_t AtomicFetchAdd64(uint64_t* _ptr, uint64_t _v);

KT_FORCEINLINE uint16_t AtomicCompareAndSwap16(uint16_t* _ptr, uint16_t _old, uint16_t _new);
KT_FORCEINLINE uint32_t AtomicCompareAndSwap32(uint32_t* _ptr, uint32_t _old, uint32_t _new);
KT_FORCEINLINE uint64_t AtomicCompareAndSwap64(uint64_t* _ptr, uint64_t _old, uint64_t _new);

template <typename T>
KT_FORCEINLINE T* AtomicCompareAndSwapPtr(T** _ptr, T* _old, T* _new);

template <typename T>
KT_FORCEINLINE T* AtomicLoadPtr(T* const* _ptr);

template <typename T>
KT_FORCEINLINE void AtomicStorePtr(T** _ptr, T* _val);


}

#if KT_COMPILER_MSVC
	#include "inl/AtomicImpl_MSVC.inl"
#elif (KT_COMPILER_GCC || KT_COMPILER_CLANG)
	#include "inl/AtomicImpl_GCC.inl"
#else
	#error Not implemented.
#endif

#include "inl/AtomicImpl_Generic.inl"