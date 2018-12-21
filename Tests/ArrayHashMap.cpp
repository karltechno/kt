#include <catch.hpp>
#include <kt/ArrayHashMap.h>
#include <string.h>
#include <kt/Random.h>

TEST_CASE("ArrayHashMap Basic find", "[ArrayHashMap]")
{
	kt::ArrayHashMap<int, int> map;
	static const int NUM = 1024;
	map.Reserve(NUM);
	for (int i = 0; i < NUM; ++i)
	{
		map.Insert(i, i * 2 + 1);
	}

	REQUIRE(map.Size() == NUM);

	for (int i = 0; i < NUM; ++i)
	{
		kt::ArrayHashMap<int, int>::KeyIterator it = map.FindKeyIt(i);
		REQUIRE(it != map.EndKeys());
		REQUIRE(*map.KeyToValueIt(it) == i * 2 + 1);
	}
}

TEST_CASE("ArrayHashMap Copy", "[ArrayHashMap]")
{
	kt::XXHash_Incremental64 l;
	l.Finalize();
	kt::ArrayHashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::ArrayHashMap<uint32_t, uint32_t> mapCopy = map.MakeCopy();

	REQUIRE(mapCopy.Size() == map.Size());

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(mapCopy.Find(i) != mapCopy.EndValues());
	}
}


TEST_CASE("ArrayHashMap operator=", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::ArrayHashMap<uint32_t, uint32_t> mapCopy;
	mapCopy[2] = 4;
	mapCopy = map.MakeCopy();

	REQUIRE(mapCopy.Size() == map.Size());

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(mapCopy.Find(i) != mapCopy.EndValues());
	}
}

TEST_CASE("ArrayHashMap move", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::ArrayHashMap<uint32_t, uint32_t> mapCopy(std::move(map));

	REQUIRE(mapCopy.Size() == num);
	REQUIRE(map.Size() == 0);

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(mapCopy.Find(i) != mapCopy.EndValues());
	}
}

TEST_CASE("ArrayHashMap operator=&&", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::ArrayHashMap<uint32_t, uint32_t> mapCopy;
	mapCopy[2] = 4;
	mapCopy = std::move(map);

	REQUIRE(mapCopy.Size() == num);
	REQUIRE(map.Size() == 0);

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(mapCopy.Find(i) != mapCopy.EndValues());
	}
}

TEST_CASE("ArrayHashMap leak test 1", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, std::shared_ptr<int>> map;

	std::shared_ptr<int> sPtr = std::make_shared<int>();

	std::weak_ptr<int> weak = sPtr;

	for (uint32_t i = 0; i < 512; ++i)
	{
		map.Insert(i, sPtr);
	}

	REQUIRE((uint32_t)sPtr.use_count() == map.Size() + 1);

	uint32_t useCount = (uint32_t)sPtr.use_count();

	for (kt::ArrayHashMap<uint32_t, std::shared_ptr<int>>::KeyIterator it = map.BeginKeys(); it != map.EndKeys(); )
	{
		it = map.Erase(it);
		REQUIRE((uint32_t)sPtr.use_count() == --useCount);
	}

	sPtr.reset();
	REQUIRE(weak.lock() == nullptr);

}

TEST_CASE("ArrayHashMap leak test 2", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, std::shared_ptr<int>> map;

	std::shared_ptr<int> sPtr = std::make_shared<int>();

	std::weak_ptr<int> weak = sPtr;

	for (uint32_t i = 0; i < 512; ++i)
	{
		map.Insert(i, sPtr);
	}

	REQUIRE((uint32_t)sPtr.use_count() == map.Size() + 1u);

	{
		kt::ArrayHashMap<uint32_t, std::shared_ptr<int>> map2(std::move(map));

		REQUIRE((uint32_t)sPtr.use_count() == map2.Size() + 1);
	}

	sPtr.reset();
	REQUIRE(weak.lock() == nullptr);
}


