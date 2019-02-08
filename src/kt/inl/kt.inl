#pragma once
#if KT_COMPILER_MSVC
	#include <intrin.h>
#endif

namespace kt
{

KT_FORCEINLINE uint32_t Ctnlz(uint32_t const _v)
{
#if KT_COMPILER_GCC || KT_COMPILER_CLANG
	return __builtin_clz(_v);
#elif KT_COMPILER_MSVC
	unsigned long idx;
	::_BitScanReverse(&idx, _v);
	return 31 - idx;
#else
	#error Not implemented
#endif
}

KT_FORCEINLINE uint64_t Cntlz(uint64_t const _v)
{
#if KT_COMPILER_GCC || KT_COMPILER_CLANG
	return __builtin_clzll(_v);
#elif (KT_COMPILER_MSVC && KT_ARCH_64BIT)
	unsigned long idx;
	return ::_BitScanReverse64(&idx, _v) ? (63 - idx) : 64;
#else
#error Not implemented
#endif
}

KT_FORCEINLINE uint32_t Cnttz(uint32_t const _v)
{
#if KT_COMPILER_GCC || KT_COMPILER_CLANG
	return __builtin_ctz(_v);
#elif KT_COMPILER_MSVC
	unsigned long idx;
	return ::_BitScanForward(&idx, _v) ? idx : 32;
#else
	#error Not implemented
#endif
}

KT_FORCEINLINE uint64_t Cnttz(uint64_t const _v)
{
#if KT_COMPILER_GCC || KT_COMPILER_CLANG
	return __builtin_ctzll(_v);
#elif (KT_COMPILER_MSVC && KT_ARCH_64BIT)
	unsigned long idx;
	return ::_BitScanForward64(&idx, _v) ? idx : 64;
#else
#error Not implemented
#endif
}

KT_FORCEINLINE int32_t Popcnt(uint32_t const _v)
{
#if KT_COMPILER_GCC || KT_COMPILER_CLANG
	return __builtin_popcount(_v);
#elif KT_COMPILER_MSVC
	return ::__popcnt(_v);
#else
#error Not implemented
#endif
}

KT_FORCEINLINE uint64_t Popcnt(uint64_t const _v)
{
#if KT_COMPILER_GCC || KT_COMPILER_CLANG
	return __builtin_popcountll(_v);
#elif (KT_COMPILER_MSVC && KT_ARCH_64BIT)
	return ::__popcnt64(_v);
#else
#error Not implemented
#endif
}

KT_FORCEINLINE uint32_t NextPow2(uint32_t const _v)
{
	return _v > 1 ? (1 << (32 - Cntlz(_v))) : 2;
}

template <typename T>
KT_FORCEINLINE bool IsPow2(T const _v)
{
	return (_v & (_v - (T)1)) == (T)0;
}


template <typename T>
T Min(T const _a, T const _b)
{
	return _a < _b ? _a : _b;
}

template <typename T>
T Max(T const _a, T const _b)
{
	return _a > _b ? _a : _b;
}

template <typename T>
T Clamp(T const _val, T const _min, T const _max)
{
	return Min(Max(_val, _min), _max);
}

template <typename T>
void Swap(T& _a, T& _b)
{
	T temp = std::move(_a);
	_a = std::move(_b);
	_b = std::move(temp);
}

}

#define KT_ENUM_CLASS_FLAG_OPERATORS(_enumType) \
inline _enumType operator | (_enumType a, _enumType b) throw() { return _enumType(((__underlying_type(_enumType))a) | ((__underlying_type(_enumType))b)); } \
inline _enumType &operator |= (_enumType &a, _enumType b) throw() { return (_enumType &)(((__underlying_type(_enumType) &)a) |= ((__underlying_type(_enumType))b)); } \
inline _enumType operator & (_enumType a, _enumType b) throw() { return _enumType(((__underlying_type(_enumType))a) & ((__underlying_type(_enumType))b)); } \
inline _enumType &operator &= (_enumType &a, _enumType b) throw() { return (_enumType &)(((__underlying_type(_enumType) &)a) &= ((__underlying_type(_enumType))b)); } \
inline _enumType operator ~ (_enumType a) throw() { return _enumType(~((__underlying_type(_enumType))a)); } \
inline _enumType operator ^ (_enumType a, _enumType b) throw() { return _enumType(((__underlying_type(_enumType))a) ^ ((__underlying_type(_enumType))b)); } \
inline _enumType &operator ^= (_enumType &a, _enumType b) throw() { return (_enumType &)(((__underlying_type(_enumType) &)a) ^= ((__underlying_type(_enumType))b)); } \
