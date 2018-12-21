#pragma once
#include "MathUtil.h"
#include "Vec3.h"
#include "Mat3.h"
#include "Mat4.h"

namespace kt
{

struct Quat
{
	Quat() {}

	KT_FORCEINLINE static Quat Identity();

	KT_FORCEINLINE Quat(float const _x, float const _y, float const _z, float const _w)
		: x(_x), y(_y), z(_z), w(_w)
	{}

	KT_FORCEINLINE Quat(kt::Vec3 const& _xyz, float const _w);

	KT_FORCEINLINE void FromNormalizedAxisAngle(kt::Vec3 const& _axis, float const _angle);

	KT_FORCEINLINE Quat& operator*=(Quat const& _other);

	float x;
	float y;
	float z;
	float w;
};

KT_FORCEINLINE float Length(Quat const& _q);
KT_FORCEINLINE Quat NLerp(Quat const& _a, Quat const& _b, float const _t);
KT_FORCEINLINE Quat Conjugate(Quat const& _q);
KT_FORCEINLINE Quat Inverse(Quat const& _q);
KT_FORCEINLINE Quat Normalize(Quat const& _q);
KT_FORCEINLINE float Dot(Quat const& _lhs, Quat const& _rhs);
KT_FORCEINLINE Quat QuatBetweenUnitVecs(Vec3 const& _lhs, Vec3 const& _rhs);
KT_FORCEINLINE Quat ToQuat(Mat3 const& _mat);
KT_FORCEINLINE Mat3 ToMat3(Quat const& _q);
KT_FORCEINLINE Mat4 ToMat4(Quat const& _q);

KT_FORCEINLINE Vec3 MulByInverse(Quat const& _q, Vec3 const& _v);
KT_FORCEINLINE Vec3 Mul(Quat const& _q, Vec3 const& _v);
KT_FORCEINLINE Quat Mul(Quat const& _lhs, Quat const& _rhs);
KT_FORCEINLINE Quat operator*(Quat const& _lhs, Quat const& _rhs);
KT_FORCEINLINE Vec3 operator*(Quat const& _q, Vec3 const& _v);

}

#include "inl/Quat.inl"