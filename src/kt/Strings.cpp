#include <string.h>

#include "Strings.h"

namespace kt
{

StringView::StringView(char const* _ptr)
	: m_ptr(_ptr)
	, m_size((uint32_t)::strlen(_ptr))
{
}

StringView::StringView(char const* _begin, char const* _end)
{
	KT_ASSERT(_end >= _begin);
	m_ptr = _begin;
	m_size = (uint32_t)(_end - _begin);
}

StringView::StringView(char const* _str, uint32_t const _len)
{
	m_ptr = _str;
	m_size = _len;
}

uint32_t StringView::Size() const 
{ 
	return m_size; 
}

bool StringView::Empty() const
{
	return m_size == 0;
}

char const* StringView::Data() const 
{ 
	return m_ptr; 
}

char const* StringView::Begin() const
{
	return m_ptr;
}

char const* StringView::End() const
{
	return m_ptr + m_size;
}

StringView StringView::Suffix(uint32_t _begin) const
{
	return Slice(_begin, Size());
}


StringView StringView::Prefix(uint32_t _end) const
{
	return Slice(0, _end);
}

StringView StringView::Slice(uint32_t _begin, uint32_t _end) const
{
	KT_ASSERT(_begin <= _end);
	KT_ASSERT(_end <= Size());
	return StringView(m_ptr + _begin, m_ptr + _end);
}

int32_t StrCmp(char const* _lhs, char const* _rhs)
{
	int32_t const res = ::strcmp(_lhs, _rhs);
	if (res == 0) return 0;
	return res < 0 ? -1 : 1;
}

int32_t StrCmpI(char const* _lhs, char const* _rhs)
{
	int32_t const res = ::_stricmp(_lhs, _rhs);
	if (res == 0) return 0;
	return res < 0 ? -1 : 1;
}

int32_t StrCmpI(StringView const& _lhs, StringView const& _rhs)
{
	int32_t i = 0;
	int32_t end = kt::Min(_lhs.m_size, _rhs.m_size);

	char const* p0 = _lhs.m_ptr;
	char const* p1 = _rhs.m_ptr;

	for (; i < end; ++i)
	{
		char c0 = *p0++;
		c0 = (c0 >= 'A' && c0 <= 'Z') ? (c0 | 0x20) : c0;

		char c1 = *p1++;
		c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;

		if (c0 != c1)
		{
			return c0 < c1 ? -1 : 1;
		}
	}

	if (_lhs.m_size != _rhs.m_size)
	{
		return _lhs.m_size > _rhs.m_size ? 1 : -1;
	}

	return 0;
}

uint32_t StrLen(char const* _str)
{
	return (uint32_t)::strlen(_str);
}

uint32_t StringFormatVa(char* _buff, uint32_t const _buffSz, char const* _fmt, va_list va)
{
	int res = ::vsnprintf(_buff, _buffSz, _fmt, va);
	KT_ASSERT(res >= 0);
	return (uint32_t)res;
}

uint32_t StringFormat(char* _buff, uint32_t const _buffSz, char const* _fmt, ...)
{
	va_list args;
	va_start(args, _fmt);
	int const wouldHaveWritten = ::vsnprintf(_buff, _buffSz, _fmt, args);
	KT_ASSERT(wouldHaveWritten >= 0);
	va_end(args);
	return (uint32_t)wouldHaveWritten;
}

StringView StrFind(StringView const& _view, char const _c)
{
	for (uint32_t i = 0; i < _view.Size(); ++i)
	{
		if (_view[i] == _c)
		{
			return StringView(_view.m_ptr + i, _view.End());
		}
	}

	return StringView();
}

StringView StrFindR(StringView const& _view, char const _c)
{
	for (uint32_t i = _view.Size(); i > 0; --i)
	{
		if (_view[i - 1] == _c)
		{
			return StringView(_view.m_ptr + (i - 1), _view.End());
		}
	}

	return StringView();
}

int32_t StrNCmp(char const* _lhs, char const* _rhs, uint32_t const _max)
{
	return ::strncmp(_lhs, _rhs, _max);
}

}