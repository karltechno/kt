#pragma once
#include <stdint.h>
#include "kt.h"
#include "Strings.h"
#include "Hash.h"

namespace kt
{

template <typename T_Key, typename T_Value, typename T_KeyOps>
class HashMap;
	
template <typename T>
struct HashMap_KeyOps
{
	using HashType = uint32_t;
	static_assert(KT_IS_POD(T), "HashMap_KeyOps should be specialized for non-POD types.");

	static bool Equal(T const& _lhs, T const& _rhs)
	{
		return _lhs == _rhs;
	}

	static HashType Hash(T const& _key)
	{
		return XXHash_32(&_key, sizeof(T));
	}
};

template <>
struct HashMap_KeyOps<char const*>
{
	using HashType = uint32_t;

	static bool Equal(char const* _lhs, char const* _rhs)
	{
		return StrCmp(_lhs, _rhs) == 0;
	}

	static HashType Hash(char const* _key)
	{
		return XXHash_32(_key, StrLen(_key));
	}
};

template <>
struct HashMap_KeyOps<StringView>
{
	using HashType = uint32_t;

	static bool Equal(StringView const& _lhs, StringView const& _rhs)
	{
		return _lhs == _rhs;
	}

	static HashType Hash(StringView const& _view)
	{
		return XXHash_32(_view.Data(), _view.Size());
	}
};

template <uint32_t T_Size>
struct HashMap_KeyOps<StaticString<T_Size>>
{
	using HashType = uint32_t;

	static bool Equal(StaticString<T_Size> const& _lhs, StaticString<T_Size> const& _rhs)
	{
		return _lhs == _rhs;
	}

	static HashType Hash(StaticString<T_Size> const& _str)
	{
		return XXHash_32(_str.Data(), _str.Size());
	}
};


template <typename T>
struct HashMap_KeyOps_StringI
{
};

template <>
struct HashMap_KeyOps_StringI<char const*>
{
	using HashType = uint32_t;

	static bool Equal(char const* _lhs, char const* _rhs)
	{
		return StrCmpI(_lhs, _rhs) == 0;
	}

	static HashType Hash(char const* _key)
	{
		return StringHashI(_key);
	}
};

template <>
struct HashMap_KeyOps_StringI<StringView>
{
	using HashType = uint32_t;

	static bool Equal(StringView const& _lhs, StringView const& _rhs)
	{
		return StrCmpI(_lhs, _rhs) == 0;
	}

	static HashType Hash(StringView const& _view)
	{
		return StringHashI(_view);
	}
};

template <uint32_t T_Size>
struct HashMap_KeyOps_StringI<StaticString<T_Size>>
{
	using HashType = uint32_t;

	static bool Equal(StaticString<T_Size> const& _lhs, StaticString<T_Size> const& _rhs)
	{
		return StrCmpI(StringView(_lhs), StringView(_rhs)) == 0;
	}

	static HashType Hash(StaticString<T_Size> const& _str)
	{
		return StringHashI(StringView(_str));
	}
};

template <typename T>
struct HashMap_KeyOps_PassthroughInt
{
	using HashType = T;

	static_assert(std::is_integral<T>::value, "Must be integral type.");

	static bool Equal(T _lhs, T _rhs)
	{
		return _lhs == _rhs;
	}

	static HashType Hash(T _val)
	{
		return _val;
	}
};

template <typename T_Key, typename T_Value>
struct IntegerKeyMap : HashMap<T_Key, T_Value, HashMap_KeyOps_PassthroughInt<T_Key>> {};

}