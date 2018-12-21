#pragma once
#include "../SIMD.h"

namespace kt
{

#define SSE_CONSTANT_BITWISE(_name, _v) KT_ALIGNAS(16) static uint32_t const _name[4] = { _v, _v, _v, _v };
#define SSE_CONSTANT_BITWISE4(_name, _0, _1, _2, _3) KT_ALIGNAS(16) static uint32_t const _name[4] = { _0, _1, _2, _3 };
#define SSE_LOAD_CONSTANT(_c) _mm_load_ps((float*)_c)

SSE_CONSTANT_BITWISE(s_signBit, 0x80000000);


KT_FORCEINLINE SIMD_Float4 SIMD_Add(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_add_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Sub(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_sub_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Mul(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_mul_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Div(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_div_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Madd(SIMD_Float4 const _a, SIMD_Float4 const _b, SIMD_Float4 const _c)
{
	return _mm_add_ps(_mm_mul_ps(_a, _b), _c);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Min(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_min_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Max(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_max_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Abs(SIMD_Float4 const _v)
{
	return _mm_andnot_ps(_v, SSE_LOAD_CONSTANT(s_signBit));
}

KT_FORCEINLINE SIMD_Float4 SIMD_Sign(SIMD_Float4 const _v)
{
	return _mm_and_ps(_v, SSE_LOAD_CONSTANT(s_signBit));
}

KT_FORCEINLINE SIMD_Float4 SIMD_Neg(SIMD_Float4 const _v)
{
	return _mm_xor_ps(_v, SSE_LOAD_CONSTANT(s_signBit));
}

KT_FORCEINLINE SIMD_Float4 SIMD_Sqrt(SIMD_Float4 const _v)
{
	return _mm_sqrt_ps(_v);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Rsqrt(SIMD_Float4 const _v)
{
	return _mm_rsqrt_ps(_v);
}

KT_FORCEINLINE SIMD_Float4 SIMD_CmpEq(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_cmpeq_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_CmpGt(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_cmpgt_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_CmpLt(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_cmplt_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_CmpLe(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_cmple_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_CmpGe(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_cmpge_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Sel(SIMD_Float4 const _mask, SIMD_Float4 const _A, SIMD_Float4 const _B)
{
	return _mm_or_ps(_mm_and_ps(_mask, _A), _mm_andnot_ps(_mask, _B));
}

KT_FORCEINLINE SIMD_Float4 SIMD_And(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_and_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_AndNot(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_andnot_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Not(SIMD_Float4 const _v)
{
	return _mm_xor_ps(_v, SIMD_Mask<1, 1, 1, 1>());
}

KT_FORCEINLINE SIMD_Float4 SIMD_Or(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_or_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Xor(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return _mm_xor_ps(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Splat(float const _f)
{
	return _mm_set1_ps(_f);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Load4(float const _f0, float const _f1, float const _f2, float const _f3)
{
	return _mm_set_ps(_f3, _f2, _f1, _f0);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Load4A(float const * _f)
{
	return _mm_load_ps(_f);
}

KT_FORCEINLINE SIMD_Float4 SIMD_Load4U(float const * _f)
{
	return _mm_loadu_ps(_f);
}

template<uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE SIMD_Float4 SIMD_Shuf(SIMD_Float4 const _v)
{
	static_assert(A < 4 && B < 4 && C < 4 && D < 4, "Shuffle error, indicies must be in the range [0, 3].");

	return _mm_shuffle_ps(_v, _v, _MM_SHUFFLE(D, C, B, A));
}

template<uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE SIMD_Float4 SIMD_Mask()
{
	SSE_CONSTANT_BITWISE4(vec, A ? 0xFFFFFFFF : 0, B ? 0xFFFFFFFF : 0, C ? 0xFFFFFFFF : 0, D ? 0xFFFFFFFF : 0);
	return SSE_LOAD_CONSTANT(vec);
}

KT_FORCEINLINE int32_t SIMD_MoveMask(SIMD_Float4 const _v)
{
	return _mm_movemask_ps(_v);
}

KT_FORCEINLINE void SIMD_Store4A(SIMD_Float4 const _v, float* _f)
{
	_mm_store_ps(_f, _v);
}

KT_FORCEINLINE void SIMD_Store4U(SIMD_Float4 const _v, float* _f)
{
	_mm_storeu_ps(_f, _v);
}


}
#undef SSE_LOAD_CONSTANT
#undef SSE_CONSTANT_BITWISE
#undef SSE_CONSTANT_BITWISE4