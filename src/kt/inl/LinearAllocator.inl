#pragma once
#include "../LinearAllocator.h"
#include "../Concurrency.h"

namespace kt
{

template <LinearAllocatorThreadSafety ThreadSafeT>
LinearAllocator<ThreadSafeT>::LinearAllocator(void* _mem, size_t _memSize)
{
	Init(_mem, _memSize);
}

template <LinearAllocatorThreadSafety ThreadSafeT>
LinearAllocator<ThreadSafeT>::LinearAllocator(IAllocator* _backingAllocator, size_t _memSize)
{
	Init(_backingAllocator, _memSize);
}


template <LinearAllocatorThreadSafety ThreadSafeT>
LinearAllocator<ThreadSafeT>::~LinearAllocator()
{
	KT_ASSERT(m_curPtr.load() == (uintptr_t)m_memBegin);
	if (m_allocator)
	{
		m_allocator->Free(m_memBegin);
	}
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void* LinearAllocator<ThreadSafeT>::Alloc(size_t const _sz, size_t const _align)
{
	if (ThreadSafeT == LinearAllocatorThreadSafety::ThreadSafeAlloc)
	{
		size_t const worstCaseAllocSize = _sz + (_align - 1);
		void* allocBegin = (void*)std::atomic_fetch_add_explicit(&m_curPtr, worstCaseAllocSize, std::memory_order_relaxed);

		uintptr_t const alignedPtr = AlignValue((uintptr_t)allocBegin, _align);
		if (alignedPtr + _sz > (uintptr_t)m_memEnd)
		{
			// Can't satisfy this request
			return nullptr;
		}
		return (void*)alignedPtr;
	}
	else
	{
		// Todo: re-evaluate use of std::atomic with thread safety template (although this is just plain mov's on x86).

		uintptr_t const current = (uintptr_t)std::atomic_load_explicit(&m_curPtr, std::memory_order_relaxed);

		uintptr_t const aligned = AlignValue(current, _align);
		if (aligned + _sz > (uintptr_t)m_memEnd)
		{
			// Cant satisfy this request.
			return nullptr;
		}
		std::atomic_store_explicit(&m_curPtr, aligned + _sz, std::memory_order_relaxed);
		return (void*)aligned;
	}
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void LinearAllocator<ThreadSafeT>::Init(void* _mem, size_t const _memSize)
{
	KT_ASSERT(!m_allocator);
	m_memBegin = _mem;
	m_memEnd = (void*)((uintptr_t)_mem + _memSize);
	m_curPtr = _mem;
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void LinearAllocator<ThreadSafeT>::Init(IAllocator* _allocator, size_t const _memSize)
{
	KT_ASSERT(!m_memBegin);
	KT_ASSERT(!m_allocator);

	m_allocator = _allocator;
	m_memBegin = _allocator->Alloc(_memSize);
	std::atomic_store_explicit(&m_curPtr, (uintptr_t)m_memBegin, std::memory_order_relaxed);
	m_memEnd = (void*)((uintptr_t)m_memBegin + _memSize);
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void LinearAllocator<ThreadSafeT>::Reset()
{
	std::atomic_store_explicit(&m_curPtr, (uintptr_t)m_memBegin, std::memory_order_relaxed);
}

template <LinearAllocatorThreadSafety ThreadSafeT>
size_t LinearAllocator<ThreadSafeT>::BytesAllocated() const
{
	return (uintptr_t)m_curPtr - (uintptr_t)m_memBegin;
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void* LinearAllocator<ThreadSafeT>::BasePointer() const
{
	return m_memBegin;
}

template <LinearAllocatorThreadSafety ThreadSafeT>
size_t LinearAllocator<ThreadSafeT>::MemorySize() const
{
	return (uintptr_t)m_memEnd - (uintptr_t)m_memBegin;
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void* LinearAllocator<ThreadSafeT>::ReAlloc(void* _ptr, size_t const _sz)
{
	KT_UNUSED(_ptr);
	return Alloc(_sz, KT_DEFAULT_ALIGN);
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void LinearAllocator<ThreadSafeT>::Free(void* _ptr)
{
	// No-op - TODO: Track allocations in debug?
	KT_UNUSED(_ptr);
}


}