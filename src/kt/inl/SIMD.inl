#include "../SIMD.h"

namespace kt
{

KT_FORCEINLINE SIMD_Float4 SIMD_Dot3_Vertical(SIMD_Float4 const _x0, SIMD_Float4 const _x1, SIMD_Float4 const _y0, SIMD_Float4 const _y1, SIMD_Float4 const _z0, SIMD_Float4 const _z1)
{
	SIMD_Float4 const x2 = SIMD_Mul(_x0, _x1);
	SIMD_Float4 const y2 = SIMD_Mul(_y0, _y1);
	SIMD_Float4 const z2 = SIMD_Mul(_z0, _z1);
	return SIMD_Add(SIMD_Add(x2, y2), z2);
}

template <>
KT_FORCEINLINE SIMD_Float4 Min(SIMD_Float4 const _a, SIMD_Float4 const _b)
{
	return SIMD_Min(_a, _b);
}

template <>
KT_FORCEINLINE SIMD_Float4 Max(SIMD_Float4 const _a, SIMD_Float4 const _b)
{
	return SIMD_Max(_a, _b);
}

KT_FORCEINLINE SIMD_Float4 Lerp(SIMD_Float4 const _a, SIMD_Float4 const _b, SIMD_Float4 const _t)
{
	return SIMD_Madd(SIMD_Sub(_b, _a), _t, _a);
}

KT_FORCEINLINE bool SIMD_AnyTrue(SIMD_Float4 const _v)
{
	return SIMD_MoveMask(_v) != 0;
}

KT_FORCEINLINE bool SIMD_AllTrue(SIMD_Float4 const _v)
{
	return SIMD_MoveMask(_v) == 15;
}

KT_FORCEINLINE SIMD_Float4 operator==(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_CmpEq(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator>=(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_CmpGe(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator<=(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_CmpLe(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator>(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_CmpGt(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator<(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_CmpLt(_lhs, _rhs);
}


KT_FORCEINLINE SIMD_Float4 operator-(SIMD_Float4 const _v)
{
	return SIMD_Neg(_v);
}

KT_FORCEINLINE SIMD_Float4 operator~(SIMD_Float4 const _v)
{
	return SIMD_Not(_v);
}

KT_FORCEINLINE SIMD_Float4 operator|(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_Or(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator&(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_And(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator^(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_Xor(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator+(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_Add(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator-(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_Sub(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator*(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_Mul(_lhs, _rhs);
}

KT_FORCEINLINE SIMD_Float4 operator/(SIMD_Float4 const _lhs, SIMD_Float4 const _rhs)
{
	return SIMD_Div(_lhs, _rhs);
}


}
