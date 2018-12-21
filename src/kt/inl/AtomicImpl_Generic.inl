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

}