TEST_CASE("ArrayHashMap Deletetion Test 1", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	uint32_t arr[num];

	for (uint32_t i = 0; i < num; ++i)
	{
		map[i] = i * 2;
		arr[i] = i;
	}

	for (uint32_t i = 0; i < num; ++i)
	{
		auto it = map.FindKeyIt(i);
		REQUIRE(it != map.EndKeys());
		REQUIRE(*map.KeyToValueIt(it) == i * 2);
	}

	for (uint32_t i = 0; i < num; ++i)
	{
		uint32_t const idx = arr[i];

		auto it = map.FindKeyIt(idx);
		REQUIRE(it != map.EndKeys());
		REQUIRE(*map.KeyToValueIt(it) == idx * 2);

		map.Erase(it);
	}

	REQUIRE(map.Size() == 0);
}



TEST_CASE("ArrayHashMap string lookup 1", "[ArrayHashMap]")
{
	kt::ArrayHashMap<char const*, uint8_t> map;

	map.Reserve(2048);

	char* strings = (char*)kt::Malloc(1024 * 129);
	KT_SCOPE_EXIT(kt::Free(strings));

	kt::XorShift32 rng(2);

	static const uint32_t range = 'Z' - 'A';

	for (uint32_t i = 0; i < 1024; ++i)
	{
		char* str = strings + 129 * i;
		for (uint32_t j = 0; j < 128; ++j)
		{
			str[j] = (rng.Gen32() % range) + 'A';
		}
		str[128] = 0;
	}

	for (uint32_t i = 0; i < 1024; ++i)
	{
		char const* str = strings + 129 * i;
		map.Insert(str, str[str[0] % 128]);
	}

	for (uint32_t i = 0; i < 1024; ++i)
	{
		char const* str = strings + 129 * i;
		kt::ArrayHashMap<char const*, uint8_t>::KeyIterator it = map.FindKeyIt(str);
		REQUIRE(it != map.EndKeys());

		REQUIRE(*map.KeyToValueIt(it) == it.Key()[str[0] % 128]);
		map.Erase(it);
	}
}

TEST_CASE("ArrayHashMap iteration test 1", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	bool found[num] = {};

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map[i] = i;
	}

	for (kt::ArrayHashMap<uint32_t, uint32_t>::KeyIterator it = map.BeginKeys(); it != map.EndKeys(); ++it)
	{
		REQUIRE(it.Key() == *map.KeyToValueIt(it));
		REQUIRE(found[it.Key()] == false);
		found[it.Key()] = true;
	}

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(found[i] == true);
	}
}

TEST_CASE("ArrayHashMap invalid key lookup", "[ArrayHashMap]")
{
	kt::ArrayHashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map[i] = i;
	}

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(map.Find(i) != map.EndValues());
		REQUIRE(map.Find(i + num) == map.EndValues());
	}

	kt::ArrayHashMap<uint32_t, uint32_t> copy = map.MakeCopy();

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(copy.Find(i) != copy.EndValues());
		REQUIRE(copy.Find(i + num) == copy.EndValues());
	}
}

TEST_CASE("ArrayHashmap operator[]", "[ArrayHashMap]")
{
	kt::ArrayHashMap<kt::StringView, uint32_t> map;

	map["Hello"] = 5;

	kt::ArrayHashMap<kt::StringView, uint32_t>::KeyIterator it = map.FindKeyIt("Hello");

	REQUIRE(it != map.EndKeys());
	REQUIRE(it.Key() == "Hello");
	REQUIRE(*map.KeyToValueIt(it) == 5);

	map.Insert("cats", 3);

	it = map.FindKeyIt("cats");

	REQUIRE(it != map.EndKeys());
	REQUIRE(it.Key() == "cats");
	REQUIRE(*map.KeyToValueIt(it) == 3);

	map["cats"] = 23;

	it = map.FindKeyIt("cats");


	REQUIRE(it != map.EndKeys());
	REQUIRE(it.Key() == "cats");
	REQUIRE(*map.KeyToValueIt(it) == 23);

}