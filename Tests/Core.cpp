#include "catch.hpp"

#include <kt/kt.h>
#include <kt/Memory.h>
#include <kt/Hash.h>
#include <kt/Random.h>

TEST_CASE("Core Macros", "")
{
	CHECK(kt::IsPow2(2) == true);
	CHECK(kt::IsPow2(4) == true);
	CHECK(kt::IsPow2(16) == true);
	CHECK(kt::IsPow2(32) == true);

	CHECK(kt::IsPow2(36) == false);
	CHECK(kt::IsPow2(4567) == false);
	CHECK(kt::IsPow2(23452) == false);
	CHECK(kt::IsPow2(84678) == false);

	CHECK(kt::AlignValue(2, 4) == 4);
	CHECK(kt::AlignValue(16, 16) == 16);
	CHECK(kt::AlignValue(16, 4096) == 4096);
	CHECK(kt::AlignValue(1544, 4096) == 4096);

	CHECK((kt::AlignValue(3452345, 2048) % 2048) == 0);
	CHECK((kt::AlignValue(345345, 4096) % 4096) == 0);

	CHECK(kt::NextPow2(2) == 4);
	CHECK(kt::NextPow2(4) == 8);
	CHECK(kt::NextPow2(8) == 16);
	CHECK(kt::NextPow2(31) == 32);
	CHECK(kt::NextPow2(1547) == 2048);

	CHECK(!kt::IsAligned(5, 8));
	CHECK(!kt::IsAligned(6, 8));
	CHECK(!kt::IsAligned(7, 8));
	CHECK(kt::IsAligned(8, 8));
	CHECK(kt::IsAligned(16, 8));
	CHECK(kt::IsAligned(32, 8));

	CHECK(!kt::IsAligned(5, 4096));
	CHECK(!kt::IsAligned(6, 4096));
	CHECK(!kt::IsAligned(7, 4096));
	CHECK(!kt::IsAligned(8, 4096));
	CHECK(kt::IsAligned(4096, 4096));
	CHECK(kt::IsAligned(4096 * 2, 4096));
}
