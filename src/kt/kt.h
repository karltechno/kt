#pragma once
#include <stdint.h>

#include "Platform.h"
#include "Macros.h"

namespace kt
{

// Check if the value is a power of two.
template <typename T>
KT_FORCEINLINE bool IsPow2(T const _v);

// Returns the next power of two after the specified value. Undefined for 2^32.
// This is the true 'next' power of two, ie 2^n return 2^(n+1). So use NextPow2(N-1) for rounding to nearest pow2.
KT_FORCEINLINE uint32_t NextPow2(uint32_t const _v);

// Generic Min.
template <typename T>
T Min(T const _a, T const _b);

// Generic Max.
template <typename T>
T Max(T const _a, T const _b);

// Generic clamp: Max(Min(_val, _max), _min));
template <typename T>
T Clamp(T const _val, T const _min, T const _max);

// Generic swap.
template <typename T>
void Swap(T& _a, T& _b);

// Bit ops

// Count leading zeroes.
KT_FORCEINLINE uint32_t Cntlz(uint32_t const _v);
KT_FORCEINLINE uint64_t Cntlz(uint64_t const _v);

// Count trailing zeroes.
KT_FORCEINLINE uint32_t Cnttz(uint32_t const _v);
KT_FORCEINLINE uint64_t Cnttz(uint64_t const _v);

// Population count.
KT_FORCEINLINE int32_t Popcnt(uint32_t const _v);
KT_FORCEINLINE uint64_t Popcnt(uint64_t const _v);

// Ranged for support.

template <typename T>
auto begin(T&& _container) -> decltype(_container.Begin()) { return _container.Begin(); }
template <typename T>
auto end(T&& _container) -> decltype(_container.End()) { return _container.End(); }


}

// Define enum class bitwise flag operators. 
#define KT_ENUM_CLASS_FLAG_OPERATORS(_enum)

#include "inl/kt.inl"