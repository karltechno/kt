#include "catch.hpp"

#include <kt/kt.h>
#include <kt/Vec2.h>
#include <kt/Vec3.h>
#include <kt/Vec4.h>
#include <kt/Mat4.h>
#include <kt/Mat3.h>
#include <kt/Quat.h>

bool RelError(float const _lhs, float const _rhs, float const _error)
{
	float const diff = kt::Abs(_lhs - _rhs);
	float const err = _error * kt::Max(1.0f, kt::Max(kt::Abs(_lhs), kt::Abs(_rhs)));
	return diff <= err;
}

bool RelError(kt::Vec2 const& _lhs, kt::Vec2 const& _rhs, float const _error)
{
	return RelError(_lhs.x, _rhs.x, _error) && RelError(_lhs.y, _rhs.y, _error);
}

bool RelError(kt::Vec3 const& _lhs, kt::Vec3 const& _rhs, float const _error)
{
	return RelError(_lhs.x, _rhs.x, _error) && RelError(_lhs.y, _rhs.y, _error) && RelError(_lhs.z, _rhs.z, _error);
}

bool RelError(kt::Vec4 const& _lhs, kt::Vec4 const& _rhs, float const _error)
{
	return RelError(_lhs.x, _rhs.x, _error) && RelError(_lhs.y, _rhs.y, _error) && RelError(_lhs.z, _rhs.z, _error) && RelError(_lhs.w, _rhs.w, _error);
}

bool RelError(kt::Mat4 const& _lhs, kt::Mat4 const& _rhs, float const _error)
{
	for (uint32_t i = 0; i < 4; ++i)
	{
		if (!RelError(_lhs.m_cols[0], _rhs.m_cols[0], _error))
		{
			return false;
		}
	}
	return true;
}

template <typename T>
bool AbsError(T const& _lhs, T const& _rhs, float const _error)
{
	T const diff = kt::Abs(_lhs - _rhs);
	return kt::MaxComponent(diff) <= _error;
}

TEST_CASE("Basic Vec4", "")
{
	{
		kt::Vec4 const v1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		kt::Vec4 const v2 = { 2.0f, 3.0f, 4.0f, 5.0f };
		REQUIRE(AbsError(v2 - v1, kt::Vec4(1.0f), 0.001f));
	}

	{
		kt::Vec4 const v1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		REQUIRE(AbsError(v1 * 10.0f, kt::Vec4(10.0f, 20.0f, 30.0f, 40.0f), 0.001f));
	}

	{
		kt::Vec4 const v1 = { 1.0f, 2.0f, 3.0f, 4.0f };
		REQUIRE(AbsError(v1 * 10.0f, kt::Vec4(10.0f, 20.0f, 30.0f, 40.0f), 0.001f));
	}

	{
		kt::Vec4 const v1 = { 10.0f, 20.0f, 30.0f, 40.0f };
		REQUIRE(AbsError(v1 / 10.0f, kt::Vec4(1.0f, 2.0f, 3.0f, 4.0f), 0.001f));
	}
}


TEST_CASE("Basic Vec3", "")
{
	{
		kt::Vec3 const v1 = { 1.0f, 2.0f, 3.0f };
		kt::Vec3 const v2 = { 2.0f, 3.0f, 4.0f };
		REQUIRE(AbsError(v2 - v1, kt::Vec3(1.0f), 0.001f));
	}

	{
		kt::Vec3 const v1 = { 1.0f, 2.0f, 3.0f };
		REQUIRE(AbsError(v1 * 10.0f, kt::Vec3(10.0f, 20.0f, 30.0f), 0.001f));
	}

	{
		kt::Vec3 const v1 = { 1.0f, 2.0f, 3.0f };
		REQUIRE(AbsError(v1 * 10.0f, kt::Vec3(10.0f, 20.0f, 30.0f), 0.001f));
	}

	{
		kt::Vec3 const v1 = { 10.0f, 20.0f, 30.0f };
		REQUIRE(AbsError(v1 / 10.0f, kt::Vec3(1.0f, 2.0f, 3.0f), 0.001f));
	}
}

TEST_CASE("Basic Vec2", "")
{
	{
		kt::Vec2 const v1 = { 1.0f, 2.0f };
		kt::Vec2 const v2 = { 2.0f, 3.0f };
		REQUIRE(AbsError(v2 - v1, kt::Vec2(1.0f), 0.001f));
	}

	{
		kt::Vec2 const v1 = { 1.0f, 2.0f };
		REQUIRE(AbsError(v1 * 10.0f, kt::Vec2(10.0f, 20.0f), 0.001f));
	}

	{
		kt::Vec2 const v1 = { 1.0f, 2.0f };
		REQUIRE(AbsError(v1 * 10.0f, kt::Vec2(10.0f, 20.0f), 0.001f));
	}

	{
		kt::Vec2 const v1 = { 10.0f, 20.0f };
		REQUIRE(AbsError(v1 / 10.0f, kt::Vec2(1.0f, 2.0f), 0.001f));
	}
}

