#pragma once
#include "MathUtil.h"
#include "Vec4.h"
#include "Vec3.h"
#include "Mat3.h"

namespace kt
{

// A single precision 4x4 matrix class. In most cases this is intended for use in the typical
// computer graphics context (ie 3x3 rot/scale mtx and translation). 

struct Mat4
{

	KT_FORCEINLINE static Mat4 Identity();
	KT_FORCEINLINE static Mat4 PerspectiveLH_ZO(float const _fov, float const _aspect, float const _near, float const _far);
	KT_FORCEINLINE static Mat4 OrthographicLH_ZO(float const _left, float const _right, float _bottom, float _top, float _nearZ, float _farZ);
	KT_FORCEINLINE static Mat4 Translation(Vec3 const& _translation);
	KT_FORCEINLINE static Mat4 LookAtLH(Vec3 const& _origin, Vec3 const& _lookDir, Vec3 const& _up = Vec3(0.0f, 1.0f, 0.0f));

	KT_FORCEINLINE static Mat4 RotX(float const _theta);
	KT_FORCEINLINE static Mat4 RotY(float const _theta);
	KT_FORCEINLINE static Mat4 RotZ(float const _theta);
	KT_FORCEINLINE static Mat4 RotZYX(float const _x, float const _y, float const _z);
	KT_FORCEINLINE static Mat4 Rot(Vec3 const& _aroundUnitVec, float const _theta);

	Mat4() = default;

	KT_FORCEINLINE Mat4(Vec4 const& _col0, Vec4 const& _col1, Vec4 const& _col2, Vec4 const& _col3);
	KT_FORCEINLINE Mat4(Mat3 const& _mat3, Vec3 const& _pos);

	KT_FORCEINLINE float& AtColRow(uint32_t const _col, uint32_t const _row);
	KT_FORCEINLINE float const& AtColRow(uint32_t const _col, uint32_t const _row) const;

	KT_FORCEINLINE Vec4& operator[](uint32_t const _col);
	KT_FORCEINLINE Vec4 const& operator[](uint32_t const _col) const;

	KT_FORCEINLINE void SetCol(uint32_t const _colIdx, Vec4 const& _vec);
	KT_FORCEINLINE void SetPos(Vec3 const& _pos);
	KT_FORCEINLINE void SetRot(Mat3 const& _mat3);

	KT_FORCEINLINE float* Data();
	KT_FORCEINLINE float const* Data() const;

	Vec4 m_cols[4];
};

KT_FORCEINLINE Mat4 Transpose(Mat4 const& _mat);

KT_FORCEINLINE Mat4 Inverse(Mat4 const& _mat);

KT_FORCEINLINE float Determinant(Mat4 const& _mat);

KT_FORCEINLINE Mat4 InverseOrthoAffine(Mat4 const& _mat);

KT_FORCEINLINE Mat4 operator*(Mat4 const& _a, Mat4 const& _b);

KT_FORCEINLINE Vec3 operator*(Mat4 const& _mat, Vec3 const& _vec);

KT_FORCEINLINE Vec4 operator*(Mat4 const& _mat, Vec4 const& _vec);

KT_FORCEINLINE Mat4 Mul(Mat4 const& _a, Mat4 const& _b);

KT_FORCEINLINE Vec3 MulPoint(Mat4 const& _mat, Vec3 const& _vec);

KT_FORCEINLINE Vec4 Mul(Mat4 const& _mat, Vec4 const& _vec);

KT_FORCEINLINE Vec3 MulDir(Mat4 const& _mat, Vec3 const& _vec);

KT_FORCEINLINE Vec3 MulByInverse(Mat4 const& _mat, Vec3 const& _vec);


} 

#include "inl/Mat4.inl"