#pragma once
#include "../kt.h"
#include "../Concurrency.h"

extern "C" void _ReadBarrier();
#pragma intrinsic(_ReadBarrier)

extern "C" void _WriteBarrier();
#pragma intrinsic(_WriteBarrier)

extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)

extern "C" long _InterlockedExchangeAdd(long volatile* _ptr, long _value);
#pragma intrinsic(_InterlockedExchangeAdd)

extern "C" long _InterlockedCompareExchange(long volatile* _ptr, long _exchange, long _comparand);
#pragma intrinsic(_InterlockedCompareExchange)

extern "C" __int64 _InterlockedCompareExchange64(__int64 volatile* _ptr, __int64 _exchange, __int64 _comparand);
#pragma intrinsic(_InterlockedCompareExchange64)

#if KT_ARCH_64BIT
	extern "C" unsigned char _InterlockedCompareExchange128(__int64 volatile* _dest, __int64 exchHigh, __int64 exchLo, __int64* compareAndResult);
	#pragma intrinsic(_InterlockedCompareExchange128)
#endif

namespace kt
{

KT_FORCEINLINE void AcquireFence()
{
	::_ReadWriteBarrier();
}

KT_FORCEINLINE void ReleaseFence()
{
	::_ReadWriteBarrier();
}

KT_FORCEINLINE void AcquireReleaseFence()
{
	::_ReadWriteBarrier();
}

KT_FORCEINLINE void MemoryFence()
{
	_mm_mfence();
}

KT_FORCEINLINE int8_t AtomicLoad8(int8_t const* _ptr)
{
	int8_t const ret = *(int8_t volatile*)_ptr;
	return ret;
}

KT_FORCEINLINE int16_t AtomicLoad16(int16_t const* _ptr)
{
	int16_t const ret = *(int16_t volatile*)_ptr;
	return ret;
}

KT_FORCEINLINE int32_t AtomicLoad32(int32_t const* _ptr)
{
	int32_t const ret = *(int32_t volatile*)_ptr;
	return ret;
}

#if KT_ARCH_64BIT
KT_FORCEINLINE int64_t AtomicLoad64(int64_t const* _ptr)
{
	int64_t const ret = *(int64_t volatile*)_ptr;
	return ret;
}
#else
#error Todo 32 bit load here.
#endif


KT_FORCEINLINE void AtomicStore8(int8_t* _ptr, int8_t _val)
{
	*(int8_t volatile*)_ptr = _val;
}

KT_FORCEINLINE void AtomicStore16(int16_t* _ptr, int16_t _val)
{
	*(int16_t volatile*)_ptr = _val;
}

KT_FORCEINLINE void AtomicStore32(int32_t* _ptr, int32_t _val)
{
	*(int32_t volatile*)_ptr = _val;
}

#if KT_ARCH_64BIT
KT_FORCEINLINE void AtomicStore64(int64_t* _ptr, int64_t _val)
{
	*(int64_t volatile*)_ptr = _val;
}
#else
#error Todo 32 bit store here.
#endif

KT_FORCEINLINE int8_t  AtomicFetchAdd8(int8_t* _ptr, int8_t _v)
{
	return _InterlockedExchangeAdd8((volatile char*)_ptr, _v);
}

KT_FORCEINLINE int16_t AtomicFetchAdd16(int16_t* _ptr, int16_t _v)
{
	return _InterlockedExchangeAdd16((int16_t volatile*)_ptr, _v);
}

KT_FORCEINLINE int32_t AtomicFetchAdd32(int32_t* _ptr, int32_t _v)
{
	return _InterlockedExchangeAdd((long volatile*)_ptr, _v);
}

KT_FORCEINLINE int64_t AtomicFetchAdd64(int64_t* _ptr, int64_t _v)
{
	return _InterlockedExchangeAdd64((int64_t volatile*)_ptr, _v);
}

KT_FORCEINLINE int16_t AtomicCompareAndSwap16(int16_t* _ptr, int16_t _old, int16_t _new)
{
	return _InterlockedCompareExchange16((short volatile*)_ptr, _new, _old);
}

KT_FORCEINLINE int32_t AtomicCompareAndSwap32(int32_t* _ptr, int32_t _old, int32_t _new)
{
	return _InterlockedCompareExchange((long volatile*)_ptr, _new, _old);
}

KT_FORCEINLINE int64_t AtomicCompareAndSwap64(int64_t* _ptr, int64_t _old, int64_t _new)
{
	return _InterlockedCompareExchange64((__int64 volatile*)_ptr, _new, _old);
}

}