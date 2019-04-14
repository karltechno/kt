#pragma once
#include "../Handles.h"
#include "../Memory.h"

namespace kt
{

template <typename DataT>
void VersionedHandlePool<DataT>::Init(IAllocator* _alloc, uint32_t _initialCapacity)
{
	m_allocator = _alloc;
	Grow(_initialCapacity);
}

template <typename DataT>
VersionedHandlePool<DataT>::~VersionedHandlePool()
{
	KT_ASSERT(!m_numAllocated);

	for (uint32_t i = 0; i < m_capacity; ++i)
	{
		m_entries[i].~Entry();
	}

	if (m_allocator && m_entries)
	{
		m_allocator->FreeSized(m_entries, sizeof(Entry) * m_capacity);
	}
}

template <typename DataT>
void VersionedHandlePool<DataT>::Grow(uint32_t _sizeHint)
{
	uint32_t const newCap = kt::Min(_sizeHint, HandleType::c_maxIndex);
	KT_ASSERT(newCap);
	KT_ASSERT(newCap > m_capacity);
	KT_ASSERT(m_allocator);
	KT_ASSERT(m_numAllocated == m_capacity);

	m_entries = (Entry*)m_allocator->ReAllocSized(m_entries, m_capacity * sizeof(Entry), newCap * sizeof(Entry), KT_ALIGNOF(Entry));

	for (uint32_t i = m_capacity; i < newCap; ++i)
	{
		kt::PlacementNew<Entry>(&m_entries[i]);
		m_entries[i].m_next = i + 1;
		m_entries[i].m_inUse = 0;
		m_entries[i].m_version = 0;
	}

	m_freeListHead = m_capacity;
	m_capacity = newCap;
}

template <typename DataT>
auto VersionedHandlePool<DataT>::Alloc(DataT*& o_data) -> HandleType
{
	if (m_capacity == m_numAllocated)
	{
		Grow(m_capacity / 2 + m_capacity);
	}

	KT_ASSERT(m_capacity != m_numAllocated);

	uint32_t const newIdx = m_freeListHead;
	++m_numAllocated;

	Entry& newEntry = m_entries[newIdx];
	KT_ASSERT(!newEntry.m_inUse);
	m_freeListHead = newEntry.m_next;
	newEntry.m_inUse = 1;
	o_data = &newEntry.m_data;
	return HandleType{ uint16_t(newIdx), newEntry.m_version };
}

template <typename DataT>
bool VersionedHandlePool<DataT>::IsValid(HandleType _handle) const
{
	return _handle.m_idx < m_capacity
		&& m_entries[_handle.m_idx].m_inUse
		&& m_entries[_handle.m_idx].m_version == _handle.m_ver;
}

template <typename DataT>
DataT* VersionedHandlePool<DataT>::Lookup(HandleType _handle)
{
	if (_handle.m_idx >= m_capacity)
	{
		return nullptr;
	}

	Entry& entry = m_entries[_handle.m_idx];
	if (!entry.m_inUse || entry.m_version != _handle.m_ver)
	{
		return nullptr;
	}

	return &entry.m_data;
}

template <typename DataT>
void VersionedHandlePool<DataT>::Free(HandleType _handle)
{
	KT_ASSERT(m_numAllocated);
	KT_ASSERT(IsValid(_handle));
	--m_numAllocated;
	Entry& entry = m_entries[_handle.m_idx];
	entry.m_inUse = 0;
	entry.m_next = m_freeListHead;
	m_freeListHead = _handle.m_idx;
}

template <typename DataT>
auto VersionedHandlePool<DataT>::NextAllocatedHandle(HandleType _handle) const -> HandleType
{
	KT_ASSERT(IsValid(_handle));
	for (uint32_t i = _handle.m_idx + 1; i < m_capacity; ++i)
	{
		if (m_entries[i].m_inUse)
		{
			return HandleType{ uint16_t(i), m_entries[i].m_version };
		}
	}

	return HandleType{};
}

template <typename DataT>
auto kt::VersionedHandlePool<DataT>::FirstAllocatedHandle() const -> HandleType
{
	if (!m_numAllocated)
	{
		return HandleType{};
	}

	for (uint32_t i = 0; i < m_capacity; ++i)
	{
		if (m_entries[i].m_inUse)
		{
			return HandleType{ uint16_t(i), m_entries[i].m_version };
		}
	}

	KT_UNREACHABLE;
}


}