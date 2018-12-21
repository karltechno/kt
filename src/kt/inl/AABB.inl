#include "../AABB.h"

namespace kt
{

KT_FORCEINLINE AABB Union(AABB const& _lhs, AABB const& _rhs)
{
	return AABB(Min(_lhs.m_min, _rhs.m_min), Max(_lhs.m_max, _rhs.m_max));
}


KT_FORCEINLINE void AABB::AddPoint(Vec3 const& _p)
{
	m_max = Max(m_max, _p);
	m_min = Min(m_min, _p);
}

KT_FORCEINLINE Vec3 AABB::Center() const
{
	return (m_max - m_min) * 0.5f + m_min;
}

KT_FORCEINLINE Vec3 AABB::HalfSize() const
{
	return (m_max - m_min) * 0.5f;
}


}