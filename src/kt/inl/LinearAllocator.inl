#pragma once
#include "LinearAllocator.h"
#include "Concurrency.h"

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
	KT_ASSERT(m_curPtr == m_memBegin);
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
		size_t const worstCaseAllocSize = AlignValue(_sz, _align);
		
#if KT_ARCH_64BIT
		void* const allocBegin = (void*)kt::AtomicFetchAdd64((int64_t*)&m_curPtr, worstCaseAllocSize);
#else
		void* const allocBegin = (void*)kt::AtomicFetchAdd32((int32_t*)&m_curPtr, worstCaseAllocSize);
#endif
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
		uintptr_t const current = (uintptr_t)m_curPtr;

		uintptr_t const aligned = AlignValue(current, _align);
		if (aligned + _sz > (uintptr_t)m_memEnd)
		{
			// Cant satisfy this request.
			return nullptr;
		}

		m_curPtr = (void*)(aligned + _sz);
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
	m_curPtr = m_memBegin;
	m_memEnd = (void*)((uintptr_t)m_memBegin + _memSize);
}

template <LinearAllocatorThreadSafety ThreadSafeT>
void LinearAllocator<ThreadSafeT>::Reset()
{
	m_curPtr = m_memBegin;
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