#include "catch.hpp"

#include <kt/kt.h>
#include <kt/Sort.h>
#include <kt/Random.h>

TEST_CASE("Radix Sort Float", "")
{
	float arr[4096];

	float temp[KT_ARRAY_COUNT(arr)];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = (kt::RandomUnitFloat(rand) - 0.5f) * 50000.0f;
	}

	kt::RadixSort(arr, arr + KT_ARRAY_COUNT(arr), temp);
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr)));
}

TEST_CASE("Radix Sort u32", "")
{
	uint32_t arr[4096];

	uint32_t temp[KT_ARRAY_COUNT(arr)];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = rand.Gen32();
	}

	kt::RadixSort(arr, arr + KT_ARRAY_COUNT(arr), temp);
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr)));
}

TEST_CASE("Radix Sort i32", "")
{
	int32_t arr[4096];

	int32_t temp[KT_ARRAY_COUNT(arr)];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = (int32_t)rand.Gen32();
	}

	kt::RadixSort(arr, arr + KT_ARRAY_COUNT(arr), temp);
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr)));
}

TEST_CASE("Radix Sort i64", "")
{
	int64_t arr[4096];

	int64_t temp[KT_ARRAY_COUNT(arr)];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = (int64_t)rand.Gen32() | (int64_t)rand.Gen32() << 32u;
	}

	kt::RadixSort(arr, arr + KT_ARRAY_COUNT(arr), temp);
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr)));
}

TEST_CASE("Radix Sort GetKey", "")
{
	int64_t arr[4096];

	int64_t temp[KT_ARRAY_COUNT(arr)];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = (int64_t)rand.Gen32() | (int64_t)rand.Gen32() << 32u;
	}

	kt::RadixSort(arr, arr + KT_ARRAY_COUNT(arr), temp, [](int64_t const i) {return i; });
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr)));
}

TEST_CASE("Radix Sort values", "")
{
	uint32_t arr[4096];
	uint64_t valArray[KT_ARRAY_COUNT(arr)];

	uint32_t keyTemp[KT_ARRAY_COUNT(arr)];
	uint64_t valTemp[KT_ARRAY_COUNT(arr)];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = rand.Gen32();
		valArray[i] = (uint64_t)arr[i] + 1234;
	}


	kt::RadixSort(arr, arr + KT_ARRAY_COUNT(arr), keyTemp, valArray, valTemp);
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr)));
	REQUIRE(kt::IsSorted(valArray, valArray + KT_ARRAY_COUNT(arr)));

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		REQUIRE(arr[i] == valArray[i] - 1234);
	}
}

TEST_CASE("Quick Sort u32", "")
{
	uint32_t arr[4096];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = rand.Gen32();
	}

	kt::QuickSort(arr, arr + KT_ARRAY_COUNT(arr));
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr)));
}

TEST_CASE("Quick Sort u32 greater than", "")
{
	uint32_t arr[4096];

	kt::XorShift32 rand;

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(arr); ++i)
	{
		arr[i] = rand.Gen32();
	}

	kt::QuickSort(arr, arr + KT_ARRAY_COUNT(arr), [](uint32_t const& _a, uint32_t const& _b) {return _a > _b; });
	REQUIRE(kt::IsSorted(arr, arr + KT_ARRAY_COUNT(arr), [](uint32_t const& _a, uint32_t const& _b) {return _a >= _b; }));
}