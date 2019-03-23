#include "catch.hpp"

#include <kt/simd_v4f32.h>
#include <kt/simd_v4b32.h>

kt::simd::v4b32 AlmostEqual_Absolute(kt::simd::v4f32 const _lhs, kt::simd::v4f32 const _rhs, float const _absErr)
{
	return kt::simd::Abs(_lhs - _rhs) <= kt::simd::Splat_v4f32(_absErr);
}

float constexpr c_smallAbsError = 0.0001f;

TEST_CASE("v4f32 basic arith", "simd")
{
	{
		kt::simd::v4f32 const f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 const f2(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4b32 const mask = AlmostEqual_Absolute(f1 + f2, kt::simd::v4f32(2.0f, 4.0f, 6.0f, 8.0f), c_smallAbsError);
		REQUIRE(kt::simd::AllTrue(mask));
	}

	{
		kt::simd::v4f32 const f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 const f2(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4b32 const mask = AlmostEqual_Absolute(f1 - f2, kt::simd::v4f32(0.0f), c_smallAbsError);
		REQUIRE(kt::simd::AllTrue(mask));
	}

	{
		kt::simd::v4f32 const f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 const f2(2.0f);
		kt::simd::v4b32 const mask = AlmostEqual_Absolute(f1 * f2, kt::simd::v4f32(2.0f, 4.0f, 6.0f, 8.0f), c_smallAbsError);
		REQUIRE(kt::simd::AllTrue(mask));
	}

	{
		kt::simd::v4f32 const f1(2.0f, 4.0f, 6.0f, 8.0f);
		kt::simd::v4f32 const f2 = kt::simd::Splat_v4f32(2.0f);
		kt::simd::v4f32 const div = f1 / f2;
		kt::simd::v4b32 const mask = AlmostEqual_Absolute(div, kt::simd::v4f32(1.0f, 2.0f, 3.0f, 4.0f), c_smallAbsError);
		REQUIRE(kt::simd::AllTrue(mask));
	}
}

TEST_CASE("v4f32 comparisons", "simd")
{
	{
		kt::simd::v4f32 f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 f2(1.5f, 1.9f, 3.5f, 3.9f);
		kt::simd::v4b32 const cmp = f1 >= f2;
		REQUIRE(kt::simd::MoveMask(cmp) == kt::simd::MoveMask(kt::simd::Mask_v4b32<0, 1, 0, 1>()));
	}

	{
		kt::simd::v4f32 f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 f2(1.5f, 1.9f, 3.5f, 3.9f);
		kt::simd::v4b32 const cmp = f1 >= f2;
		REQUIRE(kt::simd::MoveMask(cmp) == kt::simd::MoveMask(kt::simd::Mask_v4b32<0, 1, 0, 1>()));
	}

	{
		kt::simd::v4f32 f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 f2(1.5f, 1.9f, 3.5f, 3.9f);
		kt::simd::v4f32 const max = kt::simd::Max(f1, f2);
		REQUIRE(kt::simd::AllTrue(max == kt::simd::v4f32(1.5f, 2.0f, 3.5f, 4.0f)));
	}

	{
		kt::simd::v4f32 f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 f2(1.5f, 1.9f, 3.5f, 3.9f);
		kt::simd::v4f32 const min = kt::simd::Min(f1, f2);
		REQUIRE(kt::simd::AllTrue(min == kt::simd::v4f32(1.0f, 1.9f, 3.0f, 3.9f)));
	}

	{
		kt::simd::v4f32 f1(1.0f, 2.0f, 3.0f, 4.0f);
		kt::simd::v4f32 f2(1.5f, 1.9f, 3.5f, 3.9f);
		kt::simd::v4f32 const blend = kt::simd::Blend(kt::simd::Mask_v4b32<1, 0, 1, 0>(), f1, f2);
		REQUIRE(kt::simd::AllTrue(blend == kt::simd::v4f32(1.0f, 1.9f, 3.0f, 3.9f)));
	}
}


TEST_CASE("v4f32 shuffle", "simd")
{
	kt:: simd::v4f32 const vec(1.0f, 2.0f, 3.0f, 4.0f);

	{
		kt::simd::v4f32 const shuf = kt::simd::Shuf<0, 0, 0, 0>(vec);
		REQUIRE(kt::simd::AllTrue(shuf == kt::simd::v4f32(1.0f)));
	}

	{
		kt::simd::v4f32 const shuf = kt::simd::Shuf<1, 1, 1, 1>(vec);
		REQUIRE(kt::simd::AllTrue(shuf == kt::simd::v4f32(2.0f)));
	}

	{
		kt:: simd::v4f32 const shuf = kt::simd::Shuf<2, 2, 2, 2>(vec);
		REQUIRE(kt::simd::AllTrue(shuf == kt::simd::v4f32(3.0f)));
	}

	{
		kt::simd::v4f32 const shuf = kt::simd::Shuf<3, 3, 3, 3>(vec);
		REQUIRE(kt::simd::AllTrue(shuf == kt::simd::Splat_v4f32(4.0f)));
	}

	{
		kt::simd::v4f32 const shuf = kt::simd::Shuf<3, 2, 1, 0>(vec);
		REQUIRE(kt::simd::AllTrue(shuf == kt::simd::v4f32(4.0f, 3.0f, 2.0f, 1.0f)));
	}

	{
		kt::simd::v4f32 const shuf = kt::simd::Shuf<3, 3, 1, 0>(vec);
		REQUIRE(kt::simd::AllTrue(shuf == kt::simd::v4f32(4.0f, 4.0f, 2.0f, 1.0f)));
	}

	{
		kt::simd::v4f32 const shuf = kt::simd::Shuf<2, 3, 1, 0>(vec);
		REQUIRE(kt::simd::AllTrue(shuf == kt::simd::v4f32(3.0f, 4.0f, 2.0f, 1.0f)));
	}
}

TEST_CASE("v4f32 Extract", "simd")
{
	kt::simd::v4f32 const v4(1.0f, 2.0f, 3.0f, 4.0f);
	CHECK(v4.X() == 1.0f);
	CHECK(v4.Y() == 2.0f);
	CHECK(v4.Z() == 3.0f);
	CHECK(v4.W() == 4.0f);
}

TEST_CASE("v4b32 BitOps", "simd")
{
	kt::simd::v4f32 const v4(1.0f, -2.0f, 3.0f, -4.0f);

	CHECK(kt::simd::AllTrue(kt::simd::Abs(v4) == kt::simd::v4f32(1.0f, 2.0f, 3.0f, 4.0f)));

	CHECK(kt::simd::AllTrue((~kt::simd::Mask_v4b32<1, 0, 1, 0>()) == kt::simd::Mask_v4b32<0, 1, 0, 1>()));

	{
		kt::simd::v4b32 xorExpr = (kt::simd::Mask_v4b32<1, 0, 1, 0>() ^ kt::simd::Mask_v4b32<1, 1, 0, 1>());

		CHECK(kt::simd::AllTrue(xorExpr == kt::simd::Mask_v4b32<0, 1, 1, 1>()));
		xorExpr ^= kt::simd::Mask_v4b32<1, 1, 0, 1>();
		CHECK(kt::simd::AllTrue(xorExpr == kt::simd::Mask_v4b32<1, 0, 1, 0>()));
	}

	{
		kt::simd::v4b32 andExpr = kt::simd::Mask_v4b32<1, 0, 1, 1>() & kt::simd::Mask_v4b32<0, 1, 1, 1>();

		CHECK(kt::simd::AllTrue(andExpr == kt::simd::Mask_v4b32<0, 0, 1, 1>()));
		andExpr &= kt::simd::Mask_v4b32<1, 1, 0, 1>();
		CHECK(kt::simd::AllTrue(andExpr == kt::simd::Mask_v4b32<0, 0, 0, 1>()));
	}

	{
		kt::simd::v4b32 orExpr = kt::simd::Mask_v4b32<0, 0, 1, 0>() | kt::simd::Mask_v4b32<0, 0, 0, 1>();

		CHECK(kt::simd::AllTrue(orExpr == kt::simd::Mask_v4b32<0, 0, 1, 1>()));
		orExpr |= kt::simd::Mask_v4b32<1, 1, 0, 1>();
		CHECK(kt::simd::AllTrue(orExpr == kt::simd::Mask_v4b32<1, 1, 1, 1>()));
	}
}

TEST_CASE("v4f32 load/store", "simd")
{
	KT_ALIGNAS(kt::simd::v4f32::c_align) float const base[4] = { 1.4f, 1.2f, 5.3f, 1.12f };


	kt::simd::v4f32 const loadA = kt::simd::LoadA_v4f32(base);
	kt::simd::v4f32 const loadU = kt::simd::LoadU_v4f32(base);
	CHECK(kt::simd::AllTrue(loadA == loadU));

	float storeU[4];
	KT_ALIGNAS(kt::simd::v4f32::c_align) float storeA[4];
	kt::simd::StoreA_v4f32(loadA, storeA);
	kt::simd::StoreU_v4f32(loadU, storeU);

	CHECK(memcmp(storeA, storeU, sizeof(storeU)) == 0);
}