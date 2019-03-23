#pragma once
#include "../Concurrency.h"
#include "../Memory.h"

#if KT_PLATFORM_WINDOWS
	extern "C" unsigned char _InterlockedCompareExchange128(__int64 volatile* _dest, __int64 exchHigh, __int64 exchLo, __int64* compareAndResult);
	#pragma intrinsic(_InterlockedCompareExchange128)
#endif
namespace kt
{

template <typename T>
bool AtomicCompareExchange128(T* _v, T* io_expAndOld, T const& _new)
{
	static_assert(sizeof(T) == 16, "sizeof T is not 128 bits.");
	KT_ASSERT(kt::IsAligned(uintptr_t(_v), 16));
	KT_ASSERT(kt::IsAligned(uintptr_t(io_expAndOld), 16));

#if KT_PLATFORM_WINDOWS
	KT_ALIGNAS(16) __int64 new64[2];
	memcpy(new64, &_new, 16);
	return _InterlockedCompareExchange128((__int64 volatile*)_v, new64[1], new64[0], (__int64*)io_expAndOld);
#endif
}

}
