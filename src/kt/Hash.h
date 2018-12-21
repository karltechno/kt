#pragma once
#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include "External/xxhash.h"

#include "kt.h"
#include "Strings.h"


namespace kt
{
constexpr uint32_t c_xxhash32DefaultSeed = 0x45f2e2a6;
constexpr uint64_t c_xxhash64DefaultSeed = 0x45f2e2a6b4c81298ULL;

// Hash finalizer from murmur hash.
KT_FORCEINLINE uint32_t HashMix32(uint32_t _val);

// Hash finalizer from murmur hash.
KT_FORCEINLINE uint64_t HashMix64(uint64_t _val);

// Jenkins 32 -> 64 hash.
KT_FORCEINLINE uint32_t Hash64to32(uint64_t _val);

// 32 bit XXHash.
KT_FORCEINLINE uint32_t XXHash_32(void const* _p, uint32_t _sz);

// 64 bit XXHash.
KT_FORCEINLINE uint64_t XXHash_64(void const* _p, uint32_t _sz);

struct XXHash_Incremental32
{
	XXHash_Incremental32();

	void Process(void const* _p, uint32_t _sz);
	uint32_t Finalize() const;
	void Reset();

private:
	XXH32_state_t m_state;
};

struct XXHash_Incremental64
{
	XXHash_Incremental64();

	void Process(void const* _p, uint32_t _sz);
	uint64_t Finalize() const;
	void Reset();

private:
	XXH64_state_s m_state;
};


}

#include "inl/Hash.inl"