#pragma once
#include <string.h>

namespace kt
{


KT_FORCEINLINE Mat4::Mat4(Vec4 const & _col0, Vec4 const & _col1, Vec4 const & _col2, Vec4 const & _col3)
{
	m_cols[0] = _col0;
	m_cols[1] = _col1;
	m_cols[2] = _col2;
	m_cols[3] = _col3;
}

KT_FORCEINLINE Mat4 Mat4::OrthographicLH_ZO(float const _left, float const _right, float _bottom, float _top, float _nearZ, float _farZ)
{
	Mat4 m;
	::memset(&m, 0, sizeof(Mat4));

	float recipW = 1.0f / (_right - _left);
	float recipH = 1.0f / (_top - _bottom);
	float range = 1.0f / (_farZ - _nearZ);

	m[0][0] = recipW * 2.0f;
	m[1][1] = recipH * 2.0f;
	m[2][2] = range;
	m[3][0] = (_left + _right) * -recipW;
	m[3][1] = (_top + _bottom) * -recipH;
	m[3][2] = -range * _nearZ;
	m[3][3] = 1.0f;
	return m;
}

KT_FORCEINLINE Mat4 Mat4::OrthographicRH_ZO(float const _left, float const _right, float _bottom, float _top, float _nearZ, float _farZ)
{
	Mat4 m;
	::memset(&m, 0, sizeof(Mat4));

	float recipW = 1.0f / (_right - _left);
	float recipH = 1.0f / (_top - _bottom);
	float range = 1.0f / (_nearZ - _farZ);

	m[0][0] = recipW * 2.0f;
	m[1][1] = recipH * 2.0f;
	m[2][2] = range;
	m[3][0] = (_left + _right) * -recipW;
	m[3][1] = (_top + _bottom) * -recipH;
	m[3][2] = range * _nearZ;
	m[3][3] = 1.0f;
	return m;
}

KT_FORCEINLINE Mat4::Mat4(Mat3 const& _mat3, Vec3 const& _pos)
{
	m_cols[0] = Vec4(_mat3[0], 0.0f);
	m_cols[1] = Vec4(_mat3[1], 0.0f);
	m_cols[2] = Vec4(_mat3[2], 0.0f);
	m_cols[3] = Vec4(_pos, 1.0f);
}


KT_FORCEINLINE float& Mat4::AtColRow(uint32_t const _col, uint32_t const _row)
{
	KT_ASSERT(_row < 4 && _col < 4);
	return Data()[_col * 4 + _row];
}

KT_FORCEINLINE float const& Mat4::AtColRow(uint32_t const _col, uint32_t const _row) const
{
	KT_ASSERT(_row < 4 && _col < 4);
	return Data()[_col * 4 + _row];
}

KT_FORCEINLINE Vec4& Mat4::operator[](uint32_t const _col)
{
	KT_ASSERT(_col < 4);
	return m_cols[_col];
}

KT_FORCEINLINE Vec4 const& Mat4::operator[](uint32_t const _col) const
{
	KT_ASSERT(_col < 4);
	return m_cols[_col];
}

KT_FORCEINLINE void Mat4::SetCol(uint32_t const _colIdx, Vec4 const& _vec)
{
	KT_ASSERT(_colIdx < 4);
	m_cols[_colIdx] = _vec;
}

KT_FORCEINLINE void Mat4::SetPos(Vec3 const & _pos)
{
	m_cols[3] = Vec4(_pos, 1.0f);
}

KT_FORCEINLINE void Mat4::SetRot(Mat3 const & _mat3)
{
	m_cols[0] = Vec4(_mat3[0], 0.0f);
	m_cols[1] = Vec4(_mat3[1], 0.0f);
	m_cols[2] = Vec4(_mat3[2], 0.0f);
}

KT_FORCEINLINE Mat4 Transpose(Mat4 const& _mat)
{
	return Mat4
	(
		Vec4(_mat.m_cols[0][0], _mat[1][0], _mat.m_cols[2][0], _mat.m_cols[3][0]),
		Vec4(_mat.m_cols[0][1], _mat[1][1], _mat.m_cols[2][1], _mat.m_cols[3][1]),
		Vec4(_mat.m_cols[0][2], _mat[1][2], _mat.m_cols[2][2], _mat.m_cols[3][2]),
		Vec4(_mat.m_cols[0][3], _mat[1][3], _mat.m_cols[2][3], _mat.m_cols[3][3])
	);
}


KT_FORCEINLINE Mat4 Inverse(Mat4 const& _mat)
{
	Mat4 ret;

	float temp0 = ((_mat[2][2] * _mat[0][3]) - (_mat[0][2] * _mat[2][3]));
	float temp1 = ((_mat[3][2] * _mat[1][3]) - (_mat[1][2] * _mat[3][3]));
	float temp2 = ((_mat[0][1] * _mat[2][2]) - (_mat[2][1] * _mat[0][2]));
	float temp3 = ((_mat[1][1] * _mat[3][2]) - (_mat[3][1] * _mat[1][2]));
	float temp4 = ((_mat[2][1] * _mat[0][3]) - (_mat[0][1] * _mat[2][3]));
	float temp5 = ((_mat[3][1] * _mat[1][3]) - (_mat[1][1] * _mat[3][3]));

	ret[0][0] = (((_mat[2][1] * temp1) - (_mat[2][3] * temp3)) - (_mat[2][2] * temp5));
	ret[0][1] = (((_mat[3][1] * temp0) - (_mat[3][3] * temp2)) - (_mat[3][2] * temp4));
	ret[0][2] = (((_mat[0][3] * temp3) + (_mat[0][2] * temp5)) - (_mat[0][1] * temp1));
	ret[0][3] = (((_mat[1][3] * temp2) + (_mat[1][2] * temp4)) - (_mat[1][1] * temp0));

	float const invDet = 1.0f / ((_mat[0][0] * ret.m_cols[0][0]) + (_mat[1][0] * ret.m_cols[0][1]) + (_mat[2][0] * ret[0][2]) + (_mat[3][0] * ret[0][3]));

	ret[1] = Vec4(_mat[2][0] * temp1, _mat[3][0] * temp0, _mat[0][0] * temp1, _mat[1][0] * temp0);
	ret[2] = Vec4(_mat[2][0] * temp5, _mat[3][0] * temp4, _mat[0][0] * temp5, _mat[1][0] * temp4);
	ret[3] = Vec4(_mat[2][0] * temp3, _mat[3][0] * temp2, _mat[0][0] * temp3, _mat[1][0] * temp2);

	temp0 = (_mat[2][0] * _mat[0][1]) - (_mat[0][0] * _mat[2][1]);
	temp1 = (_mat[3][0] * _mat[1][1]) - (_mat[1][0] * _mat[3][1]);
	temp2 = (_mat[2][0] * _mat[0][3]) - (_mat[0][0] * _mat[2][3]);
	temp3 = (_mat[3][0] * _mat[1][3]) - (_mat[1][0] * _mat[3][3]);
	temp4 = (_mat[2][0] * _mat[0][2]) - (_mat[0][0] * _mat[2][2]);
	temp5 = (_mat[3][0] * _mat[1][2]) - (_mat[1][0] * _mat[3][2]);

	ret[2][0] = ((_mat[2][3] * temp1) - (_mat[2][1] * temp3)) + ret[2][0];
	ret[2][1] = ((_mat[3][3] * temp0) - (_mat[3][1] * temp2)) + ret[2][1];
	ret[2][2] = ((_mat[0][1] * temp3) - (_mat[0][3] * temp1)) - ret[2][2];
	ret[2][3] = ((_mat[1][1] * temp2) - (_mat[1][3] * temp0)) - ret[2][3];
	ret[3][0] = ((_mat[2][1] * temp5) - (_mat[2][2] * temp1)) + ret[3][0];
	ret[3][1] = ((_mat[3][1] * temp4) - (_mat[3][2] * temp0)) + ret[3][1];
	ret[3][2] = ((_mat[0][2] * temp1) - (_mat[0][1] * temp5)) - ret[3][2];
	ret[3][3] = ((_mat[1][2] * temp0) - (_mat[1][1] * temp4)) - ret[3][3];
	ret[1][0] = ((_mat[2][2] * temp3) - (_mat[2][3] * temp5)) - ret[1][0];
	ret[1][1] = ((_mat[3][2] * temp2) - (_mat[3][3] * temp4)) - ret[1][1];
	ret[1][2] = ((_mat[0][3] * temp5) - (_mat[0][2] * temp3)) + ret[1][2];
	ret[1][3] = ((_mat[1][3] * temp4) - (_mat[1][2] * temp2)) + ret[1][3];

	for (uint32_t i = 0; i < 16; ++i)
	{
		ret.Data()[i] *= invDet;
	}

	return ret;
}

KT_FORCEINLINE Mat4 Mat4::RotX(float const _theta)
{
	float const s = Sin(_theta);
	float const c = Cos(_theta);
	return Mat4
	(
		Vec4(1.0f, 0.0f, 0.0f, 0.0f),
		Vec4(0.0f, c, s, 0.0f),
		Vec4(0.0f, -s, c, 0.0f),
		Vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

KT_FORCEINLINE Mat4 Mat4::RotY(float const _theta)
{
	float const s = Sin(_theta);
	float const c = Cos(_theta);
	return Mat4
	(
		Vec4(c, 0.0f, -s, 0.0f),
		Vec4(0.0f, 1.0f, 0.0f, 0.0f),
		Vec4(s, 0.0f, c, 0.0f),
		Vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

KT_FORCEINLINE Mat4 Mat4::RotZ(float const _theta)
{
	float const s = Sin(_theta);
	float const c = Cos(_theta);
	return Mat4
	(
		Vec4(c, s, 0.0f, 0.0f),
		Vec4(-s, c, 0.0f, 0.0f),
		Vec4(0.0f, 0.0f, 1.0f, 0.0f),
		Vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

KT_FORCEINLINE Mat4 Mat4::RotZYX(float const _x, float const _y, float const _z)
{
	float const sX = Sin(_x);
	float const cX = Cos(_x);
	float const sY = Sin(_y);
	float const cY = Cos(_y);
	float const sZ = Sin(_z);
	float const cZ = Cos(_z);

	return Mat4
	(
		Vec4((cZ * cY), (sZ * cY), -sY, 0.0f),
		Vec4((((cZ * sY) * sX) - (sZ * cX)), (((sY * sZ) * sX) + (cZ * cX)), (cY * sX), 0.0f),
		Vec4((((cZ * sY) * cX) + (sZ * sX)), (((sZ * sY) * cX) - (cZ * sX)), (cY * cX), 0.0f),
		Vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

KT_FORCEINLINE Mat4 Mat4::Rot(Vec3 const& _aroundUnitVec, float const _theta)
{
	KT_ASSERT(Abs(1.0f - Length(_aroundUnitVec)) < 0.001f);

	float const s = Sin(_theta);
	float const c = Cos(_theta);
	float const xy = (_aroundUnitVec.x * _aroundUnitVec.y);
	float const yz = (_aroundUnitVec.y * _aroundUnitVec.z);
	float const zx = (_aroundUnitVec.z * _aroundUnitVec.x);
	float const oneMinusC = (1.0f - c);
	return Mat4
	(
		Vec4((((_aroundUnitVec.x * _aroundUnitVec.x) * oneMinusC) + c), ((xy * oneMinusC) + (_aroundUnitVec.z * s)), ((zx * oneMinusC) - (_aroundUnitVec.y * s)), 0.0f),
		Vec4(((xy * oneMinusC) - (_aroundUnitVec.z * s)), (((_aroundUnitVec.y * _aroundUnitVec.y) * oneMinusC) + c), ((yz * oneMinusC) + (_aroundUnitVec.x * s)), 0.0f),
		Vec4(((zx * oneMinusC) + (_aroundUnitVec.y * s)), ((yz * oneMinusC) - (_aroundUnitVec.x * s)), (((_aroundUnitVec.z * _aroundUnitVec.z) * oneMinusC) + c), 0.0f),
		Vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

KT_FORCEINLINE Mat4 InverseOrthoAffine(Mat4 const& _mat)
{
	Vec4 posPart = -_mat.m_cols[3];

	Mat4 ret
	(
		Vec4(_mat.m_cols[0][0], _mat.m_cols[1][0], _mat.m_cols[2][0], 0.0f),
		Vec4(_mat.m_cols[0][1], _mat.m_cols[1][1], _mat.m_cols[2][1], 0.0f),
		Vec4(_mat.m_cols[0][2], _mat.m_cols[1][2], _mat.m_cols[2][2], 0.0f),
		Vec4(_mat.m_cols[0][3], _mat.m_cols[1][3], _mat.m_cols[2][3], 1.0f)
	);

	ret.m_cols[3] = Vec4(posPart.x * ret.m_cols[0] + posPart.y * ret.m_cols[1] + posPart.z * ret.m_cols[2]);
	ret.m_cols[3].w = 1.0f;
	return ret;
}

KT_FORCEINLINE Mat4 operator*(Mat4 const & _a, Mat4 const & _b)
{
	return Mul(_a, _b);
}

KT_FORCEINLINE Vec3 operator*(Mat4 const & _mat, Vec3 const & _vec)
{
	return MulPoint(_mat, _vec);
}

KT_FORCEINLINE Mat4 Mul(Mat4 const& _a, Mat4 const& _b)
{
	Mat4 ret;
	ret.m_cols[0] = _a * _b.m_cols[0];
	ret.m_cols[1] = _a * _b.m_cols[1];
	ret.m_cols[2] = _a * _b.m_cols[2];
	ret.m_cols[3] = _a * _b.m_cols[3];
	return ret;
}

KT_FORCEINLINE Vec3 MulPoint(Mat4 const& _mat, Vec3 const& _vec)
{
	Vec4 ret = _mat.m_cols[0] * _vec.x;
	ret += _mat.m_cols[1] * _vec.y;
	ret += _mat.m_cols[2] * _vec.z;
	ret += _mat.m_cols[3];
	return Vec3(ret.x, ret.y, ret.z);
}

KT_FORCEINLINE Vec3 MulDir(Mat4 const& _mat, Vec3 const& _vec)
{
	Vec4 ret = _mat.m_cols[0] * _vec.x;
	ret += _mat.m_cols[1] * _vec.y;
	ret += _mat.m_cols[2] * _vec.z;
	return Vec3(ret.x, ret.y, ret.z);
}

KT_FORCEINLINE Vec3 MulByInverse(Mat4 const& _mat, Vec3 const& _vec)
{
	// Todo optimise
	return MulPoint(InverseOrthoAffine(_mat), _vec);
}


KT_FORCEINLINE Vec4 Mul(Mat4 const& _mat, Vec4 const& _vec)
{
	Vec4 ret = _mat.m_cols[0] * _vec.x;
	ret += _mat.m_cols[1] * _vec.y;
	ret += _mat.m_cols[2] * _vec.z;
	ret += _mat.m_cols[3] * _vec.w;
	return ret;
}

KT_FORCEINLINE Vec4 operator*(Mat4 const& _mat, Vec4 const& _vec)
{
	return Mul(_mat, _vec);
}


KT_FORCEINLINE Mat4 Mat4::PerspectiveLH_ZO(float const _fov, float const _aspect, float const _near, float const _far)
{
	Mat4 ret;
	::memset(&ret, 0, sizeof(Mat4));

	float const f = Tan(kPiOverTwo - (_fov * 0.5f)); // cot(_fov * 0.5)
	float const w = f / _aspect;
	float const range = _far / (_far - _near);

	ret.m_cols[0][0] = w;
	ret.m_cols[1][1] = f;
	ret.m_cols[2][2] = range;
	ret.m_cols[2][3] = 1.0f;
	ret.m_cols[3][2] = -range * _near;
	return ret;
}

KT_FORCEINLINE Mat4 Mat4::PerspectiveRH_ZO(float const _fov, float const _aspect, float const _near, float const _far)
{
	Mat4 ret;
	::memset(&ret, 0, sizeof(Mat4));

	float const f = Tan(kPiOverTwo - (_fov * 0.5f)); // cot(_fov * 0.5)
	float const w = f / _aspect;
	float const range = _far / (_near - _far);

	ret.m_cols[0][0] = w;
	ret.m_cols[1][1] = f;
	ret.m_cols[2][2] = range;
	ret.m_cols[2][3] = -1.0f;
	ret.m_cols[3][2] = range * _near;
	return ret;
}

KT_FORCEINLINE Mat4 Mat4::Identity()
{
	Mat4 ret;
	::memset(ret.Data(), 0, sizeof(Mat4));
	ret.m_cols[0][0] = 1.0f;
	ret.m_cols[1][1] = 1.0f;
	ret.m_cols[2][2] = 1.0f;
	ret.m_cols[3][3] = 1.0f;
	return ret;
}


KT_FORCEINLINE Mat4 Mat4::Translation(Vec3 const& _translation)
{
	Mat4 ret = Mat4::Identity();

	ret.m_cols[3] = Vec4(_translation, 1.0f);
	return ret;
}


KT_FORCEINLINE Mat4 Mat4::LookAtLH(Vec3 const& _origin, Vec3 const& _lookDir, Vec3 const& _up)
{
	Vec3 const normLookDir = Normalize(_lookDir);
	Vec3 const camRight = Normalize(Cross(_up, normLookDir));
	Vec3 const camUp = Normalize(Cross(normLookDir, camRight));

	Mat4 ret;
	ret.m_cols[0][0] = camRight.x;
	ret.m_cols[1][0] = camRight.y;
	ret.m_cols[2][0] = camRight.z;

	ret.m_cols[0][1] = camUp.x;
	ret.m_cols[1][1] = camUp.y;
	ret.m_cols[2][1] = camUp.z;

	ret.m_cols[0][2] = normLookDir.x;
	ret.m_cols[1][2] = normLookDir.y;
	ret.m_cols[2][2] = normLookDir.z;

	ret.m_cols[0][3] = 0.0f;
	ret.m_cols[1][3] = 0.0f;
	ret.m_cols[2][3] = 0.0f;

	ret.m_cols[3].x = -Dot(camRight, _origin);
	ret.m_cols[3].y = -Dot(camUp, _origin);
	ret.m_cols[3].z = -Dot(normLookDir, _origin);
	ret.m_cols[3].w = 1.0f;

	return ret;
}

KT_FORCEINLINE Mat4 Mat4::LookAtRH(Vec3 const& _origin, Vec3 const& _lookDir, Vec3 const& _up)
{
	return LookAtLH(_origin, -_lookDir, _up);
}


KT_FORCEINLINE float* Mat4::Data()
{
	return (float*)m_cols;
}

KT_FORCEINLINE float const* Mat4::Data() const
{
	return (float const*)m_cols;
}

KT_FORCEINLINE float Determinant(Mat4 const& _mat)
{
	float const temp0 = ((_mat[2][2] * _mat[0][3]) - (_mat[0][2] * _mat[2][3]));
	float const temp1 = ((_mat[3][2] * _mat[1][3]) - (_mat[1][2] * _mat[3][3]));
	float const temp2 = ((_mat[0][1] * _mat[2][2]) - (_mat[2][1] * _mat[0][2]));
	float const temp3 = ((_mat[1][1] * _mat[3][2]) - (_mat[3][1] * _mat[1][2]));
	float const temp4 = ((_mat[2][1] * _mat[0][3]) - (_mat[0][1] * _mat[2][3]));
	float const temp5 = ((_mat[3][1] * _mat[1][3]) - (_mat[1][1] * _mat[3][3]));
	float const detX = (((_mat[2][1] * temp1) - (_mat[2][3] * temp3)) - (_mat[2][2] * temp5));
	float const detY = (((_mat[3][1] * temp0) - (_mat[3][3] * temp2)) - (_mat[3][2] * temp4));
	float const detZ = (((_mat[0][3] * temp3) + (_mat[0][2] * temp5)) - (_mat[0][1] * temp1));
	float const detW = (((_mat[1][3] * temp2) + (_mat[1][2] * temp4)) - (_mat[1][1] * temp0));
	return (_mat[0][0] * detX) + (_mat[1][0] * detY) + (_mat[2][0] * detZ) + (_mat[3][0] * detW);
}



}