TEST_CASE("Mat4 affine inverse", "")
{
	{
		kt::Mat4 mat = kt::Mat4::RotZYX(1.5f, 2.5f, 2.0f);
		mat.SetPos(kt::Vec3(2.0f, 4.0f, 1.0f));

		kt::Vec3 const initialV = { 2.0f, 4.0f, 45.0f };

		kt::Vec3 transformed = mat * initialV;
		transformed = kt::InverseOrthoAffine(mat) * transformed;
		REQUIRE(RelError(transformed, initialV, 0.0001f));
	}
}

TEST_CASE("Mat4 full inverse", "")
{
	{
		kt::Mat4 mat = kt::Mat4::RotZYX(4.5f, 2.5f, 2.0f);
		mat.SetPos(kt::Vec3(2.0f, 4.0f, 1.0f));

		kt::Vec3 const initialV = { 2.0f, 4.0f, 45.0f };

		kt::Vec3 transformed = mat * initialV;
		transformed = kt::Inverse(mat) * transformed;
		REQUIRE(RelError(transformed, initialV, 0.0001f));
	}
}

TEST_CASE("Mat3 ortho inverse", "")
{
	{
		kt::Mat3 mat = kt::Mat3::RotZYX(1.5f, 3.5f, 2.0f);

		kt::Vec3 const initialV = { 2.0f, 4.0f, 45.0f };

		kt::Vec3 transformed = mat * initialV;
		transformed = kt::InverseOrtho(mat) * transformed;
		REQUIRE(RelError(transformed, initialV, 0.0001f));
	}
}

TEST_CASE("Mat3 full inverse", "")
{
	{
		kt::Mat3 mat = kt::Mat3::RotZYX(4.5f, 2.5f, 2.0f);

		kt::Vec3 const initialV = { 2.0f, 4.0f, 45.0f };

		kt::Vec3 transformed = mat * initialV;
		transformed = kt::Inverse(mat) * transformed;
		REQUIRE(RelError(transformed, initialV, 0.0001f));
	}
}

TEST_CASE("Mat3 to Quat", "")
{
	{
		kt::Mat3 const mat = kt::Mat3::RotZYX(1.0f, 1.2f, 0.2f);
		kt::Vec3 const toRot = kt::Vec3(125.0f, 12.0f, -13.0f);

		kt::Quat const quat = kt::ToQuat(mat);
		kt::Vec3 const fromMat = mat * toRot;
		kt::Vec3 const fromQuat = kt::ToQuat(mat) * toRot;

		REQUIRE(RelError(fromMat, fromQuat, 0.0001f));

		kt::Vec3 const invFromMat = kt::InverseOrtho(mat) * fromMat;
		kt::Vec3 const invFromQuat = kt::Conjugate(quat) * fromQuat;

		REQUIRE(RelError(invFromMat, invFromQuat, 0.0001f));
	}

	{
		kt::Mat3 const mat1 = kt::Mat3::RotZYX(12.0f, -1.2f, 0.2f);
		kt::Mat3 const mat2 = kt::Mat3::RotZYX(1.0f, 12.0f, -3.1f);

		kt::Mat3 const concatMat = mat2 * mat1;
		kt::Quat const concatQuat = kt::ToQuat(mat2) * kt::ToQuat(mat1);

		kt::Vec3 const toRot = kt::Vec3(125.0f, 12.0f, -13.0f);

		kt::Vec3 const fromMat = concatMat * toRot;
		kt::Vec3 const fromQuat = concatQuat * toRot;

		REQUIRE(RelError(fromMat, fromQuat, 0.0001f));
	}

}

TEST_CASE("Quat to mat", "")
{
	{
		kt::Quat q;
		q.FromNormalizedAxisAngle(kt::Vec3(0.0f, 1.0f, 0.0f), 2.45f);

		kt::Mat3 const mat3FromQ = kt::ToMat3(q);
		kt::Mat4 const mat4FromQ = kt::ToMat4(q);
	
		kt::Vec3 const toMul(24.0f, 12.0f, -27.0f);

		kt::Vec3 const mulByQ = q * toMul;
		kt::Vec3 const mulByMat3 = mat3FromQ * toMul;
		kt::Vec3 const mulByMat4 = mat4FromQ * toMul;
	
		REQUIRE(RelError(mulByMat3, mulByQ, 0.0001f));
		REQUIRE(RelError(mulByMat4, mulByQ, 0.0001f));
		REQUIRE(RelError(mulByMat4, mulByMat3, 0.0001f));

	}


}