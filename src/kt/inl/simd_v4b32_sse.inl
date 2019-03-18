#pragma once
#include "../simd_v4f32.h"
#include <xmmintrin.h>

namespace kt
{

namespace simd
{

KT_FORCEINLINE v4b32& v4b32::operator&=(v4b32 _other)
{
	v = _mm_and_si128(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4b32& v4b32::operator^=(v4b32 _other)
{
	v = _mm_xor_si128(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4b32& v4b32::operator|=(v4b32 _other)
{
	v = _mm_or_si128(v, _other.v);
	return *this;
}

template <uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE v4b32 Shuf(v4b32 _v)
{
	static_assert(A < 4 && B < 4 && C < 4 && D < 4, "Shuffle error, indicies must be in the range [0, 3].");
	return v4b32(_mm_shuffle_epi32(_v.v, _v.v, _MM_SHUFFLE(D, C, B, A)));
}

template <uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE v4b32 Mask_v4b32()
{
	KT_SIMD_V4_BITWISE_CONST4(vec, A ? 0xFFFFFFFF : 0, B ? 0xFFFFFFFF : 0, C ? 0xFFFFFFFF : 0, D ? 0xFFFFFFFF : 0);
	return v4b32(KT_SIMD_V4B32_LOAD_CONST(vec));
}

KT_FORCEINLINE uint32_t MoveMask(v4b32 _v)
{
	return _mm_movemask_ps(_mm_castsi128_ps(_v.v));
}

KT_FORCEINLINE bool AnyTrue(v4b32 _v)
{
	return MoveMask(_v) != 0;
}

KT_FORCEINLINE bool AllTrue(v4b32 _v)
{
	return MoveMask(_v) == 15;
}

KT_FORCEINLINE v4b32 operator==(v4b32 _lhs, v4b32 _rhs)
{
	return v4b32(_mm_cmpeq_epi32(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4b32 operator~(v4b32 _v)
{
	KT_SIMD_V4_BITWISE_CONST1(s_allBits, 0xFFFFFFFF);
	return v4b32(_mm_xor_si128(_v.v, KT_SIMD_V4B32_LOAD_CONST(s_allBits)));
}

KT_FORCEINLINE v4b32 operator|(v4b32 _lhs, v4b32 _rhs)
{
	return v4b32(_mm_or_si128(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4b32 operator&(v4b32 _lhs, v4b32 _rhs)
{
	return v4b32(_mm_and_si128(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4b32 operator^(v4b32 _lhs, v4b32 _rhs)
{
	return v4b32(_mm_xor_si128(_lhs.v, _rhs.v));
}

} // namespace simd



} // namespace kt