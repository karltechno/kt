#include "..\Vec3.h"
#pragma once

namespace kt
{

KT_FORCEINLINE Vec3& Vec3::operator*= (float const _f)
{
	x *= _f;
	y *= _f;
	z *= _f;
	return *this;
}

KT_FORCEINLINE Vec3& Vec3::operator-= (float const _f)
{
	x -= _f;
	y -= _f;
	z -= _f;
	return *this;
}

KT_FORCEINLINE Vec3& Vec3::operator-= (Vec3 const& _other)
{
	x -= _other.x;
	y -= _other.y;
	z -= _other.z;
	return *this;
}

KT_FORCEINLINE Vec3& Vec3::operator+= (Vec3 const& _other)
{
	x += _other.x;
	y += _other.y;
	z += _other.z;
	return *this;
}

KT_FORCEINLINE Vec3& Vec3::operator/= (float const _f)
{
	const float inverse = 1.0f / _f;
	x *= inverse;
	y *= inverse;
	z *= inverse;
	return *this;
}

KT_FORCEINLINE float& Vec3::operator[](uint32_t const _index)
{
	KT_ASSERT(_index < 3);
	return (&x)[_index];
}


KT_FORCEINLINE float Vec3::operator[](uint32_t const _index) const
{
	KT_ASSERT(_index < 3);
	return (&x)[_index];
}


KT_FORCEINLINE Vec3 operator* (Vec3 const& _lhs, float const _f)
{
	return Vec3(_lhs.x * _f, _lhs.y * _f, _lhs.z * _f);
}

KT_FORCEINLINE Vec3 operator/ (Vec3 const& _lhs, float const _f)
{
	float const invF = 1.0f / _f;
	return Vec3(_lhs.x * invF, _lhs.y * invF, _lhs.z * invF);
}

KT_FORCEINLINE Vec3 operator* (float const _f, Vec3 const& _rhs)
{
	return Vec3(_rhs.x * _f, _rhs.y * _f, _rhs.z * _f);
}

KT_FORCEINLINE Vec3 operator/ (float const _f, Vec3 const& _rhs)
{
	float const invF = 1.0f / _f;
	return Vec3(_rhs.x * invF, _rhs.y * invF, _rhs.z * invF);
}


KT_FORCEINLINE Vec3 operator-(Vec3 const& _v)
{
	return Vec3(-_v.x, -_v.y, -_v.z);
}


KT_FORCEINLINE Vec3 operator+(Vec3 const& _lhs, Vec3 const& _rhs)
{
	return Vec3(_lhs.x + _rhs.x, _lhs.y + _rhs.y, _lhs.z + _rhs.z);
}


KT_FORCEINLINE Vec3 operator-(Vec3 const& _lhs, Vec3 const& _rhs)
{
	return Vec3(_lhs.x - _rhs.x, _lhs.y - _rhs.y, _lhs.z - _rhs.z);
}


KT_FORCEINLINE float LengthSq(Vec3 const& _vec)
{
	return _vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z;
}

KT_FORCEINLINE float Length(Vec3 const& _vec)
{
	return Sqrt(_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
}

KT_FORCEINLINE Vec3 Normalize(Vec3 const& _vec)
{
	const float mag = Length(_vec);
	if (mag > 0.0f)
	{
		Vec3 ret(_vec);
		const float inverseMag = 1.0f / mag;
		ret.x *= inverseMag;
		ret.y *= inverseMag;
		ret.z *= inverseMag;
		return ret;
	}

	return Vec3(0.0f, 0.0f, 1.0f);
}


KT_FORCEINLINE float Distance(Vec3 const& _lhs, Vec3 const& _rhs)
{
	return Length(_lhs - _rhs);
}

KT_FORCEINLINE float DistanceSq(Vec3 const& _lhs, Vec3 const& _rhs)
{
	return LengthSq(_lhs - _rhs);
}

KT_FORCEINLINE float Dot(Vec3 const& _lhs, Vec3 const& _rhs)
{
	return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z;
}

KT_FORCEINLINE Vec3 Cross(Vec3 const& _lhs, Vec3 const& _rhs)
{
	return Vec3
	(
		_lhs.y * _rhs.z - _lhs.z * _rhs.y,
		_lhs.z * _rhs.x - _lhs.x * _rhs.z,
		_lhs.x * _rhs.y - _lhs.y * _rhs.x
	);
}

KT_FORCEINLINE bool operator==(Vec3 const& _v1, Vec3 const& _v2)
{
	return _v1.x == _v2.x && _v1.y == _v2.y && _v1.z == _v2.z;
}


KT_FORCEINLINE bool operator!=(Vec3 const& _v1, Vec3 const& _v2)
{
	return !(_v1 == _v2);
}


KT_FORCEINLINE Vec3 Lerp(Vec3 const& _lhs, Vec3 const& _rhs, float const _t)
{
	return Vec3
	(
		Lerp(_lhs.x, _rhs.x, _t),
		Lerp(_lhs.y, _rhs.y, _t),
		Lerp(_lhs.z, _rhs.z, _t)
	);
}

KT_FORCEINLINE Vec3 Min(Vec3 const& _a, Vec3 const& _b)
{
	return Vec3(Min(_a.x, _b.x), Min(_a.y, _b.y), Min(_a.z, _b.z));
}

KT_FORCEINLINE Vec3 Max(Vec3 const& _a, Vec3 const& _b)
{
	return Vec3(Max(_a.x, _b.x), Max(_a.y, _b.y), Max(_a.z, _b.z));
}

KT_FORCEINLINE Vec3 Clamp(Vec3 const& _a, Vec3 const& _min, Vec3 const& _max)
{
	return Vec3(Clamp(_a.x, _min.x, _max.x), Clamp(_a.y, _min.y, _max.y), Clamp(_a.z, _min.z, _max.z));
}

KT_FORCEINLINE Vec3 Abs(Vec3 const& _v)
{
	return Vec3(Abs(_v.x), Abs(_v.y), Abs(_v.z));
}

KT_FORCEINLINE float MaxComponent(Vec3 const& _v)
{
	return Max(Max(_v.x, _v.y), _v.z);
}


} 