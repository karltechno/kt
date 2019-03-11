#pragma once
#include "kt.h"
#include "Memory.h"
#include "Concurrency.h"

namespace kt
{

// Extremely simple linear/frame/bump allocator. Alloc simply bumps pointer from the provided memory block. 
// Free is no-op, realloc simply calls alloc. Reset frees all allocations at once.
struct LinearAllocator : IAllocator
{
	KT_NO_COPY(LinearAllocator);

	struct AllocScope
	{
		AllocScope(LinearAllocator& _alloc)
			: m_allocator(_alloc)
		{
			m_rewindTo = _alloc.BytesAllocated();
		}

		~AllocScope() { m_allocator.RewindToBytesAllocated(m_rewindTo); }

	private:
		size_t m_rewindTo;
		LinearAllocator& m_allocator;
	};

	LinearAllocator() = default;
	LinearAllocator(void* _mem, size_t _memSize);
	LinearAllocator(IAllocator* _backingAllocator, size_t _memSize);
	~LinearAllocator();

	void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) override;
	void* ReAlloc(void* _ptr, size_t const _sz) override;
	void Free(void* _ptr) override;

	// Init from memory.
	void Init(void* _mem, size_t const _memSize);
	
	// Init from backing allocator
	void Init(IAllocator* _allocator, size_t const _memSize);

	// 'Frees' all allocated memory (bumps current allocation pointer back to memory block start).
	void Reset();

	// Current amount of bytes allocated.
	size_t BytesAllocated() const;

	// Rewind the memory block to _num bytes allocated. Can be quered from BytesAllocated() before allocating temporary memory.
	void RewindToBytesAllocated(size_t const _num);

	// The backing memory block.
	void* BasePointer() const; 

	// Total bytes in backing memory block (regardless of allocations).
	size_t MemorySize() const;

private:
	uintptr_t m_memBegin = 0;
	uintptr_t m_memEnd = 0;

	uintptr_t m_curPtr;

	uintptr_t m_lastAllocSize = 0;

	IAllocator* m_allocator = nullptr;
};

}
