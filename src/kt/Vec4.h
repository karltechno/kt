#pragma once
#include "kt.h"
#include "MathUtil.h"
#include "Vec3.h"

namespace kt
{

struct Vec4
{
	float x;
	float y;
	float z;
	float w;

	KT_FORCEINLINE Vec4() {}

	KT_FORCEINLINE explicit Vec4(float const _f)
		: x(_f), y(_f), z(_f), w(_f)
	{
	}

	KT_FORCEINLINE Vec4(float const _inX, float const _inY, float const _inZ, float const _inW)
		: x(_inX), y(_inY), z(_inZ), w(_inW)
	{
	}

	KT_FORCEINLINE Vec4(Vec4 const& _v)
		: x(_v.x), y(_v.y), z(_v.z), w(_v.w)
	{
	}

	KT_FORCEINLINE Vec4(Vec3 const& _vec, float const _w)
		: x(_vec.x), y(_vec.y), z(_vec.z), w(_w) 
	{
	}

	KT_FORCEINLINE Vec4 &operator=(Vec4 const& _v);

	KT_FORCEINLINE Vec4 &operator*= (float const _f);

	KT_FORCEINLINE Vec4 &operator-= (float const _f);

	KT_FORCEINLINE Vec4 &operator-= (Vec4 const& _other);

	KT_FORCEINLINE Vec4 &operator+= (Vec4 const& _other);

	KT_FORCEINLINE Vec4 &operator/= (float const _f);

	KT_FORCEINLINE float& operator[](uint32_t const _index);

	KT_FORCEINLINE float const& operator[](uint32_t const _index) const;
};

KT_FORCEINLINE Vec4 operator* (Vec4 const& _lhs, Vec4 const& _rhs);

KT_FORCEINLINE Vec4 operator* (Vec4 const& _lhs, float const _f);

KT_FORCEINLINE Vec4 operator/ (Vec4 const& _lhs, float const _f);

KT_FORCEINLINE Vec4 operator* (float const _f, Vec4 const& _rhs);

KT_FORCEINLINE Vec4 operator/ (float const _f, Vec4 const& _rhs);

KT_FORCEINLINE Vec4 operator-(Vec4 const& _v);

KT_FORCEINLINE Vec4 operator+(Vec4 const& _lhs, Vec4 const& _rhs);

KT_FORCEINLINE Vec4 operator-(Vec4 const& _lhs, Vec4 const& _rhs);

KT_FORCEINLINE float LengthSq(Vec4 const& _vec);

KT_FORCEINLINE float Length(Vec4 const& _vec);

KT_FORCEINLINE Vec4 Normalize(Vec4 const& _vec);

KT_FORCEINLINE float Distance(Vec4 const& _lhs, Vec4 const& _rhs);

KT_FORCEINLINE float DistanceSq(Vec4 const& _lhs, Vec4 const& _rhs);

KT_FORCEINLINE float Dot(Vec4 const& _lhs, Vec4 const& _rhs);

KT_FORCEINLINE bool operator==(Vec4 const& _v1, Vec4 const& _v2);

KT_FORCEINLINE bool operator!=(Vec4 const& _v1, Vec4 const& _v2);

KT_FORCEINLINE Vec4 Lerp(Vec4 const& _lhs, Vec4 const& _rhs, float const _t);

KT_FORCEINLINE Vec4 Min(Vec4 const& _a, Vec4 const& _b);

KT_FORCEINLINE Vec4 Max(Vec4 const& _a, Vec4 const& _b);

KT_FORCEINLINE Vec4 Clamp(Vec4 const& _a, Vec4 const& _min, Vec4 const& _max);

KT_FORCEINLINE Vec4 Abs(Vec4 const& _v);

KT_FORCEINLINE float MaxComponent(Vec4 const& _v);

}

#include "inl/Vec4.inl"