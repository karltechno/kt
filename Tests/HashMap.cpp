#include <memory>
#include "catch.hpp"

#include <kt/kt.h>
#include <kt/HashMap.h>
#include <kt/Random.h>
#include <kt/Strings.h>

TEST_CASE("HashMap Copy", "")
{
	kt::HashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::HashMap<uint32_t, uint32_t> mapCopy = map.MakeCopy();

	CHECK(mapCopy.Size() == map.Size());

	for (uint32_t i = 0; i < num; ++i)
	{
		CHECK(mapCopy.Find(i) != mapCopy.End());
	}
}


TEST_CASE("HashMap operator=", "")
{
	kt::HashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::HashMap<uint32_t, uint32_t> mapCopy;
	mapCopy[2] = 4;
	mapCopy = map.MakeCopy();

	CHECK(mapCopy.Size() == map.Size());

	for (uint32_t i = 0; i < num; ++i)
	{
		CHECK(mapCopy.Find(i) != mapCopy.End());
	}
}

TEST_CASE("HashMap move", "")
{
	kt::HashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::HashMap<uint32_t, uint32_t> mapCopy(std::move(map));

	CHECK(mapCopy.Size() == num);
	CHECK(map.Size() == 0);

	for (uint32_t i = 0; i < num; ++i)
	{
		CHECK(mapCopy.Find(i) != mapCopy.End());
	}
}

TEST_CASE("HashMap operator=&&", "")
{
	kt::HashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map.Insert(i, i);
	}
	kt::HashMap<uint32_t, uint32_t> mapCopy;
	mapCopy[2] = 4;
	mapCopy = std::move(map);

	CHECK(mapCopy.Size() == num);
	CHECK(map.Size() == 0);

	for (uint32_t i = 0; i < num; ++i)
	{
		CHECK(mapCopy.Find(i) != mapCopy.End());
	}
}

TEST_CASE("HashMap leak test 1", "")
{
	kt::HashMap<uint32_t, std::shared_ptr<int>> map;

	std::shared_ptr<int> sPtr = std::make_shared<int>();

	std::weak_ptr<int> weak = sPtr;

	for (uint32_t i = 0; i < 512; ++i)
	{
		map.Insert(i, sPtr);
	}

	CHECK((uint32_t)sPtr.use_count() == map.Size() + 1);

	uint32_t useCount = (uint32_t)sPtr.use_count();

	for (kt::HashMap<uint32_t, std::shared_ptr<int>>::Iterator it = map.Begin(); it != map.End(); )
	{
		it = map.Erase(it);
		CHECK((uint32_t)sPtr.use_count() == --useCount);
	}

	sPtr.reset();
	CHECK(weak.lock() == nullptr);

}

TEST_CASE("HashMap leak test 2", "")
{
	kt::HashMap<uint32_t, std::shared_ptr<int>> map;

	std::shared_ptr<int> sPtr = std::make_shared<int>();

	std::weak_ptr<int> weak = sPtr;

	for (uint32_t i = 0; i < 512; ++i)
	{
		map.Insert(i, sPtr);
	}

	CHECK((uint32_t)sPtr.use_count() == map.Size() + 1u);

	{
		kt::HashMap<uint32_t, std::shared_ptr<int>> map2(std::move(map));

		CHECK((uint32_t)sPtr.use_count() == map2.Size() + 1);
	}

	sPtr.reset();
	CHECK(weak.lock() == nullptr);
}


TEST_CASE("HashMap Deletetion Test 1", "")
{
	kt::HashMap<uint32_t, uint32_t> map;

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
		auto it = map.Find(i);
		REQUIRE(it != map.End());
		CHECK((*it).m_val == i * 2);
	}

	for (uint32_t i = 0; i < num; ++i)
	{
		uint32_t const idx = arr[i];

		auto it = map.Find(idx);
		REQUIRE(it != map.End());
		CHECK((*it).m_val == idx * 2);

		map.Erase(it);
	}

	CHECK(map.Size() == 0);
}



TEST_CASE("HashMap string lookup 1", "")
{
	kt::HashMap<char const*, uint8_t> map;

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
		kt::HashMap<char const*, uint8_t>::ConstIterator it = map.Find(str);
		REQUIRE(it != map.End());
		CHECK(it->m_val == it->m_key[str[0] % 128]);
		map.Erase(it);
	}
}

TEST_CASE("Hash map iteration test 1", "")
{
	kt::HashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	bool found[num] = {};

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map[i] = i;
	}

	for (kt::HashMap<uint32_t, uint32_t>::ConstIterator it = map.Begin(); it != map.End(); ++it)
	{
		REQUIRE(it->m_key == it->m_val);
		CHECK(found[it->m_key] == false);
		found[it->m_key] = true;
	}

	for (uint32_t i = 0; i < num; ++i)
	{
		CHECK(found[i] == true);
	}
}

TEST_CASE("Hash map invalid key lookup", "")
{
	kt::HashMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num; ++i)
	{
		map[i] = i;
	}

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(map.Find(i) != map.End());
		CHECK(map.Find(i + num) == map.End());
	}

	kt::HashMap<uint32_t, uint32_t> copy = map.MakeCopy();

	for (uint32_t i = 0; i < num; ++i)
	{
		REQUIRE(copy.Find(i) != copy.End());
		CHECK(copy.Find(i + num) == copy.End());
	}
}

TEST_CASE("Hash map operator[]", "")
{
	kt::HashMap<kt::StringView, uint32_t> map;

	map["Hello"] = 5;

	kt::HashMap<kt::StringView, uint32_t>::Iterator it = map.Find("Hello");

	REQUIRE(it != map.End());
	CHECK(it->m_key == "Hello");
	CHECK(it->m_val == 5);

	map.Insert("cats", 3);

	it = map.Find("cats");

	REQUIRE(it != map.End());
	CHECK(it->m_key == "cats");
	CHECK(it->m_val == 3);

	map["cats"] = 23;

	it = map.Find("cats");


	REQUIRE(it != map.End());
	CHECK(it->m_key == "cats");
	CHECK(it->m_val == 23);

}