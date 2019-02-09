#include "catch.hpp"

#include <kt/kt.h>
#include <kt/Strings.h>
#include <kt/Hash.h>

TEST_CASE("Static String 1", "")
{
	kt::String256 str;
	str.Append("Hello");

	CHECK(str == "Hello");

	str.Append(123u);

	CHECK(str == "Hello123");
}

TEST_CASE("String view 1", "")
{
	kt::StringView v("Hello");

	kt::StringView v2("Hello");

	CHECK(v == v2);
}

TEST_CASE("Static string format", "")
{
	kt::String256 str;
	str.AppendFmt("Hello %u %u %d", 1, 2, 3);
	CHECK(str == "Hello 1 2 3");
}


TEST_CASE("Insensitive string compare", "")
{
	kt::StringView const a = "HeLlo !232323 HoWs It GoIng!?[";
	kt::StringView const b = "hElLo !232323 Hows it GoIng!?[";

	kt::StringView const c = "hElLo !232323 Hows it GoIng!?[0";

	CHECK(StrCmpI(a, b) == 0);

	CHECK(StrCmpI(a, c) != 0);
	CHECK(StrCmpI(b, c) != 0);
}


TEST_CASE("Insensitive string hash", "")
{
	kt::StringView const a = "HeLlo !232323 HoWs It GoIng!?[";
	kt::StringView const b = "hElLo !232323 Hows it GoIng!?[";

	kt::StringView const c = "hElLo !232323 Hows it GoIng!?[0";


	CHECK(StringHashI(a) == StringHashI(b));

	CHECK(StringHashI(b) != StringHashI(c));
	CHECK(StringHashI(a) != StringHashI(c));
}

