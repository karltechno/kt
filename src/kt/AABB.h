#pragma once
#include "Vec3.h"

namespace kt
{

struct Quat;
struct Mat3;
struct Mat4;


struct AABB
{
	AABB() = default;

	AABB(Vec3 const& _min, Vec3 const& _max)
		: m_min(_min), m_max(_max) {}

	KT_FORCEINLINE static AABB FloatMax();

	KT_FORCEINLINE void AddPoint(Vec3 const& _p);

	KT_FORCEINLINE Vec3 Center() const;
	KT_FORCEINLINE Vec3 HalfSize() const;
	
	Vec3 m_min;
	Vec3 m_max;
};

KT_FORCEINLINE AABB Union(AABB const& _lhs, AABB const& _rhs);

}

#include "inl/AABB.inl"