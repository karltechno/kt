#pragma once
#include "kt.h"

namespace kt
{
struct IAllocator;

struct VersionedHandle
{
	static uint32_t constexpr c_invalidBitPattern = 0xFFFFFFFF;
	static uint32_t constexpr c_maxIndex = UINT16_MAX;

	VersionedHandle()
	{
		m_packed = c_invalidBitPattern;
	}

	VersionedHandle(uint16_t _idx, uint16_t _ver)
	{
		m_idx = _idx;
		m_ver = _ver;
	}

	bool IsValid() const
	{
		return m_packed != c_invalidBitPattern;
	}

	union
	{
		struct
		{
			uint16_t m_idx;
			uint16_t m_ver;
		};

		uint32_t m_packed;
	};
};

// Simple LIFO versioned handle + data pool.
template <typename DataT>
class VersionedHandlePool
{
	KT_NO_COPY(VersionedHandlePool);
public:
	static uint32_t constexpr c_endIndex = 0xFFFFFFFF;
	using HandleType = VersionedHandle;

	struct DataIteratorBase
	{
		DataIteratorBase(VersionedHandlePool* _pool, uint32_t _idx)
			: m_pool(_pool)
			, m_idx(_idx)
		{}

		DataIteratorBase& operator++();
		DataIteratorBase operator++(int);

		bool operator==(DataIteratorBase const& _other) const { return m_pool == _other.m_pool && m_idx == _other.m_idx; }
		bool operator!=(DataIteratorBase const& _other) const { return !(*this == _other); }
		
		VersionedHandlePool* m_pool;
		uint32_t m_idx;
	};

	struct ConstDataIterator : DataIteratorBase
	{
		ConstDataIterator(VersionedHandlePool* _pool, uint32_t _idx)
			: DataIteratorBase(_pool, _idx)
		{}

		ConstDataIterator(DataIteratorBase const& _iter)
			: DataIteratorBase(_iter)
		{}

		DataT const& operator*() const { KT_ASSERT(this->m_pool->IsIndexInUse(this->m_idx)); return *this->m_pool->LookupAtIndex(this->m_idx); }
		DataT const* operator->() const { KT_ASSERT(this->m_pool->IsIndexInUse(this->m_idx)); return this->m_pool->LookupAtIndex(this->m_idx); }
	};

	struct DataIterator : DataIteratorBase
	{
		DataIterator(VersionedHandlePool* _pool, uint32_t _idx)
			: DataIteratorBase(_pool, _idx)
		{
		}

		DataIterator(DataIteratorBase const& _iter)
			: DataIteratorBase(_iter)
		{
		}

		DataT& operator*() const { KT_ASSERT(this->m_pool->IsIndexInUse(this->m_idx)); return *this->m_pool->LookupAtIndex(this->m_idx); }
		DataT* operator->() const { KT_ASSERT(this->m_pool->IsIndexInUse(this->m_idx)); return this->m_pool->LookupAtIndex(this->m_idx); }
	};

	VersionedHandlePool() = default;
	~VersionedHandlePool();

	VersionedHandlePool(VersionedHandlePool&& _other);
	VersionedHandlePool& operator=(VersionedHandlePool&& _other);

	void Init(IAllocator* _alloc, uint32_t _initialCapacity);

	DataT* Lookup(HandleType _handle);

	HandleType Alloc(DataT*& o_data);
	void Free(HandleType _handle);

	bool IsValid(HandleType _handle) const;
	bool IsIndexInUse(uint32_t _index) const;

	DataT* LookupAtIndex(uint32_t _idx);
	DataT* LookupAtIndex(uint32_t _idx) const;

	uint32_t NumAllocated() const { return m_numAllocated; }
	uint32_t Capacity() const { return m_capacity; }

	uint32_t FirstAllocatedIndex() const;
	uint32_t NextAllocatedIndex(uint32_t _index) const;
	HandleType HandleForIndex(uint32_t _index) const;

	DataIterator Begin();
	DataIterator End();

	ConstDataIterator Begin() const;
	ConstDataIterator End() const;

private:
	void Grow(uint32_t _sizeHint);

	struct Entry
	{
		uint32_t m_next : 31;
		uint32_t m_inUse : 1;
		uint16_t m_version;

		DataT m_data;
	};

	IAllocator* m_allocator = nullptr;
	Entry* m_entries = nullptr;
	uint32_t m_numAllocated = 0;
	uint32_t m_capacity = 0;

	uint32_t m_freeListHead = 0;
};

}

#include "inl/Handles.inl"