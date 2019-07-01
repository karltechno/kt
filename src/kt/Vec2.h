#pragma once
#include "kt.h"
#include "MathUtil.h"

namespace kt
{

struct Vec2
{
	float x;
	float y;

	Vec2() {}

	explicit Vec2(float const _f)
		: x(_f), y(_f) {}

	Vec2(float const _inX, float const _inY)
		: x(_inX), y(_inY) {}

	Vec2(Vec2 const& _v)
		: x(_v.x), y(_v.y) {}

	Vec2 &operator=(Vec2 const& _v);

	KT_FORCEINLINE Vec2 &operator*= (float const _f);

	KT_FORCEINLINE Vec2 &operator-= (float const _f);

	KT_FORCEINLINE Vec2 &operator-= (Vec2 const& _other);

	KT_FORCEINLINE Vec2 &operator+= (Vec2 const& _other);

	KT_FORCEINLINE Vec2 &operator/= (float const _f);

	KT_FORCEINLINE float& operator[](uint32_t const _index);

	KT_FORCEINLINE float const& operator[](uint32_t const _index) const;
};

KT_FORCEINLINE Vec2 operator* (Vec2 const& _lhs, Vec2 const& _rhs);

KT_FORCEINLINE Vec2 operator* (Vec2 const& _lhs, float const _f);

KT_FORCEINLINE Vec2 operator/ (Vec2 const& _lhs, float const _f);

KT_FORCEINLINE Vec2 operator* (float const _f, Vec2 const& _rhs);

KT_FORCEINLINE Vec2 operator/ (float const _f, Vec2 const& _rhs);

KT_FORCEINLINE Vec2 operator-(Vec2 const& _v);

KT_FORCEINLINE Vec2 operator+(Vec2 const& _lhs, Vec2 const& _rhs);

KT_FORCEINLINE Vec2 operator-(Vec2 const& _lhs, Vec2 const& _rhs);

KT_FORCEINLINE float LengthSq(Vec2 const& _vec);

KT_FORCEINLINE float Length(Vec2 const& _vec);

KT_FORCEINLINE Vec2 Normalize(Vec2 const& _vec);

KT_FORCEINLINE float Distance(Vec2 const& _lhs, Vec2 const& _rhs);

KT_FORCEINLINE float DistanceSq(Vec2 const& _lhs, Vec2 const& _rhs);

KT_FORCEINLINE float Dot(Vec2 const& _lhs, Vec2 const& _rhs);

KT_FORCEINLINE float Cross(Vec2 const& _lhs, Vec2 const& _rhs);

KT_FORCEINLINE bool operator==(Vec2 const& _v1, Vec2 const& _v2);

KT_FORCEINLINE bool operator!=(Vec2 const& _v1, Vec2 const& _v2);

KT_FORCEINLINE Vec2 Lerp(Vec2 const& _lhs, Vec2 const& _rhs, float const _t);

KT_FORCEINLINE Vec2 Min(Vec2 const& _a, Vec2 const& _b);

KT_FORCEINLINE Vec2 Max(Vec2 const& _a, Vec2 const& _b);

KT_FORCEINLINE Vec2 Clamp(Vec2 const& _a, Vec2 const& _min, Vec2 const& _max);

KT_FORCEINLINE Vec2 Abs(Vec2 const& _v);

KT_FORCEINLINE Vec2 PerpCCW(Vec2 const& _v);

KT_FORCEINLINE Vec2 PerpCW(Vec2 const& _v);

KT_FORCEINLINE float MaxComponent(Vec2 const& _v);


} 

#include "inl/Vec2.inl"