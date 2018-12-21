#include "Hash.h"

namespace kt
{

XXHash_Incremental32::XXHash_Incremental32()
{
	Reset();
}

void XXHash_Incremental32::Process(void const* _p, uint32_t _sz)
{
	XXH32_update(&m_state, _p, _sz);
}

uint32_t XXHash_Incremental32::Finalize() const
{
	return XXH32_digest(&m_state);
}

void XXHash_Incremental32::Reset()
{
	XXH32_reset(&m_state, c_xxhash32DefaultSeed);
}

XXHash_Incremental64::XXHash_Incremental64()
{
	Reset();
}

void XXHash_Incremental64::Process(void const* _p, uint32_t _sz)
{
	XXH64_update(&m_state, _p, _sz);
}

uint64_t XXHash_Incremental64::Finalize() const
{
	return XXH64_digest(&m_state);
}

void XXHash_Incremental64::Reset()
{
	XXH64_reset(&m_state, c_xxhash64DefaultSeed);
}

}