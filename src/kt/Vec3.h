#pragma once
#include "kt.h"
#include "MathUtil.h"
#include "Vec2.h"

namespace kt
{
struct Vec4;

struct Vec3
{
	float x;
	float y;
	float z;

	KT_FORCEINLINE Vec3() {}

	KT_FORCEINLINE explicit Vec3(float const _f)
		: x(_f), y(_f), z(_f)
	{
	}

	KT_FORCEINLINE Vec3(float const _inX, float const _inY, float const _inZ)
		: x(_inX), y(_inY), z(_inZ)	
	{
	}

	Vec3(Vec3 const&) = default;
	Vec3& operator=(Vec3 const&) = default;

	KT_FORCEINLINE Vec3(Vec2 const& _v, float const _z)
		: x(_v.x), y(_v.y), z(_z)
	{}

	KT_FORCEINLINE Vec3(Vec4 const& _v); 

	KT_FORCEINLINE Vec3 &operator*= (float const _f);

	KT_FORCEINLINE Vec3 &operator-= (float const _f);

	KT_FORCEINLINE Vec3 &operator-= (Vec3 const& _other);

	KT_FORCEINLINE Vec3 &operator+= (Vec3 const& _other);

	KT_FORCEINLINE Vec3 &operator/= (float const _f);

	KT_FORCEINLINE float& operator[](uint32_t const _index);

	KT_FORCEINLINE float operator[](uint32_t const _index) const;
};

KT_FORCEINLINE Vec3 operator* (Vec3 const& _lhs, Vec3 const& _rhs);

KT_FORCEINLINE Vec3 operator* (Vec3 const& _lhs, float const _f);

KT_FORCEINLINE Vec3 operator/ (Vec3 const& _lhs, float const _f);

KT_FORCEINLINE Vec3 operator* (float const _f, Vec3 const& _rhs);

KT_FORCEINLINE Vec3 operator/ (float const _f, Vec3 const& _rhs);

KT_FORCEINLINE Vec3 operator-(Vec3 const& _v);

KT_FORCEINLINE Vec3 operator+(Vec3 const& _lhs, Vec3 const& _rhs);

KT_FORCEINLINE Vec3 operator-(Vec3 const& _lhs, Vec3 const& _rhs);

KT_FORCEINLINE float LengthSq(Vec3 const& _vec);

KT_FORCEINLINE float Length(Vec3 const& _vec);

KT_FORCEINLINE Vec3 Normalize(Vec3 const& _vec);

KT_FORCEINLINE float Distance(Vec3 const& _lhs, Vec3 const& _rhs);

KT_FORCEINLINE float DistanceSq(Vec3 const& _lhs, Vec3 const& _rhs);

KT_FORCEINLINE float Dot(Vec3 const& _lhs, Vec3 const& _rhs);

KT_FORCEINLINE Vec3 Cross(Vec3 const& _lhs, Vec3 const& _rhs);

KT_FORCEINLINE bool operator==(Vec3 const& _v1, Vec3 const& _v2);

KT_FORCEINLINE bool operator!=(Vec3 const& _v1, Vec3 const& _v2);

KT_FORCEINLINE Vec3 Lerp(Vec3 const& _lhs, Vec3 const& _rhs, float const _t);

KT_FORCEINLINE Vec3 Min(Vec3 const& _a, Vec3 const& _b);

KT_FORCEINLINE Vec3 Max(Vec3 const& _a, Vec3 const& _b);

KT_FORCEINLINE Vec3 Clamp(Vec3 const& _a, Vec3 const& _min, Vec3 const& _max);

KT_FORCEINLINE Vec3 Abs(Vec3 const& _v);

KT_FORCEINLINE float MaxComponent(Vec3 const& _v);

} 

#include "inl/Vec3.inl"