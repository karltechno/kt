#include "../AABB.h"

#include <float.h>

#include "../Mat3.h"
#include "../Mat4.h"

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


KT_FORCEINLINE AABB AABB::FloatMax()
{
	return AABB(kt::Vec3(FLT_MAX), kt::Vec3(-FLT_MAX));
}


// See rtcd page 87
KT_FORCEINLINE AABB AABB::Transformed(kt::Mat3 const& _mat, kt::Vec3 const& _p) const
{
	AABB ret;
	ret.m_min = _p;
	ret.m_max = _p;

	for (uint32_t i = 0; i < 3; ++i)
	{
		for (uint32_t j = 0; j < 3; ++j)
		{
			float const e = _mat[j][i] * m_min[j];
			float const f = _mat[j][i] * m_max[j];
		
			if (e < f)
			{
				ret.m_min[i] += e;
				ret.m_max[i] += f;
			}
			else
			{
				ret.m_min[i] += f;
				ret.m_max[i] += e;
			}
		}
	}

	return ret;
}

// See rtcd page 87
KT_FORCEINLINE AABB AABB::Transformed(kt::Mat4 const& _mat) const
{
	AABB ret;
	ret.m_min = _mat.GetPos();
	ret.m_max = _mat.GetPos();

	for (uint32_t i = 0; i < 3; ++i)
	{
		for (uint32_t j = 0; j < 3; ++j)
		{
			float const e = _mat[j][i] * m_min[j];
			float const f = _mat[j][i] * m_max[j];

			if (e < f)
			{
				ret.m_min[i] += e;
				ret.m_max[i] += f;
			}
			else
			{
				ret.m_min[i] += f;
				ret.m_max[i] += e;
			}
		}
	}

	return ret;
}


}