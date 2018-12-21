#include "catch.hpp"
#include <memory>

#include <kt/kt.h>
#include <kt/Sort.h>
#include <kt/Array.h>
#include <kt/HashMap.h>
#include <kt/Strings.h>
#include <kt/Random.h>

TEST_CASE("Array Basic Tests", "")
{
	kt::Array<uint32_t> arr;

	static constexpr uint32_t u32PushBackCount = 512;

	for (uint32_t i = 0; i < u32PushBackCount; ++i)
	{
		arr.PushBack(i * 2 + 1);
	}

	REQUIRE(arr.Size() == u32PushBackCount);

	for (uint32_t i = 0; i < u32PushBackCount; ++i)
	{
		CHECK(arr[i] == i * 2 + 1);
	}
}


TEST_CASE("Array Leak test", "")
{
	{
		std::shared_ptr<int> sharedPtr = std::make_shared<int>();

		std::weak_ptr<int> weakPtr = sharedPtr;
		{
			kt::Array<std::shared_ptr<int>> arr;

			for (uint32_t i = 0; i < 512; ++i)
			{
				arr.PushBack(sharedPtr);
			}

			CHECK(weakPtr.lock());
			sharedPtr.reset();
		}

		CHECK(weakPtr.lock() == nullptr);
	}

	std::weak_ptr<int> weakPtr2;

	{
		std::shared_ptr<int> sharedPtr = std::make_shared<int>();

		weakPtr2 = sharedPtr;

		kt::Array<std::shared_ptr<int>> arr;

		for (uint32_t i = 0; i < 512; ++i)
		{
			arr.PushBack(sharedPtr);
		}

		kt::Array<std::shared_ptr<int>> arr2 = arr.MakeCopy();

		CHECK((uint32_t)sharedPtr.use_count() == arr2.Size() + arr.Size() + 1);
	}

	CHECK(weakPtr2.lock() == nullptr);

	{
		std::shared_ptr<int> sharedPtr = std::make_shared<int>();
		weakPtr2 = sharedPtr;

		kt::Array<std::shared_ptr<int>> arr;

		for (uint32_t i = 0; i < 512; ++i)
		{
			arr.PushBack(sharedPtr);
		}

		kt::Array<std::shared_ptr<int>> arr2(std::move(arr));

		CHECK((uint32_t)sharedPtr.use_count() == 1 + arr2.Size());
	}
	CHECK(weakPtr2.lock() == nullptr);

}

TEST_CASE("Array ordered erase test", "")
{
	kt::Array<uint32_t> arr;

	for (uint32_t i = 0; i < 512; ++i)
	{
		arr.PushBack(i);
	}

	uint32_t i = 0;

	for (kt::Array<uint32_t>::Iterator it = arr.Begin(); it != arr.End(); /* */)
	{
		if (i++ % 3)
		{
			it = arr.Erase(it);
			CHECK(kt::IsSorted(arr.Begin(), arr.End()));
		}
		else
		{
			++it;
		}
	}
}

TEST_CASE("Array Erase Leak Test", "")
{
	std::shared_ptr<int> sharedPtr = std::make_shared<int>();
	std::weak_ptr<int> weak = sharedPtr;

	kt::Array<std::shared_ptr<int>> arr;

	for (uint32_t i = 0; i < 512; ++i)
	{
		arr.PushBack(sharedPtr);
	}

	CHECK((uint32_t)sharedPtr.use_count() == arr.Size() + 1);

	for (kt::Array<std::shared_ptr<int>>::Iterator it = arr.Begin(); it != arr.End(); /* */)
	{
		uint32_t prevCount = (uint32_t)sharedPtr.use_count();
		it = arr.Erase(it);
		CHECK((uint32_t)sharedPtr.use_count() == prevCount - 1);
	}

	sharedPtr.reset();

	CHECK(weak.lock() == nullptr);
}

TEST_CASE("Array Erase unordered Leak Test", "")
{
	std::shared_ptr<int> sharedPtr = std::make_shared<int>();
	std::weak_ptr<int> weak = sharedPtr;

	kt::Array<std::shared_ptr<int>> arr;

	for (uint32_t i = 0; i < 512; ++i)
	{
		arr.PushBack(sharedPtr);
	}


	CHECK((uint32_t)sharedPtr.use_count() == arr.Size() + 1);

	for (kt::Array<std::shared_ptr<int>>::Iterator it = arr.Begin(); it != arr.End(); /* */)
	{
		uint32_t prevCount = (uint32_t)sharedPtr.use_count();
		it = arr.EraseSwap(it);
		CHECK((uint32_t)sharedPtr.use_count() == prevCount - 1);
	}

	sharedPtr.reset();

	CHECK(weak.lock() == nullptr);
}

TEST_CASE("Array copy", "")
{
	uint32_t const NUM = 128;
	kt::Array<uint32_t> arr;
	for (uint32_t i = 0; i < NUM; ++i)
	{
		arr.PushBack(i);
	}

	kt::Array<uint32_t> copy = arr.MakeCopy();

	REQUIRE(copy.Size() == arr.Size());
	for (uint32_t i = 0; i < NUM; ++i)
	{
		CHECK(arr[i] == i);
	}
}


TEST_CASE("Array move", "")
{
	uint32_t const NUM = 128;
	kt::Array<uint32_t> arr;
	for (uint32_t i = 0; i < NUM; ++i)
	{
		arr.PushBack(i);
	}

	kt::Array<uint32_t> moved(std::move(arr));

	REQUIRE(moved.Size() == NUM);
	CHECK(arr.Size() == 0);
	for (uint32_t i = 0; i < NUM; ++i)
	{
		CHECK(moved[i] == i);
	}
}

TEST_CASE("Array operator=&&", "")
{
	uint32_t const NUM = 128;
	kt::InplaceArray<uint32_t, 128> arr;
	for (uint32_t i = 0; i < NUM; ++i)
	{
		arr.PushBack(i);
	}

	kt::Array<uint32_t> moved;

	moved.PushBack(2);
	moved = std::move(arr);

	REQUIRE(moved.Size() == NUM);
	CHECK(arr.Size() == 0);
	for (uint32_t i = 0; i < NUM; ++i)
	{
		CHECK(moved[i] == i);
	}
}