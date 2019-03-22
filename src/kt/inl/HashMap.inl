#pragma once
#include "../HashMap.h"

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
	, m_data(_other.m_data)
{
	_other.m_data.m_ptr = nullptr;
	_other.m_data.m_size = 0;
	_other.m_data.m_capacity = 0;
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
void HashMap<T_Key, T_Value, T_KeyOps>::Release()
{
	if (m_data.m_ptr)
	{
		Clear();
		KT_ASSERT(m_data.m_size == 0);
		FreeMapData(m_data, m_allocator);
	}
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
void kt::HashMap<T_Key, T_Value, T_KeyOps>::Clear()
{
	if (!m_data.m_size)
	{
		return;
	}

	HashType* hashes = m_data.HashPtr();
	KvPair* kvps = m_data.KvPairPtr();

	for (uint32_t i = 0; i < m_data.m_capacity; ++i)
	{
		if (hashes[i] != 0)
		{
			kvps[i].~KvPair();
			hashes[i] = 0;

			if (--m_data.m_size == 0)
			{
				break;
			}
		}
	}
	KT_ASSERT(m_data.m_size == 0);
}



template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::operator=(HashMap&& _other) -> HashMap&
{
	 Release();

	 m_allocator = _other.m_allocator;
	 m_data = _other.m_data;

	 _other.m_data.m_ptr = nullptr;
	 _other.m_data.m_size = 0;
	 _other.m_data.m_capacity = 0;

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

	HashType* myHashes = m_data.HashPtr();
	KvPair* myKvp = m_data.KvPairPtr();

	if (m_data.m_size)
	{
		newMap.m_data = AllocMapData(_newAllocator, m_data.m_capacity);
		KvPair* theirKvp = newMap.m_data.KvPairPtr();

		memcpy(newMap.m_data.HashPtr(), myHashes, sizeof(HashType) * m_data.m_capacity);

		uint32_t numLeft = m_data.m_size;
		newMap.m_data.m_size = m_data.m_size;
		newMap.m_data.m_capacity = m_data.m_capacity;

		for (uint32_t i = 0; i < m_data.m_capacity; ++i)
		{
			if (myHashes[i] != 0)
			{
				PlacementNew(&theirKvp[i], myKvp[i]);

				if (--numLeft == 0)
				{
					break;
				}
			}
		}
	}

	return newMap;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
KT_NO_INLINE void HashMap<T_Key, T_Value, T_KeyOps>::GrowCapacityAndRehash(uint32_t const _newCap)
{
	uint32_t const newCapacity = _newCap ? _newCap : Max(c_defaultCapacity, m_data.m_capacity * 2u);
	KT_ASSERT(IsPow2(newCapacity));

	MapData oldMapData = m_data;

	MapData newMapData = AllocMapData(m_allocator, newCapacity);
	newMapData.m_size = 0;
	m_data = newMapData;

	memset(newMapData.HashPtr(), 0, sizeof(HashType) * newCapacity);

	HashType* oldHashes = oldMapData.HashPtr();
	KvPair* oldKvp = oldMapData.KvPairPtr();

	uint32_t const oldCapacity = oldMapData.m_capacity;
	
	uint32_t numLeft = oldMapData.m_size;

	for (uint32_t i = 0; i < oldCapacity; ++i)
	{
		if (oldHashes[i] != 0)
		{
			InsertInternal(oldHashes[i], oldKvp[i].m_key, oldKvp[i].m_val);
			oldKvp[i].~KvPair();

			if (--numLeft == 0)
			{
				break;
			}
		}
	}

	FreeMapData(oldMapData, m_allocator);
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
	ValueType defaultV = {};
	KeyType keyCopy = _key;
	IndexType const idx = InsertInternal(HashKey(_key), keyCopy, defaultV, false);
	
	return m_data.KvPairPtr()[idx].m_val;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Insert(KeyType const& _key, ValueType const& _value) -> Iterator
{
	HashType const hash = HashKey(_key);

	KeyType key = _key;
	ValueType val = _value;

	IndexType const idx = InsertInternal(hash, key, val);
	return Iterator(this, idx);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
inline uint32_t HashMap<T_Key, T_Value, T_KeyOps>::Size() const
{
	return m_data.m_size;
}


template <typename T_Key, typename T_Value, typename T_KeyOps /*= HashMap_KeyOps<T_Key>*/>
bool HashMap<T_Key, T_Value, T_KeyOps>::Empty() const
{
	return m_data.m_size == 0;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
inline uint32_t HashMap<T_Key, T_Value, T_KeyOps>::Capacity() const
{
	return m_data.m_capacity;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
void HashMap<T_Key, T_Value, T_KeyOps>::Reserve(uint32_t const _sz)
{
	uint32_t const idealCap = (uint32_t)(_sz / c_maxLoadFactor);

	if (m_data.m_capacity < idealCap)
	{
		GrowCapacityAndRehash(idealCap ? NextPow2(idealCap - 1) : 0);
	}
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
float HashMap<T_Key, T_Value, T_KeyOps>::GetLoadFactor() const
{
	return m_data.m_capacity != 0 ? m_data.m_size / (float)m_data.m_capacity : 0.0f;
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::GetNextIndex(IndexType const _idx) const -> IndexType
{
	HashType* hashes = m_data.HashPtr();
	for (uint32_t idx = _idx + 1; idx < m_data.m_capacity; ++idx)
	{
		if (hashes[idx] != 0)
		{
			return idx;
		}
	}
	return c_invalidIndex;
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::FirstFreeIndex() const -> IndexType
{
	HashType* hashes = m_data.HashPtr();
	for (uint32_t idx = 0; idx < m_data.m_capacity; ++idx)
	{
		if (hashes[idx] != 0)
		{
			return idx;
		}
	}
	return c_invalidIndex;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::InsertInternal(HashType const _hash, T_Key& _key, T_Value& _val, bool _setValIfAlreadyInMap) -> IndexType
{
	if (!m_data.m_ptr)
	{
		GrowCapacityAndRehash();
	}

	IndexType pos = DesiredHashPos(_hash);

	HashType* hashes = m_data.HashPtr();
	KvPair* kvps = m_data.KvPairPtr();

	uint32_t distFromIdeal = CalcProbeDistance(_hash, pos);

	while (distFromIdeal <= CalcProbeDistance(hashes[pos], pos))
	{
		if (!hashes[pos])
		{
			PlacementNew(&kvps[pos].m_key, std::move(_key));
			PlacementNew(&kvps[pos].m_val, std::move(_val));
			hashes[pos] = _hash;
			++m_data.m_size;
			return pos;
		}

		if (_hash == hashes[pos] && KeyOps::Equal(kvps[pos].m_key, _key))
		{
			if (_setValIfAlreadyInMap)
			{
				kvps[pos].m_val = _val;
			}
			return pos;
		}

		pos = (pos + 1u) & (m_data.m_capacity - 1u);
		++distFromIdeal;
	}

	// out of fast path, need to robin hood.
	if (GetLoadFactor() >= c_maxLoadFactor)
	{
		GrowCapacityAndRehash();
		pos = DesiredHashPos(_hash);
		distFromIdeal = 0;

		hashes = m_data.HashPtr();
		kvps = m_data.KvPairPtr();

		while (distFromIdeal <= CalcProbeDistance(_hash, pos))
		{
			pos = (pos + 1u) & (m_data.m_capacity - 1u);
			++distFromIdeal;
		}
	}

	HashType curHash = _hash;
	uint32_t retIdx = c_invalidIndex;

	for (;;)
	{
		if (!hashes[pos])
		{
			PlacementNew(&kvps[pos].m_key, std::move(_key));
			PlacementNew(&kvps[pos].m_val, std::move(_val));
			hashes[pos] = curHash;
			++m_data.m_size;
			return retIdx == c_invalidIndex ? pos : retIdx;
		}

		uint32_t const thisSlotProbeDist = CalcProbeDistance(hashes[pos], pos);

		if (thisSlotProbeDist < distFromIdeal)
		{
			Swap(_key, kvps[pos].m_key);
			Swap(_val, kvps[pos].m_val);
			Swap(curHash, hashes[pos]);
			distFromIdeal = thisSlotProbeDist;

			if (retIdx == c_invalidIndex)
			{
				retIdx = pos;
			}
		}

		pos = (pos + 1u) & (m_data.m_capacity - 1u);
		++distFromIdeal;
	}
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
uint32_t kt::HashMap<T_Key, T_Value, T_KeyOps>::CalcProbeDistance(HashType const _hash, IndexType const _idx)
{
	if (!_hash) { return UINT32_MAX; }

	uint32_t const desired = DesiredHashPos(_hash);

	return (_idx + m_data.m_capacity - desired) & (m_data.m_capacity - 1u);
}

template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::FindIndex(T_Key const& _key, HashType const _hash) -> IndexType
{
	if (!m_data.m_size)
	{
		return c_invalidIndex;
	}

	IndexType pos = DesiredHashPos(_hash);

	HashType* hashes = m_data.HashPtr();
	KvPair* kvps = m_data.KvPairPtr();

	uint32_t dist = 0;

	for (;;)
	{
		if (!hashes[pos] || dist > CalcProbeDistance(hashes[pos], pos))
		{
			return c_invalidIndex;
		}
		if (_hash == hashes[pos] && KeyOps::Equal(kvps[pos].m_key, _key))
		{
			return pos;
		}

		++dist;

		pos = (pos + 1) & (m_data.m_capacity - 1);
	}
}


template<typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::FindIndex(T_Key const& _key) -> IndexType
{
	if (!m_data.m_size)
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
	return (IndexType)(_hash & (m_data.m_capacity - 1u));
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
bool HashMap<T_Key, T_Value, T_KeyOps>::IsIndexValid(IndexType const _idx) const
{
	return _idx < m_data.m_capacity && m_data.HashPtr()[_idx] != 0;
}

template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::RemoveIndex(IndexType const _idx) -> IndexType
{
	KT_ASSERT(IsIndexValid(_idx));

	IndexType pos = _idx;

	HashType* hashes = m_data.HashPtr();
	KvPair* kvp = m_data.KvPairPtr();

	for (;;)
	{
		IndexType const nextIdx = (pos + 1u) & (m_data.m_capacity - 1u);

		if (!hashes[nextIdx] || CalcProbeDistance(hashes[nextIdx], nextIdx) == 0)
		{
			break;
		}

		kvp[pos] = std::move(kvp[nextIdx]);
		hashes[pos] = hashes[nextIdx];
		pos = nextIdx;
	}

	hashes[pos] = 0;
	kvp[pos].~KvPair();
	--m_data.m_size;

	if (!hashes[_idx])
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
		if (!m_data.m_size)
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


template <typename T_Key, typename T_Value, typename T_KeyOps /*= HashMap_KeyOps<T_Key>*/>
size_t kt::HashMap<T_Key, T_Value, T_KeyOps>::AllocSizeForCapacity(uint32_t _capacity)
{
	size_t const hashSz = kt::AlignUp(sizeof(HashType) * _capacity, KT_ALIGNOF(KvPair));
	size_t const kvPSize = sizeof(KvPair) * _capacity;

	return hashSz + kvPSize;
}

template <typename T_Key, typename T_Value, typename T_KeyOps /*= HashMap_KeyOps<T_Key>*/>
auto kt::HashMap<T_Key, T_Value, T_KeyOps>::AllocMapData(IAllocator* _allocator, uint32_t _capacity) -> MapData
{
	KT_ASSERT(IsPow2(_capacity));

	MapData map;
	map.m_ptr = _allocator->Alloc(AllocSizeForCapacity(_capacity), kt::Max(KT_ALIGNOF(uint32_t), KT_ALIGNOF(KvPair)));
	map.m_size = 0;
	map.m_capacity = _capacity;
	return map;
}

template <typename T_Key, typename T_Value, typename T_KeyOps /*= HashMap_KeyOps<T_Key>*/>
void HashMap<T_Key, T_Value, T_KeyOps>::FreeMapData(MapData& _data, IAllocator* _allocator)
{
	_allocator->FreeSized(_data.m_ptr, AllocSizeForCapacity(_data.m_capacity));
	_data.m_capacity = 0;
	_data.m_size = 0;
	_data.m_ptr = nullptr;
}


template <typename T_Key, typename T_Value, typename T_KeyOps /*= HashMap_KeyOps<T_Key>*/>
auto HashMap<T_Key, T_Value, T_KeyOps>::MapData::HashPtr() const -> HashType*
{
	return (HashType*)m_ptr;
}


template <typename T_Key, typename T_Value, typename T_KeyOps /*= HashMap_KeyOps<T_Key>*/>
auto HashMap<T_Key, T_Value, T_KeyOps>::MapData::KvPairPtr() const -> KvPair*
{
	size_t const hashSz = kt::AlignUp(sizeof(HashType) * m_capacity, KT_ALIGNOF(KvPair));
	return (KvPair*)((uintptr_t)m_ptr + hashSz);
}


template <typename T_Key, typename T_Value, typename T_KeyOps>
auto HashMap<T_Key, T_Value, T_KeyOps>::Iterator::operator*() const -> KvPair&
{
	KT_ASSERT(this->m_map);
	KT_ASSERT(this->m_map->IsIndexValid(this->m_idx));
	return this->m_map->m_data.KvPairPtr()[this->m_idx];
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
	return this->m_map->m_data.KvPairPtr()[this->m_idx];
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