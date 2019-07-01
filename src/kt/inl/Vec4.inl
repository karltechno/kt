#pragma once
#include "../Vec4.h"

namespace kt
{

KT_FORCEINLINE Vec3::Vec3(Vec4 const & _v)
	: x(_v.x), y(_v.y), z(_v.z)
{

}

KT_FORCEINLINE Vec4& Vec4::operator=(Vec4 const& _v)
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
	w = _v.w;
	return *this;
}


KT_FORCEINLINE Vec4& Vec4::operator*= (float const _f)
{
	x *= _f;
	y *= _f;
	z *= _f;
	w *= _f;
	return *this;
}

KT_FORCEINLINE Vec4& Vec4::operator-= (float const _f)
{
	x -= _f;
	y -= _f;
	z -= _f;
	w -= _f;
	return *this;
}

KT_FORCEINLINE Vec4& Vec4::operator-= (Vec4 const& _other)
{
	x -= _other.x;
	y -= _other.y;
	z -= _other.z;
	w -= _other.w;
	return *this;
}

KT_FORCEINLINE Vec4& Vec4::operator+= (Vec4 const& _other)
{
	x += _other.x;
	y += _other.y;
	z += _other.z;
	w += _other.w;
	return *this;
}

KT_FORCEINLINE Vec4& Vec4::operator/= (float const _f)
{
	const float inverse = 1.0f / _f;
	x *= inverse;
	y *= inverse;
	z *= inverse;
	w *= inverse;
	return *this;
}

KT_FORCEINLINE float& Vec4::operator[](uint32_t const _index)
{
	KT_ASSERT(_index < 4);
	return (&x)[_index];
}


KT_FORCEINLINE float const& Vec4::operator[](uint32_t const _index) const
{
	KT_ASSERT(_index < 4);
	return (&x)[_index];
}

KT_FORCEINLINE Vec4 operator* (Vec4 const& _lhs, Vec4 const& _rhs)
{
	return Vec4(_lhs.x * _rhs.x, _lhs.y * _rhs.y, _lhs.z * _rhs.z, _lhs.w * _rhs.w);
}


KT_FORCEINLINE Vec4 operator* (Vec4 const& _lhs, float const _f)
{
	return Vec4(_lhs.x * _f, _lhs.y * _f, _lhs.z * _f, _lhs.w * _f);
}

KT_FORCEINLINE Vec4 operator/ (Vec4 const& _lhs, float const _f)
{
	float const invF = 1.0f / _f;
	return Vec4(_lhs.x * invF, _lhs.y * invF, _lhs.z * invF, _lhs.w * invF);
}

KT_FORCEINLINE Vec4 operator* (float const _f, Vec4 const& _rhs)
{
	return Vec4(_rhs.x * _f, _rhs.y * _f, _rhs.z * _f, _rhs.w * _f);
}

KT_FORCEINLINE Vec4 operator/ (float const _f, Vec4 const& _rhs)
{
	float const invF = 1.0f / _f;
	return Vec4(_rhs.x * invF, _rhs.y * invF, _rhs.z * invF, _rhs.w * invF);
}


KT_FORCEINLINE Vec4 operator-(Vec4 const& _v)
{
	return Vec4(-_v.x, -_v.y, -_v.z, -_v.w);
}


KT_FORCEINLINE Vec4 operator+(Vec4 const& _lhs, Vec4 const& _rhs)
{
	return Vec4(_lhs.x + _rhs.x, _lhs.y + _rhs.y, _lhs.z + _rhs.z, _lhs.w + _rhs.w);
}


KT_FORCEINLINE Vec4 operator-(Vec4 const& _lhs, Vec4 const& _rhs)
{
	return Vec4(_lhs.x - _rhs.x, _lhs.y - _rhs.y, _lhs.z - _rhs.z, _lhs.w - _rhs.w);
}


KT_FORCEINLINE float LengthSq(Vec4 const& _vec)
{
	return _vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z + _vec.w * _vec.w;
}

KT_FORCEINLINE float Length(Vec4 const& _vec)
{
	return Sqrt(_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z + _vec.w * _vec.w);
}

KT_FORCEINLINE Vec4 Normalize(Vec4 const& _vec)
{
	const float mag = Length(_vec);
	if (mag > 0.0001f)
	{
		Vec4 ret(_vec);
		const float inverseMag = 1.0f / mag;
		ret.x *= inverseMag;
		ret.y *= inverseMag;
		ret.z *= inverseMag;
		ret.w *= inverseMag;
		return ret;
	}

	return Vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


KT_FORCEINLINE float Distance(Vec4 const& _lhs, Vec4 const& _rhs)
{
	return Length(_lhs - _rhs);
}

KT_FORCEINLINE float DistanceSq(Vec4 const& _lhs, Vec4 const& _rhs)
{
	return LengthSq(_lhs - _rhs);
}

KT_FORCEINLINE float Dot(Vec4 const& _lhs, Vec4 const& _rhs)
{
	return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z + _lhs.w * _rhs.w;
}


KT_FORCEINLINE bool operator==(Vec4 const& _v1, Vec4 const& _v2)
{
	return _v1.x == _v2.x && _v1.y == _v2.y && _v1.z == _v2.z && _v1.w == _v2.w;
}


KT_FORCEINLINE bool operator!=(Vec4 const& _v1, Vec4 const& _v2)
{
	return !(_v1 == _v2);
}


KT_FORCEINLINE Vec4 Lerp(Vec4 const& _lhs, Vec4 const& _rhs, float const _t)
{
	return Vec4
	(
		Lerp(_lhs.x, _rhs.x, _t),
		Lerp(_lhs.y, _rhs.y, _t),
		Lerp(_lhs.z, _rhs.z, _t),
		Lerp(_lhs.w, _rhs.w, _t)
	);
}

KT_FORCEINLINE Vec4 Min(Vec4 const& _a, Vec4 const& _b)
{
	return Vec4(Min(_a.x, _b.x), Min(_a.y, _b.y), Min(_a.z, _b.z), Min(_a.w, _b.w));
}

KT_FORCEINLINE Vec4 Max(Vec4 const& _a, Vec4 const& _b)
{
	return Vec4(Max(_a.x, _b.x), Max(_a.y, _b.y), Max(_a.z, _b.z), Max(_a.w, _b.w));
}

KT_FORCEINLINE Vec4 Clamp(Vec4 const& _a, Vec4 const& _min, Vec4 const& _max)
{
	return Vec4(Clamp(_a.x, _min.x, _max.x), Clamp(_a.y, _min.y, _max.y), Clamp(_a.z, _min.z, _max.z), Clamp(_a.w, _min.w, _max.w));
}

KT_FORCEINLINE Vec4 Abs(Vec4 const& _v)
{
	return Vec4(Abs(_v.x), Abs(_v.y), Abs(_v.z), Abs(_v.w));
}

KT_FORCEINLINE float MaxComponent(Vec4 const& _v)
{
	return kt::Max(kt::Max(_v.x, _v.y), kt::Max(_v.z, _v.w));
}

}