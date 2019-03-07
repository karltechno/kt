#include "catch.hpp"

#include <string.h>

#include <kt/kt.h>
#include <kt/Random.h>
#include <kt/Serialization.h>
#include <kt/HashMapDeclare.h>
#include <kt/HashMap.h>

TEST_CASE("Basic reader test", "[Serialization]")
{
	uint32_t blockA[128];
	uint32_t blockB[128];

	kt::XorShift32 rng;
	
	for (uint32_t& u : blockA)
	{
		u = rng.Gen32();
	}

	kt::StaticMemoryBlockReader reader(blockA, sizeof(blockA));
	
	for (uint32_t i = 0; i < KT_ARRAY_COUNT(blockB); ++i)
	{
		reader.Read(blockB[i]);
	}

	CHECK(memcmp(blockA, blockB, sizeof(blockA)) == 0);

	uint32_t dummy;
	CHECK(!reader.Read(dummy));
	uint64_t numRead;
	CHECK(!reader.ReadBytes(&dummy, sizeof(uint32_t), &numRead));
	CHECK(numRead == 0);
}

TEST_CASE("Basic writer test", "[Serialization]")
{
	uint32_t blockA[128];
	uint32_t blockB[128];

	kt::XorShift32 rng;

	for (uint32_t& u : blockA)
	{
		u = rng.Gen32();
	}

	kt::StaticMemoryBlockWriter writer(blockB, sizeof(blockB));

	for (uint32_t i = 0; i < KT_ARRAY_COUNT(blockB); ++i)
	{
		writer.Write(blockA[i]);
	}

	CHECK(memcmp(blockA, blockB, sizeof(blockA)) == 0);
}

TEST_CASE("Serialize array u32", "[Serialization]")
{
	constexpr uint32_t c_size = 1024;
	kt::Array<uint32_t> arrA;
	kt::Array<uint32_t> arrB;
	uint32_t* ptr = arrA.PushBack_Raw(c_size);

	for (uint32_t i = 0; i < c_size; ++i)
	{
		ptr[i] = i;
	}

	char serializeBlock[c_size * sizeof(uint32_t) + sizeof(uint32_t)];

	{
		kt::StaticMemoryBlockWriter writer(serializeBlock, sizeof(serializeBlock));
		kt::ISerializer serializer(&writer, 0);

		Serialize(&serializer, arrA);
	}

	{
		kt::StaticMemoryBlockReader reader(serializeBlock, sizeof(serializeBlock));
		kt::ISerializer serializer(&reader, 0);

		Serialize(&serializer, arrB);
	}

	REQUIRE(arrA.Size() == arrB.Size());
	for (uint32_t i = 0; i < c_size; ++i)
	{
		CHECK(arrB[i] == arrA[i]);
	}
}

TEST_CASE("Serialize hashmap u32 -> u32", "[Serialization]")
{
	constexpr uint32_t c_size = 1023;
	kt::HashMap<uint32_t, uint32_t> mapA;
	kt::HashMap<uint32_t, uint32_t> mapB;

	mapA.Reserve(c_size);

	for (uint32_t i = 0; i < c_size; ++i)
	{
		mapA.Insert(i, i * 3 + 1);
	}

	char serializeBlock[sizeof(uint32_t) * c_size * 8];

	{
		kt::StaticMemoryBlockWriter writer(serializeBlock, sizeof(serializeBlock));
		kt::ISerializer serializer(&writer, 0);

		Serialize(&serializer, mapA);
	}

	{
		kt::StaticMemoryBlockReader reader(serializeBlock, sizeof(serializeBlock));
		kt::ISerializer serializer(&reader, 0);

		Serialize(&serializer, mapB);
	}

	REQUIRE(mapA.Size() == mapB.Size());
	for (kt::HashMap<uint32_t, uint32_t>::Iterator itA = mapA.Begin();
		 itA != mapA.End();
		 ++itA)
	{
		kt::HashMap<uint32_t, uint32_t>::Iterator itB = mapB.Find(itA->m_key);
		REQUIRE(itB != mapB.End());
		CHECK(itB->m_val == itA->m_val);
	}
}