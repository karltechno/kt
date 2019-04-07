#include "catch.hpp"

#include <kt/Slice.h>
#include <kt/Macros.h>


TEST_CASE("Basic Slice", "[slice]")
{
	kt::Slice<int> sliceInt;

	int a = 5;
	sliceInt = kt::Slice<int>(a);
	CHECK(sliceInt.Data() == &a);
	CHECK(sliceInt.Size() == 1);
	CHECK(sliceInt.Empty());

	int arrayOfInts[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	sliceInt = kt::Slice<int>(arrayOfInts);

	CHECK(sliceInt.Size() == KT_ARRAY_COUNT(arrayOfInts));
}

TEST_CASE("Basic Slice const", "[slice]")
{
	kt::Slice<const int> sliceInt;

	int a = 5;
	sliceInt = kt::Slice<const int>(a);
	CHECK(sliceInt.Data() == &a);
	CHECK(sliceInt.Size() == 1);
	CHECK(sliceInt.Empty());

	int const arrayOfInts[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	sliceInt = kt::Slice<const int>(arrayOfInts);

	CHECK(sliceInt.Size() == KT_ARRAY_COUNT(arrayOfInts));

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arrayOfInts); ++i)
	{
		CHECK(sliceInt[i] == arrayOfInts[i]);
	}
}