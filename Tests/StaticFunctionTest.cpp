#include "catch.hpp"

#include <kt/StaticFunction.h>
#include <kt/Array.h>

TEST_CASE("Simple Static Function Test", "[static_function]")
{
	kt::StaticFunction<bool(int), 32> ftor;

	auto fn = [](int a) -> bool
	{ 
		return a != 0; 
	};

	ftor.Set(fn);

	CHECK(ftor(1));
	CHECK(!ftor(0));
}

TEST_CASE("Static function move", "[static_function]")
{
	std::shared_ptr<int> myThing = std::make_shared<int>(1);
	std::weak_ptr<int> weak = myThing;

	kt::Array<kt::StaticFunction<void(void), 32>> fns;
	uint32_t const NUM = 32;

	{
		auto sharedPtrFn = [myThing]()
		{
		};


		for (uint32_t i = 0; i < NUM; ++i)
		{
			fns.PushBack(sharedPtrFn);
		}
	}

	CHECK(myThing.use_count() == NUM + 1);

	for (uint32_t i = 0; i < NUM; ++i)
	{
		CHECK((uint32_t)myThing.use_count() == ((NUM + 1) - i));
		fns.PopBack();
	}

	CHECK(weak.lock());
	CHECK(myThing.use_count() == 1);
	myThing = nullptr;
	CHECK(weak.lock() == nullptr);
}

TEST_CASE("Capturing this", "[static_function]")
{
	struct MyClass
	{
		void Test(int _y)
		{
			CHECK(_y == x);
		}
		int x;
	};

	uint32_t const NUM = 128;

	MyClass classes[NUM];
	for (uint32_t i = 0; i < KT_ARRAY_COUNT(classes); ++i)
	{
		classes[i].x = i;
	}

	kt::Array<kt::StaticFunction<void(int), 32>> arr;
	
	for (uint32_t i = 0; i < NUM; ++i)
	{
		MyClass* captureThis = &classes[i];
		arr.PushBack([captureThis](int x)
		{
			captureThis->Test(x);
		});
	}

	for (uint32_t i = 0; i < NUM; ++i)
	{
		arr[i]((int)i);
	}
}

TEST_CASE("Operator bool", "[static_function]")
{
	kt::StaticFunction<void(int), 32> fn;
	CHECK(!fn);

	fn = [](int)
	{
	};

	CHECK(fn);

	kt::StaticFunction<void(int), 32> other;
	CHECK(!other);
	other = std::move(fn);
	CHECK(other);
	CHECK(!fn);

	other.Clear();
	CHECK(!other);
}