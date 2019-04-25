#pragma once

namespace kt
{

KT_FORCEINLINE Quat NLerp(Quat const& _a, Quat const& _b, float const _t)
{
	Quat ret;
	ret.x = Lerp(_a.x, _b.x, _t);
	ret.y = Lerp(_a.y, _b.y, _t);
	ret.z = Lerp(_a.z, _b.z, _t);
	ret.w = Lerp(_a.w, _b.w, _t);
	return Normalize(ret);
}


KT_FORCEINLINE Quat ToQuat(Mat3 const& _mat)
{
	Quat q;
	if (_mat[2][2] < 0.0f)
	{
		if (_mat[0][0] > _mat[1][1])
		{
			float const t = 1 + _mat[0][0] - _mat[1][1] - _mat[2][2];
			q = Quat(t, _mat[0][1] + _mat[1][0], _mat[2][0] + _mat[0][2], _mat[1][2] - _mat[2][1]);
		}
		else
		{
			float const t = 1 - _mat[0][0] + _mat[1][1] - _mat[2][2];
			q = Quat(_mat[0][1] + _mat[1][0], t, _mat[1][2] + _mat[2][1], _mat[2][0] - _mat[0][2]);
		}
	}
	else
	{
		if (_mat[0][0] < -_mat[1][1])
		{
			float const t = 1 - _mat[0][0] - _mat[1][1] + _mat[2][2];
			q = Quat(_mat[2][0] + _mat[0][2], _mat[1][2] + _mat[2][1], t, _mat[0][1] - _mat[1][0]);
		}
		else
		{
			float const t = 1 + _mat[0][0] + _mat[1][1] + _mat[2][2];
			q = Quat(_mat[1][2] - _mat[2][1], _mat[2][0] - _mat[0][2], _mat[0][1] - _mat[1][0], t);
		}
	}
	return Normalize(q);
}



KT_FORCEINLINE Quat Quat::Identity()
{
	return Quat(0.0f, 0.0f, 0.0f, 1.0f);
}


KT_FORCEINLINE Quat& Quat::operator*=(Quat const& _other)
{
	*this = *this * _other;
	return *this;
}


KT_FORCEINLINE void Quat::FromNormalizedAxisAngle(kt::Vec3 const& _axis, float const _angle)
{
	KT_ASSERT((Abs(Length(_axis)) - 1.0f) <= 0.001f);

	float const angleOver2 = (_angle * 0.5f);
	float const s = Sin(angleOver2);
	float const c = Cos(angleOver2);
	x = _axis.x * s;
	y = _axis.y * s;
	z = _axis.z * s;
	w = c;
}


KT_FORCEINLINE Quat Conjugate(Quat const& _q)
{
	Quat ret;
	ret.x = -_q.x;
	ret.y = -_q.y;
	ret.z = -_q.z;
	ret.w = _q.w;
	return ret;
}

KT_FORCEINLINE Quat Inverse(Quat const& _q)
{
	return Conjugate(_q);
}

KT_FORCEINLINE float Length(Quat const& _q)
{
	return Sqrt(_q.x * _q.x + _q.y * _q.y + _q.z * _q.z + _q.w * _q.w);
}

KT_FORCEINLINE Quat Normalize(Quat const& _q)
{
	float const len = Length(_q);
	KT_ASSERT(len > 0.001f);
	float const invLen = 1.0f / len;
	return Quat(_q.x * invLen, _q.y * invLen, _q.z * invLen, _q.w * invLen);
}

KT_FORCEINLINE float Dot(Quat const& _lhs, Quat const& _rhs)
{
	return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z + _lhs.w * _rhs.w;
}

KT_FORCEINLINE Vec3 MulByInverse(Quat const& _q, Vec3 const& _v)
{
	return Conjugate(_q) * _v;
}

KT_FORCEINLINE Vec3 Mul(Quat const& _q, Vec3 const& _v)
{
	Vec3 const quatImag = Vec3(_q.x, _q.y, _q.z);
	Vec3 const t = 2.0f * Cross(quatImag, _v);
	return _v + _q.w * t + Cross(quatImag, t);
}

KT_FORCEINLINE Quat QuatBetweenUnitVecs(Vec3 const& _lhs, Vec3 const& _rhs)
{
	float w = 1.0f + Dot(_lhs, _rhs);

	Vec3 axis;

	if (w < 0.00001f)
	{
		// lhs ~= -rhs (w ~= cos(pi) + 1)
		w = 0.0f;
		axis = Abs(_lhs.x) > Abs(_lhs.z) ? Vec3(-_lhs.y, _lhs.x, 0.0f) : Vec3(0.0f, -_lhs.z, _lhs.y);
	}
	else
	{
		axis = Cross(_lhs, _rhs);
	}

	return Normalize(Quat(axis, w));
}


KT_FORCEINLINE Mat4 ToMat4(Quat const& _q)
{
	KT_ASSERT(Abs(1.0f - Length(_q)) < 0.001f);

	Mat4 m;
	float const x2 = _q.x + _q.x;
	float const y2 = _q.y + _q.y;
	float const z2 = _q.z + _q.z;
	float const yy2 = _q.y * y2;
	float const xy2 = _q.x * y2;
	float const xz2 = _q.x * z2;
	float const yz2 = _q.y * z2;
	float const zz2 = _q.z * z2;
	float const wz2 = _q.w * z2;
	float const wy2 = _q.w * y2;
	float const wx2 = _q.w * x2;
	float const xx2 = _q.x * x2;
	m[0][0] = -yy2 - zz2 + 1.0f;
	m[0][1] = xy2 + wz2;
	m[0][2] = xz2 - wy2;
	m[0][3] = 0.0f;
	m[1][0] = xy2 - wz2;
	m[1][1] = -xx2 - zz2 + 1.0f;
	m[1][2] = yz2 + wx2;
	m[1][3] = 0.0f;
	m[2][0] = xz2 + wy2;
	m[2][1] = yz2 - wx2;
	m[2][2] = -xx2 - yy2 + 1.0f;
	m[2][3] = 0.0f;
	m[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}
KT_FORCEINLINE Mat3 ToMat3(Quat const& _q)
{
	KT_ASSERT(Abs(1.0f - Length(_q)) < 0.001f);

	Mat3 m;
	float const x2 = _q.x + _q.x;
	float const y2 = _q.y + _q.y;
	float const z2 = _q.z + _q.z;
	float const yy2 = _q.y * y2;
	float const xy2 = _q.x * y2;
	float const xz2 = _q.x * z2;
	float const yz2 = _q.y * z2;
	float const zz2 = _q.z * z2;
	float const wz2 = _q.w * z2;
	float const wy2 = _q.w * y2;
	float const wx2 = _q.w * x2;
	float const xx2 = _q.x * x2;
	m[0][0] = -yy2 - zz2 + 1.0f;
	m[0][1] = xy2 + wz2;
	m[0][2] = xz2 - wy2;
	m[1][0] = xy2 - wz2;
	m[1][1] = -xx2 - zz2 + 1.0f;
	m[1][2] = yz2 + wx2;
	m[2][0] = xz2 + wy2;
	m[2][1] = yz2 - wx2;
	m[2][2] = -xx2 - yy2 + 1.0f;
	return m;
}


KT_FORCEINLINE Quat::Quat(Vec3 const& _xyz, float const _w)
	: x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_w)
{
}

KT_FORCEINLINE Quat Mul(Quat const& _lhs, Quat const& _rhs)
{
	Quat ret;
	ret.x = _lhs.x * _rhs.w + _lhs.y * _rhs.z - _lhs.z * _rhs.y + _lhs.w * _rhs.x;
	ret.y = -_lhs.x * _rhs.z + _lhs.y * _rhs.w + _lhs.z * _rhs.x + _lhs.w * _rhs.y;
	ret.z = _lhs.x * _rhs.y - _lhs.y * _rhs.x + _lhs.z * _rhs.w + _lhs.w * _rhs.z;
	ret.w = -_lhs.x * _rhs.x - _lhs.y * _rhs.y - _lhs.z * _rhs.z + _lhs.w * _rhs.w;
	return ret;
}

KT_FORCEINLINE Quat operator*(Quat const& _lhs, Quat const& _rhs)
{
	return Mul(_lhs, _rhs);
}

KT_FORCEINLINE Vec3 operator*(Quat const& _q, Vec3 const& _v)
{
	return Mul(_q, _v);
}


}