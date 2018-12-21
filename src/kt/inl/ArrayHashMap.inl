#pragma once
#include "../ArrayHashMap.h"
#include "..\ArrayHashMap.h"

namespace kt
{

template<typename T_Key, typename T_Value, typename T_KeyOps>
ArrayHashMap<T_Key, T_Value, T_KeyOps>::ArrayHashMap(ArrayHashMap&& _other)
	: m_map(std::move(_other.m_map))
	, m_array(std::move(_other.m_array))
{
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::operator=(ArrayHashMap&& _other) -> ArrayHashMap&
{
	m_map = std::move(_other.m_map);
	m_array = std::move(_other.m_array);
	return *this;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::Insert(KeyType const& _key, ValueType const& _value) -> ValueIterator
{
	bool newKey = false;
	typename HashMapType::Iterator it = m_map.InsertIfMissing(_key, 0, &newKey);
	if (newKey)
	{
		m_array.PushBack(_value);
		it->m_val = m_array.Size() - 1;
		return m_array.Begin() + it->m_val;
	}
	else
	{
		KT_ASSERT(it->m_val < m_array.Size());
		m_array[it->m_val] = _value;
		return m_array.Begin() + it->m_val;
	}
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::BeginValues() -> ValueIterator
{
	return m_array.Begin();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::BeginValues() const -> ValueConstIterator
{
	return m_array.Begin();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::EndValues() -> ValueIterator
{
	return m_array.End();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::EndValues() const -> ValueConstIterator
{
	return m_array.End();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::BeginKeys() -> KeyIterator
{
	return KeyIterator(m_map.Begin());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::BeginKeys() const -> KeyConstIterator
{
	return KeyConstIterator(m_map.Begin());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::EndKeys() -> KeyIterator
{
	return KeyIterator(m_map.End());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::EndKeys() const -> KeyConstIterator
{
	return KeyConstIterator(m_map.End());
}


template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::Find(KeyType const & _key) -> ValueIterator
{
	return KeyToValueIt(FindKeyIt(_key));
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::Find(KeyType const & _key) const -> ValueConstIterator
{
	return KeyToValueIt(FindKeyIt(_key));
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::FindKeyIt(KeyType const & _key) -> KeyIterator
{
	typename HashMapType::ConstIterator keyIt = m_map.Find(_key);
	if (keyIt != m_map.End())
	{
		KT_ASSERT(keyIt->m_val < m_array.Size());
		return KeyIterator(keyIt);
	}

	return KeyIterator(m_map.End());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::FindKeyIt(KeyType const & _key) const -> KeyConstIterator
{
	typename HashMapType::ConstIterator keyIt = m_map.Find(_key);
	if (keyIt != KeyConstIterator(m_map.End()))
	{
		KT_ASSERT(keyIt->m_val < m_array.Size());
		return KeyIterator(keyIt);
	}

	return KeyIterator(m_map.End());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::KeyToValueIt(KeyIterator const & _keyIt) -> ValueIterator
{
	if (_keyIt == KeyIterator(m_map.End()))
	{
		return m_array.End();
	}

	KT_ASSERT(_keyIt.InternalGetIndex() < m_array.Size());
	return m_array.Begin() + _keyIt.InternalGetIndex();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::KeyToValueIt(KeyConstIterator const & _keyIt) const -> ValueConstIterator
{
	if (_keyIt == m_map.End())
	{
		return m_array.End();
	}

	KT_ASSERT(_keyIt.m_val < m_array.Size());
	return m_array.Begin() + _keyIt.m_val;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto kt::ArrayHashMap<T_Key, T_Value, T_KeyOps>::Erase(KeyIterator _keyIt) -> KeyIterator
{
	KT_ASSERT(_keyIt->m_val < m_array.Size());
	ArrayIndexType const arrayIdx = _keyIt->m_val;
	// find entry with highest current index
	uint32_t const lastIndexToFind = m_array.Size() - 1;

	if (arrayIdx == lastIndexToFind)
	{
		// Fast path deleting last element.
		m_array.PopBack();
		return m_map.Erase(_keyIt);
	}

	HashMapType::Iterator swapToIt = m_map.End();

	for (typename HashMapType::Iterator it = m_map.Begin(); it != m_map.End(); ++it)
	{
		if (it->m_val == lastIndexToFind)
		{
			swapToIt = it;
			break;
		}
	}

	KT_ASSERT(swapToIt != m_map.End());
	KT_ASSERT(swapToIt->m_val < m_array.Size());
	Swap(m_array[swapToIt->m_val], m_array[_keyIt->m_val]);
	swapToIt->m_val = _keyIt->m_val;
	m_array.PopBack();
	return m_map.Erase(_keyIt);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::operator[](KeyType const& _key) -> ValueType&
{
	bool newKey = false;
	typename HashMapType::Iterator it = m_map.InsertIfMissing(_key, 0, &newKey);
	if (newKey)
	{
		m_array.PushBack();
		it->m_val = m_array.Size() - 1;
		return m_array.Begin()[it->m_val];
	}
	else
	{
		KT_ASSERT(it->m_val < m_array.Size());
		return m_array.Begin()[it->m_val];
	}
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
uint32_t ArrayHashMap<T_Key, T_Value, T_KeyOps>::Size() const
{
	KT_ASSERT(m_map.Size() == m_array.Size());
	return m_array.Size();
}


template<typename T_Key, typename T_Value, typename T_KeyOps>
void ArrayHashMap<T_Key, T_Value, T_KeyOps>::Reserve(uint32_t const _sz)
{
	m_array.Reserve(_sz);
	m_map.Reserve(_sz);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
void ArrayHashMap<T_Key, T_Value, T_KeyOps>::Clear()
{
	m_array.Clear();
	m_map.Clear();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::MakeCopy(IAllocator * _newAllocator) const -> ArrayMapType
{
	ArrayMapType ret;
	ret.m_array = std::move(m_array.MakeCopy(_newAllocator));
	ret.m_map = std::move(m_map.MakeCopy(_newAllocator));
	return ret;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto ArrayHashMap<T_Key, T_Value, T_KeyOps>::MakeCopy() const -> ArrayMapType
{
	return MakeCopy(m_array.GetAllocator());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
float ArrayHashMap<T_Key, T_Value, T_KeyOps>::GetLoadFactor() const
{
	return m_map.GetLoadFactor();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
void ArrayHashMap<T_Key, T_Value, T_KeyOps>::SetAllocator(IAllocator * _allocator)
{
	m_array.SetAllocator(_allocator);
	m_map.SetAllocator(_allocator);
}

}