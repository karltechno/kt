#include "Hash.h"

namespace kt
{

uint32_t StringHash(char const* _str)
{
	return XXH32(_str, StrLen(_str), 0);
}

uint32_t StringHash(kt::StringView const& _str)
{
	return XXH32(_str.m_ptr, _str.m_size, 0);
}

uint32_t StringHashI(char const* _str)
{
	return StringHashI(kt::StringView{ _str });
}

uint32_t StringHashI(kt::StringView const& _str)
{
	char* buff = (char*)KT_ALLOCA(_str.m_size);

	char* dest = buff;
	char* destEnd = buff + _str.m_size;
	char const* src = _str.m_ptr;

	while (dest != destEnd)
	{
		char const c = *src++;
		*dest++ = (c >= 'A' && c <= 'Z') ? (c | 0x20) : c;
	}

	return XXH32(buff, _str.m_size, 0);
}

// Case sensitive string hash.
uint64_t StringHash64(char const* _str)
{
	return StringHash64(kt::StringView{ _str });
}

uint64_t StringHash64(kt::StringView const& _str)
{
	return XXH64(_str.m_ptr, _str.m_size, 0);
}

uint64_t StringHash64I(char const* _str)
{
	return StringHash64I(StringView{ _str });
}

uint64_t StringHash64I(kt::StringView const& _str)
{
	char* buff = (char*)KT_ALLOCA(_str.m_size);

	char* dest = buff;
	char* destEnd = buff + _str.m_size;
	char const* src = _str.m_ptr;

	while (dest != destEnd)
	{
		char const c = *src++;
		*dest++ = (c >= 'A' && c <= 'Z') ? (c | 0x20) : c;
	}

	return XXH64(buff, _str.m_size, 0);
}


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
	XXH32_reset(&m_state, 0);
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
	XXH64_reset(&m_state, 0);
}

}