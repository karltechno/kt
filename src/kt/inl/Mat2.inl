#pragma once

namespace kt
{


KT_FORCEINLINE Mat2 Mat2::Identity()
{
	return Mat2(Vec2(1.0f, 0.0f), Vec2(0.0f, 1.0f));
}


KT_FORCEINLINE Mat2 Mat2::Rot(float const _theta)
{
	float c, s;
	c = Cos(_theta);
	s = Sin(_theta);
	
	return Mat2
	(
		Vec2(c, s),
		Vec2(-s, c)
	);
}


KT_FORCEINLINE Mat2::Mat2(Vec2 const& _col0, Vec2 const& _col1)
{
	m_cols[0] = _col0;
	m_cols[1] = _col1;
}

KT_FORCEINLINE float& Mat2::AtColRow(uint32_t const _col, uint32_t const _row)
{
	KT_ASSERT(_col < 2);
	KT_ASSERT(_row < 2);
	return m_cols[_col][_row];
}

KT_FORCEINLINE float const& Mat2::AtColRow(uint32_t const _col, uint32_t const _row) const
{
	KT_ASSERT(_col < 2);
	KT_ASSERT(_row < 2);
	return m_cols[_col][_row];
}

KT_FORCEINLINE Vec2& Mat2::operator[](uint32_t const _col)
{
	KT_ASSERT(_col < 2);
	return m_cols[_col];
}

KT_FORCEINLINE Vec2 const & Mat2::operator[](uint32_t const _col) const
{
	KT_ASSERT(_col < 2);
	return m_cols[_col];
}

KT_FORCEINLINE void Mat2::SetCol(uint32_t const _colIdx, Vec2 const& _vec)
{
	KT_ASSERT(_colIdx < 2);
	m_cols[_colIdx] = _vec;
}

KT_FORCEINLINE float* Mat2::Data()
{
	return (float*)m_cols;
}

KT_FORCEINLINE float const* Mat2::Data() const
{
	return (float*)m_cols;
}

KT_FORCEINLINE Mat2 Transpose(Mat2 const & _mat)
{
	return Mat2(Vec2(_mat.AtColRow(0, 0), _mat.AtColRow(0, 1)), Vec2(_mat.AtColRow(0, 1), _mat.AtColRow(1, 1)));
}

KT_FORCEINLINE Mat2 Inverse(Mat2 const & _mat)
{
	float const det = Determinant(_mat);
	KT_ASSERT(det > 0.0f);
	float const invDet = 1.0f / det;

	return Mat2(Vec2(invDet * _mat[1][1], invDet * -_mat[0][1]), Vec2(invDet * -_mat[1][0], invDet * _mat[0][0]));
}

KT_FORCEINLINE float Determinant(Mat2 const& _mat)
{
	return _mat[0][0] * _mat[1][1] - _mat[1][0] * _mat[0][1];
}

KT_FORCEINLINE Mat2 InverseOrtho(Mat2 const & _mat)
{
	return Transpose(_mat);
}

KT_FORCEINLINE Mat2 operator*(Mat2 const & _a, Mat2 const & _b)
{
	return Mul(_a, _b);
}

KT_FORCEINLINE Vec2 operator*(Mat2 const & _mat, Vec2 const & _vec)
{
	return Mul(_mat, _vec);
}

KT_FORCEINLINE Mat2 Mul(Mat2 const& _a, Mat2 const& _b)
{
	Mat2 ret;
	ret.m_cols[0] = _a.m_cols[0] * _b.m_cols[0][0] + _a.m_cols[1] * _b.m_cols[0][1];
	ret.m_cols[1] = _a.m_cols[0] * _b.m_cols[1][0] + _a.m_cols[1] * _b.m_cols[1][1];
	return ret;
}

KT_FORCEINLINE Vec2 Mul(Mat2 const & _mat, Vec2 const & _vec)
{
	return _mat.m_cols[0] * _vec.x + _mat.m_cols[1] * _vec.y;
}

KT_FORCEINLINE Vec2 MulByInverse(Mat2 const & _mat, Vec2 const & _vec)
{
	return Mul(Inverse(_mat), _vec);
}


}