#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace kt
{

template <uint32_t SizeT>
uint32_t StringFormat(char (&buff)[SizeT], char const* _fmt, ...)
{
	va_list args;
	va_start(args, _fmt);
	uint32_t const ret = StringFormatVa(&buff[0], SizeT, _fmt, args);
	va_end(args);
	return ret;
}

template<uint32_t SizeT>
StaticString<SizeT>::StaticString()
{
	m_data[0] = 0;
}

template<uint32_t SizeT>
StaticString<SizeT>::StaticString(char const * _str)
{
	Append(_str);
}

template<uint32_t SizeT>
template <uint32_t T_OtherSize>
StaticString<SizeT>& StaticString<SizeT>::operator=(StaticString<T_OtherSize> const& _otherString)
{
	Clear();
	Append(_otherString);
	return *this;
}


template <uint32_t SizeT>
StaticString<SizeT>& StaticString<SizeT>::operator=(const char* _str)
{
	Clear();
	Append(_str);
	return *this;
}

template<uint32_t SizeT>
char* StaticString<SizeT>::Data()
{
	return m_data;
}

template<uint32_t SizeT>
char const* StaticString<SizeT>::Data() const
{
	return m_data;
}

template<uint32_t SizeT>
char & StaticString<SizeT>::operator[](uint32_t const _idx)
{
	KT_ASSERT(_idx < m_size);
	return m_data[_idx];
}

template<uint32_t SizeT>
char StaticString<SizeT>::operator[](uint32_t const _idx) const
{
	KT_ASSERT(_idx < m_size);
	return m_data[_idx];
}

template<uint32_t SizeT>
void StaticString<SizeT>::Append(uint32_t const _uint)
{
	int const wouldHaveWritten = snprintf(m_data + m_size, c_capacityNoNull - m_size, "%u", _uint);

	uint32_t const nullTerm = Min<uint32_t>(wouldHaveWritten + m_size, c_capacityNoNull - 1);
	m_data[nullTerm] = 0;
	m_size = Min<uint32_t>(wouldHaveWritten + m_size, c_capacityNoNull);
}

template<uint32_t SizeT>
void StaticString<SizeT>::Append(const char * _str)
{
	Append(_str, (uint32_t)StrLen(_str));
}

template<uint32_t SizeT>
void StaticString<SizeT>::Append(const char* _str, uint32_t const _size)
{
	uint32_t const toAppend = Min(c_capacityNoNull - m_size, _size);

	memcpy(m_data + m_size, _str, toAppend);
	m_size += toAppend;
	m_data[m_size] = 0;
}


template <uint32_t SizeT>
void StaticString<SizeT>::Append(char const _c)
{
	if (m_size < c_capacityNoNull)
	{
		m_data[m_size++] = _c;
		m_data[m_size] = 0;
	}
}

template<uint32_t SizeT>
void StaticString<SizeT>::Clear()
{
	m_size = 0;
	m_data[0] = 0;
}

template<uint32_t SizeT>
uint32_t StaticString<SizeT>::Size() const
{
	return m_size;
}

template<uint32_t SizeT>
bool StaticString<SizeT>::Empty() const
{
	return m_size == 0;
}

template<uint32_t SizeT>
template<uint32_t T_OtherSize>
StaticString<SizeT>::StaticString(StaticString<T_OtherSize> const & _otherString)
{
	Append(_otherString.Data(), _otherString.Size());
}

template <uint32_t SizeT>
void StaticString<SizeT>::AppendFmt(const char* _fmt, ...)
{
	uint32_t const szLeft = c_capacityNoNull - m_size;
	va_list args;
	va_start(args, _fmt);
	int const wouldHaveWritten = vsnprintf(m_data + m_size, szLeft, _fmt, args);
	va_end(args);

	m_size += Min<uint32_t>(szLeft, wouldHaveWritten);
	m_data[m_size] = 0;
}

template <uint32_t SizeT>
uint32_t StaticString<SizeT>::Capacity() const
{
	return SizeT;
}

template <uint32_t SizeT>
uint32_t StaticString<SizeT>::CapacityWithoutNull() const
{
	return c_capacityNoNull;
}

template <uint32_t SizeT>
template <uint32_t T_OtherSize>
void StaticString<SizeT>::Append(StaticString<T_OtherSize> const& _other)
{
	Append(_other.Data(), _other.Size());
}

template <uint32_t SizeT>
void StaticString<SizeT>::Append(StringView const& _view)
{
	Append(_view.m_ptr, _view.m_size);
}


template <uint32_t SizeT>
kt::StaticString<SizeT>::StaticString(StringView const& _view)
{
	Append(_view);
}

template <uint32_t SizeT>
StaticString<SizeT>& kt::StaticString<SizeT>::operator=(StringView const& _view)
{
	Clear();
	Append(_view);
	return *this;
}

template<uint32_t SizeT>
char* StaticString<SizeT>::Begin()
{
	return m_data;
}

template<uint32_t SizeT>
char* StaticString<SizeT>::End()
{
	return m_data + m_size;
}

template<uint32_t SizeT>
char const* StaticString<SizeT>::Begin() const
{
	return m_data;
}

template<uint32_t SizeT>
char const* StaticString<SizeT>::End() const
{
	return m_data + m_size;
}

template<uint32_t SizeT>
void StaticString<SizeT>::Resize(uint32_t const _sz)
{
	KT_ASSERT(_sz <= c_capacityNoNull);
	m_size = Min(_sz, c_capacityNoNull);
	m_data[m_size] = 0;
}


template<uint32_t T_SizeA, uint32_t T_SizeB>
bool operator==(StaticString<T_SizeA> const& _a, StaticString<T_SizeB> const& _b)
{
	if (_a.Size() != _b.Size())
	{
		return false;
	}

	return StrNCmp(_a.Data(), _b.Data(), _a.Size()) == 0;
}

template <uint32_t SizeT>
bool operator==(StaticString<SizeT> const& _a, char const* _cstr)
{
	return StrCmp(_a.Data(), _cstr) == 0;
}


inline bool operator==(StringView const& _lhs, StringView const& _rhs)
{
	if (_lhs.m_size != _rhs.m_size)
	{
		return false;
	}

	return StrNCmp(_lhs.m_ptr, _rhs.m_ptr, _lhs.m_size) == 0;
}

template <uint32_t SizeT>
StringView::StringView(StaticString<SizeT> const& _str)
	: m_ptr(_str.Data())
	, m_size(_str.Size())
{
}

char ToLower(char _c)
{
	return (_c >= 'A' && _c <= 'Z') ? (_c | 0x20) : _c;
}



}