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
	using HandleType = VersionedHandle;

	VersionedHandlePool() = default;
	~VersionedHandlePool();

	VersionedHandlePool(VersionedHandlePool&& _other);
	VersionedHandlePool& operator=(VersionedHandlePool&& _other);

	void Init(IAllocator* _alloc, uint32_t _initialCapacity);

	DataT* Lookup(HandleType _handle);

	HandleType Alloc(DataT*& o_data);
	void Free(HandleType _handle);

	bool IsValid(HandleType _handle) const;

	uint32_t NumAllocated() const { return m_numAllocated; }
	uint32_t Capacity() const { return m_capacity; }

	HandleType FirstAllocatedHandle() const;
	HandleType NextAllocatedHandle(HandleType _handle) const;


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