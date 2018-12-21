#pragma once
#include "kt.h"

#if KT_CPU_X86
	#include <xmmintrin.h>
	namespace kt 
	{ 

	struct KT_ALIGNAS(16) SIMD_Float4
	{
		SIMD_Float4() {}
		SIMD_Float4(__m128 _v) : v(_v) {}
		operator __m128() const { return v; }
		__m128 v;
	};

	}
#else
	#error Not yet implemented
#endif

namespace kt
{

KT_FORCEINLINE SIMD_Float4 SIMD_Add(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_Sub(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_Mul(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_Div(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_Madd(SIMD_Float4 const _a, SIMD_Float4 const _b, SIMD_Float4 const _c);

KT_FORCEINLINE SIMD_Float4 SIMD_Min(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_Max(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);

KT_FORCEINLINE SIMD_Float4 SIMD_Abs(SIMD_Float4 const _v);
KT_FORCEINLINE SIMD_Float4 SIMD_Neg(SIMD_Float4 const _v);
KT_FORCEINLINE SIMD_Float4 SIMD_Sign(SIMD_Float4 const _v);

KT_FORCEINLINE SIMD_Float4 SIMD_Sqrt(SIMD_Float4 const _v);
KT_FORCEINLINE SIMD_Float4 SIMD_Rsqrt(SIMD_Float4 const _v);

KT_FORCEINLINE SIMD_Float4 SIMD_CmpEq(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_CmpGt(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_CmpLt(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_CmpLe(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_CmpGe(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);

template <uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE SIMD_Float4 SIMD_Shuf(SIMD_Float4 const _v);

template <uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE SIMD_Float4 SIMD_Mask();

KT_FORCEINLINE int32_t SIMD_MoveMask(SIMD_Float4 const _v);
KT_FORCEINLINE bool SIMD_AnyTrue(SIMD_Float4 const _v);
KT_FORCEINLINE bool SIMD_AllTrue(SIMD_Float4 const _v);

KT_FORCEINLINE SIMD_Float4 SIMD_Sel(SIMD_Float4 const _mask, SIMD_Float4 const _A, SIMD_Float4 const _B);
KT_FORCEINLINE SIMD_Float4 SIMD_And(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_AndNot(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_Not(SIMD_Float4 const _v);
KT_FORCEINLINE SIMD_Float4 SIMD_Or(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 SIMD_Xor(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);

KT_FORCEINLINE SIMD_Float4 SIMD_Splat(float const _f);
KT_FORCEINLINE SIMD_Float4 SIMD_Load4(float const _f0, float const _f1, float const _f2, float const _f3);
KT_FORCEINLINE SIMD_Float4 SIMD_Load4A(float const* _f);
KT_FORCEINLINE SIMD_Float4 SIMD_Load4U(float const* _f);
KT_FORCEINLINE void SIMD_Store4A(SIMD_Float4 const _v, float* _f);
KT_FORCEINLINE void SIMD_Store4U(SIMD_Float4 const _v, float* _f);

KT_FORCEINLINE SIMD_Float4 SIMD_Dot3_Vertical(SIMD_Float4 const _x0, SIMD_Float4 const _x1, SIMD_Float4 const _y0, SIMD_Float4 const _y1, SIMD_Float4 const _z0, SIMD_Float4 const _z1);

KT_FORCEINLINE SIMD_Float4 operator==(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator>=(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator<=(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator>(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator<(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);

KT_FORCEINLINE SIMD_Float4 operator-(SIMD_Float4 const _v);
KT_FORCEINLINE SIMD_Float4 operator~(SIMD_Float4 const _v);

KT_FORCEINLINE SIMD_Float4 operator|(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator&(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator^(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);

KT_FORCEINLINE SIMD_Float4 operator+(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator-(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator*(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);
KT_FORCEINLINE SIMD_Float4 operator/(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs);


template <>
KT_FORCEINLINE SIMD_Float4 Min(SIMD_Float4 const _a, SIMD_Float4 const _b);

template <>
KT_FORCEINLINE SIMD_Float4 Max(SIMD_Float4 const _a, SIMD_Float4 const _b);

KT_FORCEINLINE SIMD_Float4 Lerp(SIMD_Float4 const _a, SIMD_Float4 const _b, SIMD_Float4 const _t);
}



#if KT_CPU_X86
#include "inl/SIMD_SSE.inl"
#else
	#error Not yet implemented
#endif

#include "inl/SIMD.inl"