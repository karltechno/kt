#pragma once
#include "../kt.h"
#include "../Concurrency.h"

#if (KT_COMPILER_GCC || KT_COMPILER_CLANG)

KT_FORCEINLINE void AcquireFence()
{
	asm volatile("" ::: "memory");
}

KT_FORCEINLINE void ReleaseFence()
{
	asm volatile("" ::: "memory");
}

KT_FORCEINLINE void AcquireReleaseFence()
{
	asm volatile("" ::: "memory");
}

KT_FORCEINLINE void MemoryFence()
{
	__sync_synchronize();
}

namespace kt
{

KT_FORCEINLINE int8_t AtomicLoad8(int8_t* _ptr)
{
	return __atomic_load_n(_ptr, __ATOMIC_RELAXED);
}

KT_FORCEINLINE int16_t AtomicLoad16(int16_t* _ptr)
{
	return __atomic_load_n(_ptr, __ATOMIC_RELAXED);
}

KT_FORCEINLINE int32_t AtomicLoad32(int32_t* _ptr)
{
	return __atomic_load_n(_ptr, __ATOMIC_RELAXED);
}

KT_FORCEINLINE int64_t AtomicLoad64(int64_t* _ptr)
{
	return __atomic_load_n(_ptr, __ATOMIC_RELAXED);
}

KT_FORCEINLINE void AtomicStore8(int8_t* _ptr, int8_t _val)
{
	__atomic_store_n(_ptr, _val, __ATOMIC_RELAXED);
}

KT_FORCEINLINE void AtomicStore16(int16_t* _ptr, int16_t _val)
{
	__atomic_store_n(_ptr, _val, __ATOMIC_RELAXED);
}

KT_FORCEINLINE void AtomicStore32(int32_t* _ptr, int32_t _val)
{
	__atomic_store_n(_ptr, _val, __ATOMIC_RELAXED);
}

KT_FORCEINLINE void AtomicStore64(int64_t* _ptr, int64_t _val)
{
	__atomic_store_n(_ptr, _val, __ATOMIC_RELAXED);
}

KT_FORCEINLINE int8_t  AtomicFetchAdd8(int8_t* _ptr, int8_t _v)
{
	return __sync_fetch_and_add(_ptr, _v);
}

KT_FORCEINLINE int16_t AtomicFetchAdd16(int16_t* _ptr, int16_t _v)
{
	return __sync_fetch_and_add(_ptr, _v);
}

KT_FORCEINLINE int32_t AtomicFetchAdd32(int32_t* _ptr, int32_t _v)
{
	return __sync_fetch_and_add(_ptr, _v);
}

KT_FORCEINLINE int64_t AtomicFetchAdd64(int64_t* _ptr, int64_t _v)
{
	return __sync_fetch_and_add(_ptr, _v);
}

KT_FORCEINLINE int16_t AtomicCompareAndSwap16(int16_t* _ptr, int16_t _old, int16_t _new)
{
	return __sync_val_compare_and_swap(_ptr, _old, _new);
}

KT_FORCEINLINE int32_t AtomicCompareAndSwap32(int32_t* _ptr, int32_t _old, int32_t _new)
{
	return __sync_val_compare_and_swap(_ptr, _old, _new);
}

KT_FORCEINLINE int64_t AtomicCompareAndSwap64(int64_t* _ptr, int64_t _old, int64_t _new)
{
	return __sync_val_compare_and_swap(_ptr, _old, _new);
}

}

#endif