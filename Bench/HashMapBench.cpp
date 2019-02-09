#include <unordered_map>

#include <kt/kt.h>
#include "Bench.h"
#include "kt/HashMap.h"
#include "kt/Random.h"
#include "kt/Array.h"
#include <vector>
#include "kt/Strings.h"

struct StrCmpFunctor
{
	bool operator()(char const* _lhs, char const* _rhs) const
	{
		return kt::StrCmp(_lhs, _rhs) == 0;
	}
};

struct UnorderedMap_StringHash
{
	std::size_t operator()(char const* _str) const
	{
		return kt::XXHash_32(_str, kt::StrLen(_str));
	}
};

template <typename T>
struct UnorderedMap_XXHash
{
	std::size_t operator()(T const& _t) const
	{
		return kt::XXHash_32(&_t, sizeof(T));
	}
};

KT_BENCH(ktHashMap_Insertion_u32_u32, 128)
{
	kt::IntegerKeyMap<uint32_t, uint32_t> map;

	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < 1024; ++i)
	{
		map.Insert(i * 2 + 1, i * 5 - 2);
	}
}


KT_BENCH(HashMapSTL_Insertion_u32_u32, 128)
{
	std::unordered_map<uint32_t, uint32_t, UnorderedMap_XXHash<uint32_t>> map;
	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < 1024; ++i)
	{
		map.insert(std::make_pair<uint32_t, uint32_t>(i * 2 + 1, i * 5 - 2));
	}
}


KT_BENCH(ktHashMap_Insertion_string_u32, 128)
{
	kt::HashMap<char const*, uint32_t> map;

	uint32_t const NUM_STRINGS = 4096 * 4;

	char* strings = (char*)kt::Malloc((NUM_STRINGS * 129));
	KT_SCOPE_EXIT(kt::Free(strings));

	kt::XorShift32 rng;

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char* str = strings + 129 * i;
		for (uint32_t j = 0; j < 128; j += 4)
		{
			*(uint32_t*)&str[j] = rng.Gen32();
		}
		str[128] = 0;
	}

	{
		kt::ScopedBenchStart bench;

		for (uint32_t i = 0; i < NUM_STRINGS; ++i)
		{
			char const* str = strings + 129 + i;
			map.Insert(str, i * 5 - 2);
		}
	}
}


KT_BENCH(HashMapSTL_Insertion_string_u32, 128)
{
	std::unordered_map<char const*, uint32_t, UnorderedMap_StringHash, StrCmpFunctor> map;
	uint32_t const NUM_STRINGS = 4096 * 4;

	char* strings = (char*)kt::Malloc((NUM_STRINGS * 129));
	KT_SCOPE_EXIT(kt::Free(strings));

	kt::XorShift32 rng;

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char* str = strings + 129 * i;
		for (uint32_t j = 0; j < 128; j += 4)
		{
			*(uint32_t*)&str[j] = rng.Gen32();
		}
		str[128] = 0;
	}

	{
		kt::ScopedBenchStart bench;

		for (uint32_t i = 0; i < NUM_STRINGS; ++i)
		{
			char const* str = strings + 129 * i;
			map.insert(std::make_pair<char const*& , uint32_t>(str, i * 5 - 2));
		}
	}
}


KT_BENCH(ktHashMap_Lookup_string_u32, 128)
{
	kt::HashMap<char const*, uint32_t> map;

	uint32_t const NUM_STRINGS = 4096 * 4;


	char* strings = (char*)kt::Malloc((NUM_STRINGS * 129));
	KT_SCOPE_EXIT(kt::Free(strings));


	kt::XorShift32 rng;

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char* str = strings + 129 * i;
		for (uint32_t j = 0; j < 128; j += 4)
		{
			*(uint32_t*)&str[j] = rng.Gen32();
		}
		str[128] = 0;
	}

	map.Reserve(NUM_STRINGS);

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char const* str = strings + 129 * i;
		map.Insert(str, i * 5 - 2);
	}

	{
		kt::ScopedBenchStart bench;
		for (uint32_t i = 0; i < NUM_STRINGS; ++i)
		{
			char const* str = strings + 129 * i;
			auto it = map.Find(str);
			KT_UNUSED(it);
		}
	}
}


KT_BENCH(HashMapSTL_Lookup_string_u32, 128)
{
	std::unordered_map<char const*, uint32_t, UnorderedMap_StringHash, StrCmpFunctor> map;

	uint32_t const NUM_STRINGS = 4096 * 4;


	char* strings = (char*)kt::Malloc((NUM_STRINGS * 129));
	KT_SCOPE_EXIT(kt::Free(strings));

	kt::XorShift32 rng;

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char* str = strings + 129 * i;
		for (uint32_t j = 0; j < 128; j += 4)
		{
			*(uint32_t*)&str[j] = rng.Gen32();
		}
		str[128] = 0;
	}

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char const* str = strings + 129 * i;
		map.insert(std::make_pair<char const*&, uint32_t>(str, i * 5 - 2));
	}


	{
		kt::ScopedBenchStart bench;
		for (uint32_t i = 0; i < NUM_STRINGS; ++i)
		{
			char const* str = strings + 129 * i;
			auto it = map.find(str);
			KT_UNUSED(it);
		}
	}
}


KT_BENCH(ktHashMap_Lookup_u32_u32, 128)
{
	kt::IntegerKeyMap<uint32_t, uint32_t> map;

	map.Reserve(1024 * 8);

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		map.Insert(i * 2 + 1, i * 5 - 2);
	}

	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		auto it = map.Find(i * 2 + 1);
		KT_UNUSED(it);
	}
}


