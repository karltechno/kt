#pragma once
#include "../kt.h"
#include "../Macros.h"
#include "../Hash.h"


namespace kt
{



KT_FORCEINLINE uint32_t HashMix32(uint32_t const _val)
{
	uint32_t h = _val;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}

KT_FORCEINLINE uint64_t HashMix64(uint64_t const _val)
{
	uint64_t k = _val;
	k ^= k >> 33;
	k *= 0xff51afd7ed558ccdLLU;
	k ^= k >> 33;
	k *= 0xc4ceb9fe1a85ec53LLU;
	k ^= k >> 33;

	return k;
}

KT_FORCEINLINE uint32_t Hash64to32(uint64_t const _val)
{
	uint64_t v = _val;
	v = (~v) + (v << 18);
	v = v ^ (v >> 31);
	v = v * 21;
	v = v ^ (v >> 11);
	v = v + (v << 6);
	v = v ^ (v >> 22);
	return (uint32_t)v;
}

KT_FORCEINLINE uint32_t XXHash_32(void const* _p, uint32_t _sz)
{
	return XXH32(_p, _sz, c_xxhash32DefaultSeed);
}

KT_FORCEINLINE uint64_t XXHash_64(void const* _p, uint32_t _sz)
{
	return XXH64(_p, _sz, c_xxhash64DefaultSeed);
}

}
