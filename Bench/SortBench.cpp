#include "Bench.h"
#include <kt/Sort.h>
#include <kt/Random.h>

#include <algorithm>

KT_BENCH(SortSTL_4096_u32, 512)
{
	uint32_t nums[4096];
	kt::XorShift32 rnd;
	for (uint32_t i = 0; i < KT_ARRAY_COUNT(nums); ++i)
	{
		nums[i] = rnd.Gen32();
	}

	kt::ScopedBenchStart bench;
	std::sort(nums, nums + KT_ARRAY_COUNT(nums));
}

KT_BENCH(ktQSort_4096_u32, 512)
{
	uint32_t nums[4096];
	kt::XorShift32 rnd;
	for (uint32_t i = 0; i < KT_ARRAY_COUNT(nums); ++i)
	{
		nums[i] = rnd.Gen32();
	}

	kt::ScopedBenchStart bench;
	kt::QuickSort(nums, nums + KT_ARRAY_COUNT(nums));
}

KT_BENCH(ktRadixSort_4096_u32, 512)
{
	uint32_t nums[4096];
	uint32_t temp[4096];
	kt::XorShift32 rnd;
	for (uint32_t i = 0; i < KT_ARRAY_COUNT(nums); ++i)
	{
		nums[i] = rnd.Gen32();
	}

	kt::ScopedBenchStart bench;
	kt::RadixSort(nums, nums + KT_ARRAY_COUNT(nums), temp);
}