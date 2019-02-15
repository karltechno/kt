#pragma once
#include "kt.h"
#include "Memory.h"
#include "Concurrency.h"

namespace kt
{


enum class LinearAllocatorThreadSafety
{
	// Alloc is not thread safe.
	NonThreadSafe,

	// Alloc is thread safe.
	ThreadSafeAlloc
};

// Extremely simple linear/frame/bump allocator. Alloc simply bumps pointer from the provided memory block. 
// Free is no-op, realloc simply calls alloc. Reset frees all allocations at once.
template <LinearAllocatorThreadSafety ThreadSafeT = LinearAllocatorThreadSafety::NonThreadSafe>
struct LinearAllocator : IAllocator
{
	KT_NO_COPY(LinearAllocator);

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

	// The backing memory block.
	void* BasePointer() const; 

	// Total bytes in backing memory block (regardless of allocations).
	size_t MemorySize() const;

private:
	void* m_memBegin = nullptr;
	void* m_memEnd = nullptr;

	std::atomic<uintptr_t> m_curPtr = 0;

	IAllocator* m_allocator = nullptr;
};

}

#include "inl/LinearAllocator.inl"