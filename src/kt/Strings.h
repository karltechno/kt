#pragma once
#include "kt.h"
#include "Memory.h"

namespace kt
{

struct StringView;

template <uint32_t SizeT>
class StaticString
{
public:
	static_assert(SizeT > 0, "String size must be greater than 0.");

	StaticString();

	// Char* constructor.
	StaticString(char const* _str);
	
	// StringView constructor.
	StaticString(StringView const& _view);

	StaticString(StaticString<SizeT> const&) = default;
	StaticString& operator=(StaticString<SizeT> const&) = default;

	// Copy ctor from other static string
	template <uint32_t T_OtherSize>
	StaticString(StaticString<T_OtherSize> const& _otherString);

	// Assignment operator from other static string.
	template <uint32_t T_OtherSize>
	StaticString& operator=(StaticString<T_OtherSize> const& _otherString);

	// Assignment operator from char*.
	StaticString& operator=(const char* _str);

	// Assignment operator from StringView
	StaticString& operator=(StringView const& _view);

	// Begin and End iterators
	char* Begin();
	char* End();
	char const* Begin() const;
	char const* End() const;

	// Raw null terminated string.
	char* Data();
	char const* Data() const;

	// Character indexing.
	char& operator[](uint32_t const _idx);
	char operator[](uint32_t const _idx) const;

	// Append another Static String.
	template <uint32_t T_OtherSize>
	void Append(StaticString<T_OtherSize> const& _other);

	// Append uint32_t
	void Append(uint32_t const _uint);

	// Append c string.
	void Append(char const* _str);
	
	// Append character buffer.
	void Append(char const* _str, uint32_t const _size);
	
	// Append character.
	void Append(char const _c);
	
	// Append StringView.
	void Append(StringView const& _view);

	// Append formatted string.
	void AppendFmt(const char* _fmt, ...);

	// Clear to empty string.
	void Clear();

	// Number of characters in string (not including null terminator)
	uint32_t Size() const;

	// Is the string empty (Size() == 0)
	bool Empty() const;

	// Capacity, including null character.
	uint32_t Capacity() const;

	// Capacity, without null character.
	uint32_t CapacityWithoutNull() const;

	// Resize and null terminate. Note: contents in interval [currentSize, newSize) are undefined.
	void Resize(uint32_t const _sz);

private:
	static const uint32_t c_capacityNoNull = SizeT - 1;

	char m_data[SizeT];
	uint32_t m_size = 0u;
};

using String128 = StaticString<128>;
using String256 = StaticString<256>;
using String512 = StaticString<512>;
using String1024 = StaticString<1024>;

template<uint32_t T_SizeA, uint32_t T_SizeB>
bool operator==(StaticString<T_SizeA> const& _a, StaticString<T_SizeB> const& _b);

template <uint32_t SizeT>
bool operator==(StaticString<SizeT> const& _a, char const* _cstr);

struct StringView
{
	// Default constructor, null string.
	StringView() = default;

	// Construct from null terminated C string.
	StringView(char const* _str);

	// Construct from char buffer and length.
	StringView(char const* _str, uint32_t const len);

	// Construct from char buffer with begin and end.
	StringView(char const* _begin, char const* _end);

	// Construct from StaticString.
	template <uint32_t SizeT>
	StringView(StaticString<SizeT> const& _str);

	// Size getters.
	uint32_t Size() const;
	bool Empty() const;

	// Underlying char data. Note: not necessarily null terminated.
	char const* Data() const;

	// Begin and end accessors.
	char const* Begin() const;
	char const* End() const;

	// Char index accessor.
	char operator[](uint32_t const _idx) const { KT_ASSERT(_idx < m_size); return m_ptr[_idx]; }

	char const* m_ptr = nullptr;
	uint32_t m_size = 0;
};

inline bool operator==(kt::StringView const& _lhs, kt::StringView const& _rhs);

// String va-args format.
uint32_t StringFormat(char* _buff, uint32_t const _buffSz, char const* _fmt, ...);
uint32_t StringFormatVa(char* _buff, uint32_t const _buffSz, char const* _fmt, va_list va);

// String va-args format from a fixed size buffer.
template <uint32_t SizeT>
uint32_t StringFormat(char(&buff)[SizeT], char const* _fmt, ...);

// String Compare
int32_t StrCmp(char const* _lhs, char const* _rhs);

// String compare up to N chars from both strings.
int32_t StrNCmp(char const* _lhs, char const* _rhs, uint32_t const _max);

// String length.
uint32_t StrLen(char const* _str);

// Search for a character from the start of a string. Returns a string view [_c, end) or empty if not found.
StringView StrFind(StringView const& _view, char const _c);

// Search for a character from the end of a string. Returns a string view [_c, end) or empty if not found.
StringView StrFindR(StringView const& _view, char const _c);

}

#include "inl/Strings.inl"