#pragma once
#include "../Concurrency.h"

namespace kt
{

template <typename T>
KT_FORCEINLINE T* AtomicLoadPtr(T* const* _ptr)
{
#if KT_ARCH_64BIT
	return (T*)AtomicLoad64((int64_t*)_ptr);
#else
	return (T*)AtomicLoad32((int32_t*)_ptr);
#endif
}

template <typename T>
KT_FORCEINLINE void AtomicStorePtr(T** _ptr, T* _val)
{
#if KT_ARCH_64BIT
	AtomicStore64((int64_t*)_ptr, (int64_t)_val);
#else
	AtomicStore32((int32_t*)_ptr, (int32_t)_val);
#endif
}

template <typename T>
KT_FORCEINLINE T* AtomicCompareAndSwapPtr(T** _ptr, T* _old, T* _new)
{
#if KT_ARCH_64BIT
	return (T*)AtomicCompareAndSwap64((int64_t*)_ptr, (int64_t)_old, (int64_t)_new);
#else
	return (T*)AtomicCompareAndSwap32((int32_t*)_ptr, (int32_t)_old, (int32_t)_new);
#endif
}

KT_FORCEINLINE uint8_t AtomicLoad8(uint8_t const* _ptr)
{
	return (uint8_t)AtomicLoad8((int8_t const*)_ptr);
}

KT_FORCEINLINE uint16_t AtomicLoad16(uint16_t const* _ptr)
{
	return (uint16_t)AtomicLoad16((int16_t const*)_ptr);
}

KT_FORCEINLINE uint32_t AtomicLoad32(uint32_t const* _ptr)
{
	return (uint32_t)AtomicLoad32((int32_t const*)_ptr);
}

KT_FORCEINLINE uint64_t AtomicLoad64(uint64_t const* _ptr)
{
	return (uint64_t)AtomicLoad64((int64_t const*)_ptr);
}

KT_FORCEINLINE void AtomicStore8(uint8_t* _ptr, uint8_t _val)
{
	AtomicStore8((int8_t*)_ptr, (int8_t)_val);
}


KT_FORCEINLINE void AtomicStore16(uint16_t* _ptr, uint16_t _val)
{
	AtomicStore16((int16_t*)_ptr, (int16_t)_val);
}

KT_FORCEINLINE void AtomicStore32(uint32_t* _ptr, uint32_t _val)
{
	AtomicStore32((int32_t*)_ptr, (int32_t)_val);
}

KT_FORCEINLINE void AtomicStore64(uint64_t* _ptr, uint64_t _val)
{
	AtomicStore64((int64_t*)_ptr, (int64_t)_val);
}

KT_FORCEINLINE uint8_t  AtomicFetchAdd8(uint8_t* _ptr, uint8_t _v)
{
	return (uint8_t)AtomicFetchAdd8((int8_t*)_ptr, (int8_t)_v);
}
KT_FORCEINLINE uint16_t AtomicFetchAdd16(uint16_t* _ptr, uint16_t _v)
{
	return (uint16_t)AtomicFetchAdd16((int16_t*)_ptr, (int16_t)_v);
}

KT_FORCEINLINE uint32_t AtomicFetchAdd32(uint32_t* _ptr, uint32_t _v)
{
	return (uint32_t)AtomicFetchAdd32((int32_t*)_ptr, (int32_t)_v);
}

KT_FORCEINLINE uint64_t AtomicFetchAdd64(uint64_t* _ptr, uint64_t _v)
{
	return (uint64_t)AtomicFetchAdd64((int64_t*)_ptr, (int64_t)_v);
}

KT_FORCEINLINE uint16_t AtomicCompareAndSwap16(uint16_t* _ptr, uint16_t _old, uint16_t _new)
{
	return (uint16_t)AtomicCompareAndSwap16((int16_t*)_ptr, (int16_t)_old, (int16_t)_new);
}

KT_FORCEINLINE uint32_t AtomicCompareAndSwap32(uint32_t* _ptr, uint32_t _old, uint32_t _new)
{
	return (uint32_t)AtomicCompareAndSwap32((int32_t*)_ptr, (int32_t)_old, (int32_t)_new);
}

KT_FORCEINLINE uint64_t AtomicCompareAndSwap64(uint64_t* _ptr, uint64_t _old, uint64_t _new)
{
	return (uint64_t)AtomicCompareAndSwap64((int64_t*)_ptr, (int64_t)_old, (int64_t)_new);
}

}
