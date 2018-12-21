#pragma once
#include "kt.h"
#include "Memory.h"

namespace kt
{

// Extremely simple linear/frame/bump allocator. Alloc simply bumps pointer from the provided memory block. 
// Free is no-op, realloc simply calls alloc. Reset frees all allocations at once.
struct LinearAllocator : IAllocator
{
	KT_NO_COPY(LinearAllocator);

	LinearAllocator() = default;
	LinearAllocator(void* _mem, size_t _memSize);
	~LinearAllocator();

	void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) override;
	void* ReAlloc(void* _ptr, size_t const _sz) override;
	void Free(void* _ptr) override;

	void Init(void* _mem, size_t const _memSize);

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

	void* m_curPtr = nullptr;
};

// Convenience for wrapping linear allocator with polymorphic backing allocator.
struct BackedLinearAllocator : private LinearAllocator
{
	BackedLinearAllocator(IAllocator* _allocator, size_t _sz);
	BackedLinearAllocator() = default;
	~BackedLinearAllocator();

	// Creating memory block of _sz from provided allocator.
	void Init(IAllocator* _allocator, size_t _sz);

	// Retrieve the provided backing allocator.
	IAllocator* GetBackingAllocator();

	using LinearAllocator::Alloc;
	using LinearAllocator::Free;
	using LinearAllocator::BytesAllocated;
	using LinearAllocator::MemorySize;
	using LinearAllocator::Reset;
	using LinearAllocator::BasePointer;

private:
	IAllocator* m_allocator = nullptr;
};

}