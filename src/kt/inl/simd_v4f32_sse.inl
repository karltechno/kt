#pragma once
#include "../simd_v4f32.h"
#include <xmmintrin.h>

namespace kt
{

namespace simd
{


KT_FORCEINLINE v4f32::v4f32(float _f)
{
	*this = Splat_v4f32(_f);
}

KT_FORCEINLINE v4f32::v4f32(float const* _v)
{
	*this = LoadU_v4f32(_v);
}
KT_FORCEINLINE v4f32::v4f32(float _a, float _b, float _c, float _d)
{
	*this = Load_v4f32(_a, _b, _c, _d);
}

KT_FORCEINLINE float v4f32::X() const
{
	return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
}

KT_FORCEINLINE float v4f32::Y() const
{
	return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
}

KT_FORCEINLINE float v4f32::Z() const
{
	return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
}

KT_FORCEINLINE float v4f32::W() const
{
	return _mm_cvtss_f32(_mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
}

KT_FORCEINLINE v4f32 Min(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_min_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 Max(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_max_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 Abs(v4f32 _v)
{
	return v4f32(_mm_and_ps(_v.v, KT_SIMD_V4F32_LOAD_CONST(s_absMask_v4f32)));
}

KT_FORCEINLINE v4f32 Sign(v4f32 _v)
{
	return v4f32(_mm_and_ps(_v.v, KT_SIMD_V4F32_LOAD_CONST(s_signBit_v4f32)));
}

KT_FORCEINLINE v4f32 Sqrt(v4f32 _v)
{
	return v4f32(_mm_sqrt_ps(_v.v));
}

KT_FORCEINLINE v4f32 Rsqrt(v4f32 _v)
{
	return v4f32(_mm_rsqrt_ps(_v.v));
}

KT_FORCEINLINE v4f32 Recip(v4f32 _v)
{
	return v4f32(_mm_rcp_ps(_v.v));
}

KT_FORCEINLINE v4f32 Blend(v4b32 _mask, v4f32 _true, v4f32 _false)
{
	return v4f32(_mm_or_ps(_mm_and_ps(_mm_castsi128_ps(_mask.v), _true.v), _mm_andnot_ps(_mm_castsi128_ps(_mask.v), _false.v)));
}

KT_FORCEINLINE v4f32 AndNot(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_andnot_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 Splat_v4f32(float _f)
{
	return v4f32(_mm_set1_ps(_f));
}

KT_FORCEINLINE v4f32 Load_v4f32(float _f0, float _f1, float _f2, float _f3)
{
	return v4f32(_mm_setr_ps(_f0, _f1, _f2, _f3));
}

KT_FORCEINLINE v4f32 LoadA_v4f32(float const* _f)
{
	return v4f32(_mm_load_ps(_f));
}

KT_FORCEINLINE v4f32 LoadU_v4f32(float const* _f)
{
	return v4f32(_mm_loadu_ps(_f));
}

KT_FORCEINLINE void StoreA_v4f32(v4f32 _v, float* _f)
{
	_mm_store_ps(_f, _v.v);
}

KT_FORCEINLINE void StoreU_v4f32(v4f32 _v, float* _f)
{
	_mm_storeu_ps(_f, _v.v);
}

template<uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE v4f32 Shuf(v4f32 _v)
{
	static_assert(A < 4 && B < 4 && C < 4 && D < 4, "Shuffle error, indicies must be in the range [0, 3].");
	return v4f32(_mm_shuffle_ps(_v.v, _v.v, _MM_SHUFFLE(D, C, B, A)));
}

KT_FORCEINLINE uint32_t MoveMask(v4f32 _v)
{
	return uint32_t(_mm_movemask_ps(_v.v));
}

KT_FORCEINLINE v4b32 As_v4b32(v4f32 _v)
{
	return v4b32(_mm_castps_si128(_v.v));
}

KT_FORCEINLINE v4f32 As_v4f32(v4b32 _v)
{
	return v4f32(_mm_castsi128_ps(_v.v));
}

KT_FORCEINLINE v4b32 operator==(v4f32 _lhs, v4f32 _rhs)
{
	return v4b32(_mm_castps_si128((_mm_cmpeq_ps(_lhs.v, _rhs.v))));
}

KT_FORCEINLINE v4b32 operator>=(v4f32 _lhs, v4f32 _rhs)
{
	return v4b32(_mm_castps_si128((_mm_cmpge_ps(_lhs.v, _rhs.v))));
}

KT_FORCEINLINE v4b32 operator<=(v4f32 _lhs, v4f32 _rhs)
{
	return v4b32(_mm_castps_si128(_mm_cmple_ps(_lhs.v, _rhs.v)));
}

KT_FORCEINLINE v4b32 operator>(v4f32 _lhs, v4f32 _rhs)
{
	return v4b32(_mm_castps_si128((_mm_cmpgt_ps(_lhs.v, _rhs.v))));
}

KT_FORCEINLINE v4b32 operator<(v4f32 _lhs, v4f32 _rhs)
{
	return v4b32(_mm_castps_si128((_mm_cmplt_ps(_lhs.v, _rhs.v))));
}

KT_FORCEINLINE v4f32 operator-(v4f32 _v)
{
	return v4f32(_mm_xor_ps(_v.v, KT_SIMD_V4F32_LOAD_CONST(s_signBit_v4f32)));
}

KT_FORCEINLINE v4f32 operator~(v4f32 _v)
{
	KT_SIMD_V4_BITWISE_CONST1(s_allBits, 0xFFFFFFFF);
	return v4f32(_mm_xor_ps(_v.v, KT_SIMD_V4F32_LOAD_CONST(s_allBits)));
}

KT_FORCEINLINE v4f32 operator|(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_or_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 operator&(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_and_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 operator^(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_xor_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 operator+(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_add_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 operator-(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_sub_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 operator*(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_mul_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32 operator/(v4f32 _lhs, v4f32 _rhs)
{
	return v4f32(_mm_div_ps(_lhs.v, _rhs.v));
}

KT_FORCEINLINE v4f32& v4f32::operator*=(v4f32 _other)
{
	v = _mm_mul_ps(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4f32& v4f32::operator/=(v4f32 _other)
{
	v = _mm_div_ps(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4f32& v4f32::operator+=(v4f32 _other)
{
	v = _mm_add_ps(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4f32& v4f32::operator-=(v4f32 _other)
{
	v = _mm_sub_ps(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4f32& v4f32::operator&=(v4f32 _other)
{
	v = _mm_and_ps(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4f32& v4f32::operator^=(v4f32 _other)
{
	v = _mm_xor_ps(v, _other.v);
	return *this;
}

KT_FORCEINLINE v4f32& v4f32::operator|=(v4f32 _other)
{
	v = _mm_or_ps(v, _other.v);
	return *this;
}

} // namespace simd

template <>
KT_FORCEINLINE simd::v4f32 Min(simd::v4f32 _lhs, simd::v4f32 _rhs)
{
	return simd::Min(_lhs, _rhs);
}

template <>
KT_FORCEINLINE simd::v4f32 Max(simd::v4f32 _lhs, simd::v4f32 _rhs)
{
	return simd::Max(_lhs, _rhs);
}

} // namespace kt