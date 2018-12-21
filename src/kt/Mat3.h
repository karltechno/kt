#pragma once
#include "kt.h"
#include "Vec3.h"

namespace kt
{

struct Mat3
{
	Mat3() = default;

	KT_FORCEINLINE static Mat3 Identity();
	KT_FORCEINLINE static Mat3 RotX(float const _theta);
	KT_FORCEINLINE static Mat3 RotY(float const _theta);
	KT_FORCEINLINE static Mat3 RotZ(float const _theta);
	KT_FORCEINLINE static Mat3 RotZYX(float const _x, float const _y, float const _z);
	KT_FORCEINLINE static Mat3 Rot(Vec3 const& _aroundUnitVec, float const _theta);


	KT_FORCEINLINE Mat3(Vec3 const& _col0, Vec3 const& _col1, Vec3 const& _col2);

	KT_FORCEINLINE float& AtColRow(uint32_t const _col, uint32_t const _row);
	KT_FORCEINLINE float AtColRow(uint32_t const _col, uint32_t const _row) const;

	KT_FORCEINLINE Vec3& operator[](uint32_t const _col);
	KT_FORCEINLINE Vec3 const& operator[](uint32_t const _col) const;

	KT_FORCEINLINE void SetCol(uint32_t const _colIdx, Vec3 const& _vec);

	KT_FORCEINLINE float* Data();
	KT_FORCEINLINE float const* Data() const;

	Vec3 m_cols[3];
};

KT_FORCEINLINE Mat3 Transpose(Mat3 const& _mat);

KT_FORCEINLINE Mat3 Inverse(Mat3 const& _mat);

KT_FORCEINLINE float Determinant(Mat3 const& _mat);

KT_FORCEINLINE Mat3 InverseOrtho(Mat3 const& _mat);

KT_FORCEINLINE Mat3 operator*(Mat3 const& _a, Mat3 const& _b);

KT_FORCEINLINE Vec3 operator*(Mat3 const& _mat, Vec3 const& _vec);

KT_FORCEINLINE Mat3 Mul(Mat3 const& _a, Mat3 const& _b);

KT_FORCEINLINE Vec3 Mul(Mat3 const& _mat, Vec3 const& _vec);

KT_FORCEINLINE Vec3 MulByInverse(Mat3 const& _mat, Vec3 const& _vec);

}

#include "inl/Mat3.inl"