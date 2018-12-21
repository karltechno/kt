#pragma once

namespace kt
{


KT_FORCEINLINE Mat3 Mat3::Identity()
{
	return Mat3
	(
		Vec3(1.0f, 0.0f, 0.0f),
		Vec3(0.0f, 1.0f, 0.0f),
		Vec3(0.0f, 0.0f, 1.0f)
	);
}

KT_FORCEINLINE Mat3 Mat3::RotX(float const _theta)
{
	float const s = Sin(_theta);
	float const c = Cos(_theta);
	return Mat3
	(
		Vec3(1.0f, 0.0f, 0.0f),
		Vec3(0.0f, c, s),
		Vec3(0.0f, -s, c)
	);
}

KT_FORCEINLINE Mat3 Mat3::RotY(float const _theta)
{
	float const s = Sin(_theta);
	float const c = Cos(_theta);
	return Mat3
	(
		Vec3(c, 0.0f, -s),
		Vec3(0.0f, 1.0f, 0.0f),
		Vec3(s, 0.0f, c)
	);
}

KT_FORCEINLINE Mat3 Mat3::RotZ(float const _theta)
{
	float const s = Sin(_theta);
	float const c = Cos(_theta);
	return Mat3
	(
		Vec3(c, s, 0.0f),
		Vec3(-s, c, 0.0f),
		Vec3(0.0f, 0.0f, 1.0f)
	);
}

KT_FORCEINLINE Mat3 Mat3::RotZYX(float const _x, float const _y, float const _z)
{
	float const sX = Sin(_x);
	float const cX = Cos(_x);
	float const sY = Sin(_y);
	float const cY = Cos(_y);
	float const sZ = Sin(_z);
	float const cZ = Cos(_z);

	return Mat3
	(
		Vec3((cZ * cY), (sZ * cY), -sY),
		Vec3((((cZ * sY) * sX) - (sZ * cX)), (((sY * sZ) * sX) + (cZ * cX)), (cY * sX)),
		Vec3((((cZ * sY) * cX) + (sZ * sX)), (((sZ * sY) * cX) - (cZ * sX)), (cY * cX))
	);
}

KT_FORCEINLINE Mat3 Mat3::Rot(Vec3 const& _aroundUnitVec, float const _theta)
{
	KT_ASSERT(Abs(1.0f - Length(_aroundUnitVec)) < 0.001f);
	
	float const s = Sin(_theta);
	float const c = Cos(_theta);
	float const xy = (_aroundUnitVec.x * _aroundUnitVec.y);
	float const yz = (_aroundUnitVec.y * _aroundUnitVec.z);
	float const zx = (_aroundUnitVec.z * _aroundUnitVec.x);
	float const oneMinusC = (1.0f - c);
	return Mat3
	(
		Vec3((((_aroundUnitVec.x * _aroundUnitVec.x) * oneMinusC) + c), ((xy * oneMinusC) + (_aroundUnitVec.z * s)), ((zx * oneMinusC) - (_aroundUnitVec.y * s))),
		Vec3(((xy * oneMinusC) - (_aroundUnitVec.z * s)), (((_aroundUnitVec.y * _aroundUnitVec.y) * oneMinusC) + c), ((yz * oneMinusC) + (_aroundUnitVec.x * s))),
		Vec3(((zx * oneMinusC) + (_aroundUnitVec.y * s)), ((yz * oneMinusC) - (_aroundUnitVec.x * s)), (((_aroundUnitVec.z * _aroundUnitVec.z) * oneMinusC) + c))
	);
}

Mat3::Mat3(Vec3 const& _col0, Vec3 const& _col1, Vec3 const& _col2)
{
	m_cols[0] = _col0;
	m_cols[1] = _col1;
	m_cols[2] = _col2;
}

KT_FORCEINLINE float& Mat3::AtColRow(uint32_t const _col, uint32_t const _row)
{
	KT_ASSERT(_col < 3);
	KT_ASSERT(_row < 3);
	return m_cols[_col][_row];
}

KT_FORCEINLINE float Mat3::AtColRow(uint32_t const _col, uint32_t const _row) const
{
	KT_ASSERT(_col < 3);
	KT_ASSERT(_row < 3);
	return m_cols[_col][_row];
}

KT_FORCEINLINE Vec3& Mat3::operator[](uint32_t const _col)
{
	KT_ASSERT(_col < 3);
	return m_cols[_col];
}

KT_FORCEINLINE Vec3 const& Mat3::operator[](uint32_t const _col) const
{
	KT_ASSERT(_col < 3);
	return m_cols[_col];
}

KT_FORCEINLINE void Mat3::SetCol(uint32_t const _colIdx, Vec3 const& _vec)
{
	KT_ASSERT(_colIdx < 3);
	m_cols[_colIdx] = _vec;
}

KT_FORCEINLINE float* Mat3::Data()
{
	return (float*)m_cols;
}

KT_FORCEINLINE float const* Mat3::Data() const
{
	return (float*)m_cols;
}

KT_FORCEINLINE Mat3 Transpose(Mat3 const & _mat)
{
	return Mat3
	(
		Vec3(_mat[0][0], _mat[1][0], _mat[2][0]),
		Vec3(_mat[0][1], _mat[1][1], _mat[2][1]),
		Vec3(_mat[0][2], _mat[1][2], _mat[2][2])
	);
}

KT_FORCEINLINE Mat3 Inverse(Mat3 const& _mat)
{
	Vec3 const c0 = Cross(_mat[1], _mat[2]);
	Vec3 const c1 = Cross(_mat[2], _mat[0]);
	Vec3 const c2 = Cross(_mat[0], _mat[1]);
	float const invD = (1.0f / Dot(_mat[2], c2));
	// TODO: Check det
	return Mat3
	(
		Vec3((c0[0] * invD), (c1[0] * invD), (c2[0] * invD)),
		Vec3((c0[1] * invD), (c1[1] * invD), (c2[1] * invD)),
		Vec3((c0[2] * invD), (c1[2] * invD), (c2[2] * invD))
	);

}

KT_FORCEINLINE float Determinant(Mat3 const& _mat)
{
	return Dot(_mat[0], Cross(_mat[1], _mat[2]));
}

KT_FORCEINLINE Mat3 InverseOrtho(Mat3 const& _mat)
{
	return Transpose(_mat);
}

KT_FORCEINLINE Mat3 operator*(Mat3 const& _a, Mat3 const& _b)
{
	return Mul(_a, _b);
}

KT_FORCEINLINE Vec3 operator*(Mat3 const& _mat, Vec3 const& _vec)
{
	return Mul(_mat, _vec);
}

KT_FORCEINLINE Mat3 Mul(Mat3 const& _a, Mat3 const& _b)
{
	Mat3 ret;
	ret.m_cols[0] = _a[0] * _b[0][0] + _a[1] * _b[0][1] + _a[2] * _b[0][2];
	ret.m_cols[1] = _a[0] * _b[1][0] + _a[1] * _b[1][1] + _a[2] * _b[1][2];
	ret.m_cols[2] = _a[0] * _b[2][0] + _a[1] * _b[2][1] + _a[2] * _b[2][2];
	return ret;
}

KT_FORCEINLINE Vec3 Mul(Mat3 const& _mat, Vec3 const & _vec)
{
	return _mat[0] * _vec.x + _mat[1] * _vec.y + _mat[2] * _vec.z;
}

KT_FORCEINLINE Vec3 MulByInverse(Mat3 const& _mat, Vec3 const& _vec)
{
	return Mul(Inverse(_mat), _vec);
}

}