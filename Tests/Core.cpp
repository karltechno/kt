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

	CHECK(kt::AlignUp(2, 4) == 4);
	CHECK(kt::AlignUp(16, 16) == 16);
	CHECK(kt::AlignUp(16, 4096) == 4096);
	CHECK(kt::AlignUp(1544, 4096) == 4096);

	CHECK((kt::AlignUp(3452345, 2048) % 2048) == 0);
	CHECK((kt::AlignUp(345345, 4096) % 4096) == 0);

	CHECK(kt::NextPow2(2) == 4);
	CHECK(kt::NextPow2(4) == 8);
	CHECK(kt::NextPow2(8) == 16);
	CHECK(kt::NextPow2(31) == 32);
	CHECK(kt::NextPow2(63) == 64);
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

	CHECK(kt::FloorLog2(1u) == 0);
	CHECK(kt::FloorLog2(2u) == 1);
	CHECK(kt::FloorLog2(4u) == 2);
	CHECK(kt::FloorLog2(7u) == 2);
	CHECK(kt::FloorLog2(8u) == 3);
	CHECK(kt::FloorLog2(255u) == 7);
	CHECK(kt::FloorLog2(256u) == 8);
}

struct Pod_NoCopy
{
	Pod_NoCopy(Pod_NoCopy const&)
	{
		pod = 2;
	}

	uint32_t pod;
};

struct Pod_ForceCopy
{
	Pod_ForceCopy(Pod_ForceCopy const&)
	{
		pod = 2;
	}

	uint32_t pod;
};

KT_BITWISE_COPYABLE(Pod_ForceCopy);

TEST_CASE("Bitwise Copyable", "")
{
	CHECK(!KT_HAS_TRIVIAL_COPY(Pod_NoCopy));
	CHECK(KT_HAS_TRIVIAL_COPY(Pod_ForceCopy));
}

enum class MyEnumFlags : uint32_t
{
	None = 0x0,
	A = 0x1,
	B = 0x2,
	C = 0x4,
	D = 0x8,
};

KT_ENUM_CLASS_FLAG_OPERATORS(MyEnumFlags);

TEST_CASE("Enum flags", "")
{
	MyEnumFlags flags = MyEnumFlags::None;
	CHECK(!flags);

	flags |= MyEnumFlags::A;

	CHECK(!!(flags & MyEnumFlags::A));
	CHECK(!(flags & MyEnumFlags::B));
}
