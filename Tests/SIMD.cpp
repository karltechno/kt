#include "catch.hpp"

#include <kt/SIMD.h>

TEST_CASE("SIMD Basic ops", "[SIMD]")
{
	{
		kt::SIMD_Float4 const f1 = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);
		kt:: SIMD_Float4 const f2 = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);
		kt:: SIMD_Float4 const mask = kt::SIMD_CmpEq(f1 + f2, kt::SIMD_Load4(2.0f, 4.0f, 6.0f, 8.0f));
		REQUIRE(kt::SIMD_AllTrue(mask));
	}

	{
		kt:: SIMD_Float4 const f1 = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);
		kt:: SIMD_Float4 const f2 = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);
		kt:: SIMD_Float4 const mask = kt::SIMD_CmpEq(f1 - f2, kt::SIMD_Splat(0.0f));
		REQUIRE(kt::SIMD_AllTrue(mask));
	}

	{
		kt:: SIMD_Float4 const f1 = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);
		kt:: SIMD_Float4 const f2 = kt::SIMD_Splat(2.0f);
		kt:: SIMD_Float4 const mask = kt::SIMD_CmpEq(f1 * f2, kt::SIMD_Load4(2.0f, 4.0f, 6.0f, 8.0f));
		REQUIRE(kt::SIMD_AllTrue(mask));
	}

	{
		kt:: SIMD_Float4 const f1 = kt::SIMD_Load4(2.0f, 4.0f, 6.0f, 8.0f);
		kt:: SIMD_Float4 const f2 = kt::SIMD_Splat(2.0f);
		kt:: SIMD_Float4 const mask = kt::SIMD_CmpEq(f1 / f2, kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f));
		REQUIRE(kt::SIMD_AllTrue(mask));
	}
}

TEST_CASE("SIMD Basic comparisons", "[SIMD]")
{
	{
		kt:: SIMD_Float4 f1 = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);
		kt:: SIMD_Float4 f2 = kt::SIMD_Load4(1.5f, 1.9f, 3.5f, 3.9f);
		kt:: SIMD_Float4 const cmp = kt::SIMD_CmpGe(f1, f2);
		REQUIRE(kt::SIMD_MoveMask(cmp) == kt::SIMD_MoveMask(kt::SIMD_Mask<0, 1, 0, 1>()));
	}

	{
		kt:: SIMD_Float4 f1 = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);
		kt:: SIMD_Float4 f2 = kt::SIMD_Load4(1.5f, 1.9f, 3.5f, 3.9f);
		kt:: SIMD_Float4 const cmp = kt::SIMD_CmpGe(f1, f2);
		REQUIRE(kt::SIMD_MoveMask(cmp) == kt::SIMD_MoveMask(kt::SIMD_Mask<0, 1, 0, 1>()));
	}
}


TEST_CASE("SIMD shuffle", "[SIMD]")
{
	kt:: SIMD_Float4 const vec = kt::SIMD_Load4(1.0f, 2.0f, 3.0f, 4.0f);

	{
		kt:: SIMD_Float4 const shuf = kt::SIMD_Shuf<0, 0, 0, 0>(vec);
		REQUIRE(kt::SIMD_AllTrue(kt::SIMD_CmpEq(shuf, kt::SIMD_Splat(1.0f))));
	}

	{
		kt:: SIMD_Float4 const shuf = kt::SIMD_Shuf<1, 1, 1, 1>(vec);
		REQUIRE(kt::SIMD_AllTrue(kt::SIMD_CmpEq(shuf, kt::SIMD_Splat(2.0f))));
	}

	{
		kt:: SIMD_Float4 const shuf = kt::SIMD_Shuf<2, 2, 2, 2>(vec);
		REQUIRE(kt::SIMD_AllTrue(kt::SIMD_CmpEq(shuf, kt::SIMD_Splat(3.0f))));
	}

	{
		kt:: SIMD_Float4 const shuf = kt::SIMD_Shuf<3, 3, 3, 3>(vec);
		REQUIRE(kt::SIMD_AllTrue(kt::SIMD_CmpEq(shuf, kt::SIMD_Splat(4.0f))));
	}

	{
		kt:: SIMD_Float4 const shuf = kt::SIMD_Shuf<3, 2, 1, 0>(vec);
		REQUIRE(kt::SIMD_AllTrue(kt::SIMD_CmpEq(shuf, kt::SIMD_Load4(4.0f, 3.0f, 2.0f, 1.0f))));
	}

	{
		kt:: SIMD_Float4 const shuf = kt::SIMD_Shuf<3, 3, 1, 0>(vec);
		REQUIRE(kt::SIMD_AllTrue(kt::SIMD_CmpEq(shuf, kt::SIMD_Load4(4.0f, 4.0f, 2.0f, 1.0f))));
	}

	{
		kt:: SIMD_Float4 const shuf = kt::SIMD_Shuf<2, 3, 1, 0>(vec);
		REQUIRE(kt::SIMD_AllTrue(kt::SIMD_CmpEq(shuf, kt::SIMD_Load4(3.0f, 4.0f, 2.0f, 1.0f))));
	}
}