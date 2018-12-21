#pragma once
#include <type_traits>

namespace kt
{

template <typename T_Key, typename T_Value, typename T_KeyOps>
HashMap<T_Key, T_Value, T_KeyOps>::~HashMap()
{
	Release();
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
HashMap<T_Key, T_Value, T_KeyOps>::HashMap(HashMap&& _other)
	: m_allocator(_other.m_allocator)
	, m_entries(_other.m_entries)
	, m_size(_other.m_size)
	, m_capacity(_other.m_capacity)
{
	_other.m_entries = nullptr;
	_other.m_size = 0;
	_other.m_capacity = 0;
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
void HashMap<T_Key, T_Value, T_KeyOps>::Release()
{
	if (m_entries)
	{
		Clear();
		KT_ASSERT(m_size == 0);
		m_allocator->Free(m_entries);
		m_capacity = 0;
	}
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
void kt::HashMap<T_Key, T_Value, T_KeyOps>::Clear()
{
	if (!m_size)
	{
		return;
	}

	for (uint32_t i = 0; i < m_capacity; ++i)
	{
		if (!m_entries[i].IsEmpty())
		{
			m_entries[i].~InternalEntry();
			m_entries[i].SetEmpty();

			if (--m_size == 0)
			{
				break;
			}
		}
	}
	KT_ASSERT(m_size == 0);
}



template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::operator=(HashMap&& _other) -> HashMap&
{
	 Release();

	 m_allocator = _other.m_allocator;
	 m_entries = _other.m_entries;
	 m_size = _other.m_size;
	 m_capacity = _other.m_capacity;

	 _other.m_entries = nullptr;
	 _other.m_size = 0;
	 _other.m_capacity = 0;

	 return *this;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto kt::HashMap<T_Key, T_Value, T_KeyOps>::MakeCopy() const -> HashMapType
{
	return MakeCopy(m_allocator);
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto kt::HashMap<T_Key, T_Value, T_KeyOps>::MakeCopy(IAllocator* _newAllocator) const -> HashMapType
{
	HashMap newMap(_newAllocator);

	if (m_size)
	{
		newMap.m_entries = (InternalEntry*)_newAllocator->Alloc(sizeof(InternalEntry) * m_capacity, KT_ALIGNOF(InternalEntry));

		uint32_t numLeft = m_size;
		newMap.m_size = m_size;
		newMap.m_capacity = m_capacity;

		for (uint32_t i = 0; i < m_capacity; ++i)
		{
			if (!m_entries[i].IsEmpty())
			{
				PlacementNew(&newMap.m_entries[i], m_entries[i]);

				if (--numLeft == 0)
				{
					break;
				}
			}
			else
			{
				m_entries[i].SetEmpty();
			}
		}
	}

	return newMap;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
void HashMap<T_Key, T_Value, T_KeyOps>::GrowCapacityAndRehash(uint32_t const _newCap)
{
	uint32_t const newCapacity = _newCap ? _newCap : Max(c_defaultCapacity, m_capacity * 2u);
	KT_ASSERT(IsPow2(newCapacity));

	InternalEntry* newEntries = (InternalEntry*)m_allocator->Alloc(sizeof(InternalEntry) * newCapacity, KT_ALIGNOF(InternalEntry));
	
	for (uint32_t i = 0; i < newCapacity; ++i)
	{
		newEntries[i].SetEmpty();
	}

	InternalEntry* oldEntries = m_entries;

	uint32_t const oldCapacity = m_capacity;
	
	uint32_t numLeft = m_size;

	m_entries = newEntries;
	m_capacity = newCapacity;
	m_size = 0;

	for (uint32_t i = 0; i < oldCapacity; ++i)
	{
		if (!oldEntries[i].IsEmpty())
		{
			InsertInternal(HashKey(oldEntries[i].m_kvPair.m_key), oldEntries[i].m_kvPair.m_key, oldEntries[i].m_kvPair.m_val);
			oldEntries[i].~InternalEntry();

			if (--numLeft == 0)
			{
				break;
			}
		}
	}

	m_allocator->Free(oldEntries);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Begin() -> Iterator
{
	return Iterator(this, FirstFreeIndex());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::End() -> Iterator
{
	return Iterator(this, c_invalidIndex);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Begin() const -> ConstIterator
{
	return ConstIterator(this, FirstFreeIndex());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::End() const -> ConstIterator
{
	return ConstIterator(this, c_invalidIndex);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Find(KeyType const & _key) -> Iterator
{
	return Iterator(this, FindIndex(_key));
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Find(KeyType const & _key) const -> ConstIterator
{
	return ConstIterator(this, FindIndex(_key));
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Erase(Iterator const& _it) -> Iterator
{
	return Iterator(this, RemoveIndex(_it.m_idx));
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::operator[](KeyType const & _key) -> ValueType&
{
	uint32_t idx = FindIndex(_key);

	if (idx == c_invalidIndex)
	{
		ValueType defaultV = {};
		KeyType keyCopy = _key;
		idx = InsertInternal(HashKey(_key), keyCopy, defaultV);
	}
	
	return m_entries[idx].m_kvPair.m_val;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Insert(KeyType const& _key, ValueType const& _value) -> Iterator
{
	HashType const hash = HashKey(_key);
	IndexType const foundIdx = FindIndex(_key, hash);
	if (foundIdx != c_invalidIndex)
	{
		m_entries[foundIdx].m_kvPair.m_val = _value;
		return Iterator(this, foundIdx);
	}

	KeyType key = _key;
	ValueType val = _value;

	IndexType const idx = InsertInternal(hash, key, val);
	return Iterator(this, idx);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::InsertIfMissing(KeyType const& _key, ValueType const& _default, bool* _didNewInsert) -> Iterator
{
	HashType const hash = HashKey(_key);
	IndexType const foundIdx = FindIndex(_key, hash);
	if (foundIdx != c_invalidIndex)
	{
		if (_didNewInsert)
		{
			*_didNewInsert = false;
		}

		return Iterator(this, foundIdx);
	}

	if (_didNewInsert)
	{
		*_didNewInsert = true;
	}

	KeyType key = _key;
	ValueType val = _default;

	// Todo: could optimise find and insert here..
	IndexType const idx = InsertInternal(hash, key, val);
	return Iterator(this, idx);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
inline uint32_t HashMap<T_Key, T_Value, T_KeyOps>::Size() const
{
	return m_size;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
inline uint32_t HashMap<T_Key, T_Value, T_KeyOps>::Capacity() const
{
	return m_capacity;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
void HashMap<T_Key, T_Value, T_KeyOps>::Reserve(uint32_t const _sz)
{
	uint32_t const idealCap = (uint32_t)(_sz / c_maxLoadFactor);

	if (m_capacity < idealCap)
	{
		GrowCapacityAndRehash(idealCap ? NextPow2(idealCap - 1) : 0);
	}
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
float HashMap<T_Key, T_Value, T_KeyOps>::GetLoadFactor() const
{
	return m_size / (float)m_capacity;
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::GetNextIndex(IndexType const _idx) const -> IndexType
{
	for (uint32_t i = _idx + 1; i < m_capacity; ++i)
	{
		if (!m_entries[i].IsEmpty())
		{
			return i;
		}
	}
	return c_invalidIndex;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::FirstFreeIndex() const -> IndexType
{
	for (uint32_t idx = 0; idx < m_capacity; ++idx)
	{
		if (!m_entries[idx].IsEmpty())
		{
			return idx;
		}
	}
	return c_invalidIndex;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::InsertInternal(HashType const _hash, T_Key& _key, T_Value& _val) -> IndexType
{
	if (!m_entries || GetLoadFactor() >= c_maxLoadFactor)
	{
		GrowCapacityAndRehash();
	}

	IndexType pos = DesiredHashPos(_hash);
	uint8_t dist = 0;

	uint32_t retIdx = c_invalidIndex;

	for (;;)
	{
		if (m_entries[pos].IsEmpty())
		{
			PlacementNew(&m_entries[pos].m_kvPair.m_key, _key);
			PlacementNew(&m_entries[pos].m_kvPair.m_val, _val);
			m_entries[pos].SetProbeDist(dist);
			++m_size;
			return retIdx == c_invalidIndex ? pos : retIdx;;
		}

		uint8_t const thisSlotProbeDist = m_entries[pos].GetProbeDist();

		if (thisSlotProbeDist < dist)
		{
			Swap(_key, (T_Key&)m_entries[pos].m_kvPair.m_key);
			Swap(_val, (T_Value&)m_entries[pos].m_kvPair.m_val);
			m_entries[pos].SetProbeDist(dist);
			dist = thisSlotProbeDist;

			if (retIdx == c_invalidIndex)
			{
				retIdx = pos;
			}
		}

		pos = (pos + 1u) & (m_capacity - 1u);
		++dist;
		if (dist == InternalEntry::s_maxProbeDist)
		{
			GrowCapacityAndRehash(m_capacity * 2u);
			return InsertInternal(_hash, _key, _val);
		}
	}
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
uint32_t kt::HashMap<T_Key, T_Value, T_KeyOps>::CalcProbeDistance(HashType const _hash, IndexType const _idx)
{
	KT_ASSERT(IsIndexValid(_idx));

	uint32_t const desired = DesiredHashPos(_hash);

	return (_idx + m_capacity - desired) & (m_capacity - 1u);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::FindIndex(T_Key const& _key, HashType const _hash) -> IndexType
{
	if (!m_size)
	{
		return c_invalidIndex;
	}

	IndexType pos = DesiredHashPos(_hash);

	int8_t dist = 0;

	for (;;)
	{
		if (dist > m_entries[pos].GetProbeDist())
		{
			return c_invalidIndex;
		}
		if (KeyOps::Equal(m_entries[pos].m_kvPair.m_key, _key))
		{
			return pos;
		}

		++dist;

		pos = (pos + 1) & (m_capacity - 1);
	}
}


template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::FindIndex(T_Key const& _key) -> IndexType
{
	if (!m_size)
	{
		return c_invalidIndex;
	}

	return FindIndex(_key, HashKey(_key));
}


template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::HashKey(T_Key const& _key) -> HashType
{
	return KeyOps::Hash(_key);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::DesiredHashPos(HashType const _hash) -> IndexType
{
	return (IndexType)(_hash & (m_capacity - 1u));
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
bool HashMap<T_Key, T_Value, T_KeyOps>::IsIndexValid(IndexType const _idx) const
{
	return _idx < m_capacity && !m_entries[_idx].IsEmpty();
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::RemoveIndex(IndexType const _idx) -> IndexType
{
	KT_ASSERT(IsIndexValid(_idx));

	IndexType pos = _idx;

	for (;;)
	{
		IndexType const nextIdx = (pos + 1u) & (m_capacity - 1u);

		if (m_entries[nextIdx].IsEmpty() || m_entries[nextIdx].GetProbeDist() == 0)
		{
			break;
		}

		m_entries[pos] = std::move(m_entries[nextIdx]);
		m_entries[pos].SetProbeDist(m_entries[pos].GetProbeDist() - 1);
		pos = nextIdx;
	}

	m_entries[pos].SetEmpty();
	m_entries[pos].~InternalEntry();
	--m_size;

	if (m_entries[_idx].IsEmpty())
	{
		return GetNextIndex(_idx);
	}
	else
	{
		return _idx;
	}
}


template <typename T_Key, typename T_Value, typename T_KeyOps /*= HashMap_KeyOps<T_Key>*/>
void HashMap<T_Key, T_Value, T_KeyOps>::SetAllocator(IAllocator* _allocator)
{
	if (m_allocator != _allocator)
	{
		if (!m_size)
		{
			Release();
			m_allocator = _allocator;
		}
		else
		{
			*this = std::move(MakeCopy(_allocator));
		}
	}
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Iterator::operator*() const -> KvPair&
{
	KT_ASSERT(this->m_map);
	KT_ASSERT(this->m_map->IsIndexValid(this->m_idx));
	return this->m_map->m_entries[this->m_idx].m_kvPair;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::ConstIterator::operator->() const -> KvPair const *
{
	return &(this->operator*());
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Iterator::operator->() const -> HashMap::KvPair *
{
	return &(this->operator*());
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::ConstIterator::operator*() const -> KvPair const&
{
	KT_ASSERT(this->m_map);
	KT_ASSERT(this->m_map->IsIndexValid(this->m_idx));
	return this->m_map->m_entries[this->m_idx].m_kvPair;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::IteratorBase::operator++(int) -> IteratorBase
{
	KT_ASSERT(this->m_map);
	KT_ASSERT(this->m_map->IsIndexValid(this->m_idx));
	IteratorBase it = IteratorBase(this->m_map, this->m_idx);
	m_idx = this->m_map->GetNextIndex(this->m_idx);
	return it;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::IteratorBase::operator++() -> IteratorBase&
{
	KT_ASSERT(this->m_map);
	KT_ASSERT(this->m_map->IsIndexValid(this->m_idx));
	this->m_idx = this->m_map->GetNextIndex(this->m_idx);
	return *this;
}

}