KT_BENCH(HashMapSTL_Lookup_u32_u32, 128)
{
	std::unordered_map<uint32_t, uint32_t> map;
	map.reserve(1024 * 8);

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		map.insert(std::make_pair<uint32_t, uint32_t>(i * 2 + 1, i * 5 - 2));
	}

	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		auto it = map.find(i * 2 + 1);
		KT_UNUSED(it);
	}
}

struct HashMap_BigData
{
	char junk[1024];
};

KT_BENCH(ktHashMap_Lookup_u32_BigData, 128)
{
	kt::IntegerKeyMap<uint32_t, HashMap_BigData> map;

	map.Reserve(1024 * 8);

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		map.Insert(i * 2 + 1, HashMap_BigData{});
	}

	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		auto it = map.Find(i * 2 + 1);
		KT_UNUSED(it);
	}
}

KT_BENCH(HashMapSTL_Lookup_u32_BigData, 128)
{
	std::unordered_map<uint32_t, HashMap_BigData> map;
	map.reserve(1024 * 8);

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		map.insert(std::make_pair<uint32_t, HashMap_BigData>(i * 2 + 1, HashMap_BigData{}));
	}

	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < 1024 * 8; ++i)
	{
		auto it = map.find(i * 2 + 1);
		KT_UNUSED(it);
	}
}


KT_BENCH(ktArray_Iterate_4096_u32, 256)
{
	kt::Array<uint32_t> arr;

	for (uint32_t i = 0; i < 4096; ++i)
	{
		arr.PushBack(i);
	}

	kt::ScopedBenchStart bench;
	for (kt::Array<uint32_t>::Iterator it = arr.Begin(); it != arr.End(); ++it)
	{
		*it = *it + 1;
	}
}

KT_BENCH(STLVector_Iterate_4096_u32, 256)
{
	std::vector<uint32_t> arr;

	for (uint32_t i = 0; i < 4096; ++i)
	{
		arr.push_back(i);
	}

	kt::ScopedBenchStart bench;
	for (std::vector<uint32_t>::iterator it = arr.begin(); it != arr.end(); ++it)
	{
		*it = *it + 1;
	}
}


KT_BENCH(ktHashMap_Lookup_Failed_u32_u32, 128)
{
	kt::IntegerKeyMap<uint32_t, uint32_t> map;

	uint32_t const num = 2048;

	map.Reserve(num);

	for (uint32_t i = 0; i < num * 8; ++i)
	{
		map.Insert(num, i * 5 - 2);
	}

	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < num * 8; ++i)
	{
		auto it = map.Find(num + i);
		KT_UNUSED(it);
	}
}

KT_BENCH(STLHashMap_Lookup_Failed_u32_u32, 128)
{
	std::unordered_map<uint32_t, uint32_t, std::hash<uint32_t>> map;

	uint32_t const num = 2048;

	map.reserve(num);

	for (uint32_t i = 0; i < num * 8; ++i)
	{
		map.insert(std::pair<uint32_t, uint32_t>(num, i * 5 - 2));
	}

	kt::ScopedBenchStart bench;

	for (uint32_t i = 0; i < num * 8; ++i)
	{
		auto it = map.find(num + i);
		KT_UNUSED(it);
	}
}


KT_BENCH(ktHashMap_Remove_string_u32, 128)
{
	kt::HashMap<char const*, uint32_t> map;

	uint32_t const NUM_STRINGS = 4096 * 4;

	char* strings = (char*)kt::Malloc((NUM_STRINGS * 129));
	KT_SCOPE_EXIT(kt::Free(strings));

	kt::XorShift32 rng;

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char* str = strings + 129 * i;
		for (uint32_t j = 0; j < 128; j += 4)
		{
			*(uint32_t*)&str[j] = rng.Gen32();
		}
		str[128] = 0;
	}

	{

		for (uint32_t i = 0; i < NUM_STRINGS; ++i)
		{
			char const* str = strings + 129 + i;
			map.Insert(str, i * 5 - 2);
		}
	}

	{
		kt::ScopedBenchStart bench;
		for (auto it = map.Begin(); it != map.End(); /* */)
		{
			it = map.Erase(it);
		}
	}
}


KT_BENCH(HashMapSTL_Remove_string_u32, 128)
{
	std::unordered_map<char const*, uint32_t, UnorderedMap_StringHash, StrCmpFunctor> map;
	uint32_t const NUM_STRINGS = 4096 * 4;

	char* strings = (char*)kt::Malloc((NUM_STRINGS * 129));
	KT_SCOPE_EXIT(kt::Free(strings));

	kt::XorShift32 rng;

	for (uint32_t i = 0; i < NUM_STRINGS; ++i)
	{
		char* str = strings + 129 * i;
		for (uint32_t j = 0; j < 128; j += 4)
		{
			*(uint32_t*)&str[j] = rng.Gen32();
		}
		str[128] = 0;
	}

	{
		for (uint32_t i = 0; i < NUM_STRINGS; ++i)
		{
			char const* str = strings + 129 + i;
			map.insert(std::make_pair<char const*&, uint32_t>(str, i * 5 - 2));
		}
	}

	{
		kt::ScopedBenchStart bench;
		for (auto it = map.begin(); it != map.end(); /* */)
		{
			it = map.erase(it);
		}
	}
}