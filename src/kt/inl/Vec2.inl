#pragma once

namespace kt
{

KT_FORCEINLINE Vec2& Vec2::operator=(Vec2 const& _v)
{
	x = _v.x;
	y = _v.y;
	return *this;
}


KT_FORCEINLINE Vec2& Vec2::operator*= (float const _f)
{
	x *= _f;
	y *= _f;
	return *this;
}

KT_FORCEINLINE Vec2& Vec2::operator-= (float const _f)
{
	x -= _f;
	y -= _f;
	return *this;
}

KT_FORCEINLINE Vec2& Vec2::operator-= (Vec2 const& _other)
{
	x -= _other.x;
	y -= _other.y;
	return *this;
}

KT_FORCEINLINE Vec2& Vec2::operator+= (Vec2 const& _other)
{
	x += _other.x;
	y += _other.y;
	return *this;
}

KT_FORCEINLINE Vec2& Vec2::operator/= (float const _f)
{
	const float inverse = 1.0f / _f;
	x *= inverse;
	y *= inverse;
	return *this;
}

KT_FORCEINLINE float& Vec2::operator[](uint32_t const _index)
{
	KT_ASSERT(_index < 2);
	return (&x)[_index];
}


KT_FORCEINLINE float const& Vec2::operator[](uint32_t const _index) const
{
	KT_ASSERT(_index < 2);
	return (&x)[_index];
}


KT_FORCEINLINE Vec2 operator* (Vec2 const& _lhs, float const _f)
{
	return Vec2(_lhs.x * _f, _lhs.y * _f);
}

KT_FORCEINLINE Vec2 operator/ (Vec2 const& _lhs, float const _f)
{
	float const invF = 1.0f / _f;
	return Vec2(_lhs.x * invF, _lhs.y * invF);
}

KT_FORCEINLINE Vec2 operator* (float const _f, Vec2 const& _rhs)
{
	return Vec2(_rhs.x * _f, _rhs.y * _f);
}

KT_FORCEINLINE Vec2 operator/ (float const _f, Vec2 const& _rhs)
{
	float const invF = 1.0f / _f;
	return Vec2(_rhs.x * invF, _rhs.y * invF);
}


KT_FORCEINLINE Vec2 operator-(Vec2 const& _v)
{
	return Vec2(-_v.x, -_v.y);
}


KT_FORCEINLINE Vec2 operator+(Vec2 const& _lhs, Vec2 const& _rhs)
{
	return Vec2(_lhs.x + _rhs.x, _lhs.y + _rhs.y);
}

KT_FORCEINLINE Vec2 operator-(Vec2 const& _lhs, Vec2 const& _rhs)
{
	return Vec2(_lhs.x - _rhs.x, _lhs.y - _rhs.y);
}

KT_FORCEINLINE float LengthSq(Vec2 const& _vec)
{
	return _vec.x * _vec.x + _vec.y * _vec.y;
}

KT_FORCEINLINE float Length(Vec2 const& _vec)
{
	return Sqrt(_vec.x * _vec.x + _vec.y * _vec.y);
}

KT_FORCEINLINE Vec2 Normalize(Vec2 const& _vec)
{
	const float mag = Length(_vec);
	if (mag > 0.0f)
	{
		Vec2 ret(_vec);
		const float inverseMag = 1.0f / mag;
		ret.x *= inverseMag;
		ret.y *= inverseMag;
		return ret;
	}

	return Vec2(0.0f, 1.0f);
}


KT_FORCEINLINE float Distance(Vec2 const& _lhs, Vec2 const& _rhs)
{
	return Length(_lhs - _rhs);
}

KT_FORCEINLINE float DistanceSq(Vec2 const& _lhs, Vec2 const& _rhs)
{
	return LengthSq(_lhs - _rhs);
}

KT_FORCEINLINE float Dot(Vec2 const& _lhs, Vec2 const& _rhs)
{
	return _lhs.x * _rhs.x + _lhs.y * _rhs.y;
}

KT_FORCEINLINE float Cross(Vec2 const& _lhs, Vec2 const& _rhs)
{
	return _lhs.x * _rhs.y - _lhs.y * _rhs.x;
}

KT_FORCEINLINE bool operator==(Vec2 const& _v1, Vec2 const& _v2)
{
	return _v1.x == _v2.x && _v1.y == _v2.y;
}


KT_FORCEINLINE bool operator!=(Vec2 const& _v1, Vec2 const& _v2)
{
	return !(_v1 == _v2);
}


KT_FORCEINLINE Vec2 Lerp(Vec2 const& _lhs, Vec2 const& _rhs, float const _t)
{
	return Vec2
	(
		Lerp(_lhs.x, _rhs.x, _t),
		Lerp(_lhs.y, _rhs.y, _t)
	);
}

KT_FORCEINLINE Vec2 Min(Vec2 const& _a, Vec2 const& _b)
{
	return Vec2(Min(_a.x, _b.x), Min(_a.y, _b.y));
}

KT_FORCEINLINE Vec2 Max(Vec2 const& _a, Vec2 const& _b)
{
	return Vec2(Max(_a.x, _b.x), Max(_a.y, _b.y));
}

KT_FORCEINLINE Vec2 Clamp(Vec2 const& _a, Vec2 const& _min, Vec2 const& _max)
{
	return Vec2(Clamp(_a.x, _min.x, _max.x), Clamp(_a.y, _min.y, _max.y));
}

KT_FORCEINLINE Vec2 Abs(Vec2 const& _v)
{
	return Vec2(Abs(_v.x), Abs(_v.y));
}

KT_FORCEINLINE Vec2 PerpCCW(Vec2 const& _v)
{
	return Vec2(-_v.y, _v.x);
}

KT_FORCEINLINE Vec2 PerpCW(Vec2 const& _v)
{
	return Vec2(_v.y, -_v.x);
}


KT_FORCEINLINE float MaxComponent(Vec2 const& _v)
{
	return kt::Max(_v.x, _v.y);
}

} 