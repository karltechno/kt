#pragma once
#include "kt.h"
#include "Vec2.h"

namespace kt
{

struct Mat2
{
	KT_FORCEINLINE static Mat2 Identity();
	KT_FORCEINLINE static Mat2 Rot(float const _theta);

	Mat2() = default;

	KT_FORCEINLINE Mat2(Vec2 const& _col0, Vec2 const& _col1);

	KT_FORCEINLINE float& AtColRow(uint32_t const _col, uint32_t const _row);
	KT_FORCEINLINE float const& AtColRow(uint32_t const _col, uint32_t const _row) const;

	KT_FORCEINLINE Vec2& operator[](uint32_t const _col);
	KT_FORCEINLINE Vec2 const& operator[](uint32_t const _col) const;

	KT_FORCEINLINE void SetCol(uint32_t const _colIdx, Vec2 const& _vec);

	KT_FORCEINLINE float* Data();
	KT_FORCEINLINE float const* Data() const;

	Vec2 m_cols[2];
};

KT_FORCEINLINE Mat2 Transpose(Mat2 const& _mat);

KT_FORCEINLINE Mat2 Inverse(Mat2 const& _mat);

KT_FORCEINLINE float Determinant(Mat2 const& _mat);

KT_FORCEINLINE Mat2 InverseOrtho(Mat2 const& _mat);

KT_FORCEINLINE Mat2 operator*(Mat2 const& _a, Mat2 const& _b);

KT_FORCEINLINE Vec2 operator*(Mat2 const& _mat, Vec2 const& _vec);

KT_FORCEINLINE Mat2 Mul(Mat2 const& _a, Mat2 const& _b);

KT_FORCEINLINE Vec2 Mul(Mat2 const& _mat, Vec2 const& _vec);

KT_FORCEINLINE Vec2 MulByInverse(Mat2 const& _mat, Vec2 const& _vec);

}

#include "inl/Mat2.inl"