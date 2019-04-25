#pragma once
#include "kt.h"
#include "Platform.h"
#include "SIMD.h"
#include "simd_v4b32.h"


namespace kt
{

namespace simd
{

KT_SIMD_V4F32_CONST1(s_signBit_v4f32, 0x80000000);
KT_SIMD_V4_BITWISE_CONST1(s_absMask_v4f32, 0x7FFFFFFF);

struct v4f32
{
	static uint32_t const c_width = 4;
	static uint32_t const c_align = 16;

	v4f32() = default;

	KT_FORCEINLINE explicit v4f32(float _f);
	KT_FORCEINLINE explicit v4f32(float const* _v);
	KT_FORCEINLINE v4f32(float _a, float _b, float _c, float _d);

	KT_FORCEINLINE float X() const;
	KT_FORCEINLINE float Y() const;
	KT_FORCEINLINE float Z() const;
	KT_FORCEINLINE float W() const;

	KT_FORCEINLINE v4f32& operator*=(v4f32 _other);
	KT_FORCEINLINE v4f32& operator/=(v4f32 _other);
	KT_FORCEINLINE v4f32& operator+=(v4f32 _other);
	KT_FORCEINLINE v4f32& operator-=(v4f32 _other);

	KT_FORCEINLINE v4f32& operator&=(v4f32 _other);
	KT_FORCEINLINE v4f32& operator^=(v4f32 _other);
	KT_FORCEINLINE v4f32& operator|=(v4f32 _other);

#if KT_SSE2
	KT_FORCEINLINE explicit v4f32(__m128 _xmm) : v(_xmm) {}
	__m128 v;
#endif
};

KT_FORCEINLINE v4f32 Min(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4f32 Max(v4f32 _lhs, v4f32 _rhs);

KT_FORCEINLINE v4f32 Abs(v4f32 _v);
KT_FORCEINLINE v4f32 Sign(v4f32 _v);

KT_FORCEINLINE v4f32 Sqrt(v4f32 _v);
KT_FORCEINLINE v4f32 Rsqrt(v4f32 _v);
KT_FORCEINLINE v4f32 Recip(v4f32 _v);

template <uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE v4f32 Shuf(v4f32 _v);

KT_FORCEINLINE uint32_t MoveMask(v4f32 _v);

KT_FORCEINLINE v4f32 Blend(v4b32 _mask, v4f32 _true, v4f32 _false);
KT_FORCEINLINE v4f32 AndNot(v4f32 _lhs, v4f32 _rhs);

KT_FORCEINLINE v4f32 Splat_v4f32(float _f);
KT_FORCEINLINE v4f32 Load_v4f32(float _f0, float _f1, float _f2, float _f3);
KT_FORCEINLINE v4f32 LoadA_v4f32(float const* _f);
KT_FORCEINLINE v4f32 LoadU_v4f32(float const* _f);
KT_FORCEINLINE void StoreA_v4f32(v4f32 _v, float* _f);
KT_FORCEINLINE void StoreU_v4f32(v4f32 _v, float* _f);

KT_FORCEINLINE v4b32 As_v4b32(v4f32 _v);
KT_FORCEINLINE v4f32 As_v4f32(v4b32 _v);

KT_FORCEINLINE v4b32 operator==(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4b32 operator>=(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4b32 operator<=(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4b32 operator>(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4b32 operator<(v4f32 _lhs, v4f32 _rhs);

KT_FORCEINLINE v4f32 operator-(v4f32 _v);
KT_FORCEINLINE v4f32 operator~(v4f32 _v);

KT_FORCEINLINE v4f32 operator|(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4f32 operator&(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4f32 operator^(v4f32 _lhs, v4f32 _rhs);

KT_FORCEINLINE v4f32 operator+(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4f32 operator-(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4f32 operator*(v4f32 _lhs, v4f32 _rhs);
KT_FORCEINLINE v4f32 operator/(v4f32 _lhs, v4f32 _rhs);

}

template <>
KT_FORCEINLINE simd::v4f32 Min(simd::v4f32 _lhs, simd::v4f32 _rhs);

template <>
KT_FORCEINLINE simd::v4f32 Max(simd::v4f32 _lhs, simd::v4f32 _rhs);

}


#if KT_SSE2
	#include "inl/simd_v4f32_sse.inl"
#else
	#error Not yet implemented
#endif
