#include "catch.hpp"

#include <kt/kt.h>
#include <kt/Strings.h>

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
