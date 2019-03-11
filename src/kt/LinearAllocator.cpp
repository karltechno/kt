#include "LinearAllocator.h"

namespace kt
{

LinearAllocator::LinearAllocator(void* _mem, size_t _memSize)
{
	Init(_mem, _memSize);
}

LinearAllocator::LinearAllocator(IAllocator* _backingAllocator, size_t _memSize)
{
	Init(_backingAllocator, _memSize);
}

LinearAllocator::~LinearAllocator()
{
	KT_ASSERT(m_curPtr == m_memBegin);
	if (m_allocator)
	{
		m_allocator->Free((void*)m_memBegin, m_memEnd - m_memBegin);
	}
}


void* LinearAllocator::Alloc(size_t const _sz, size_t const _align)
{
	uintptr_t const current = m_curPtr;

	uintptr_t const aligned = AlignValue(current, _align);
	if (aligned + _sz > m_memEnd)
	{
		// Cant satisfy this request.
		return nullptr;
	}
	m_curPtr = aligned + _sz;
	m_lastAllocSize = _sz;
	return (void*)aligned;
}


void LinearAllocator::Init(void* _mem, size_t const _memSize)
{
	KT_ASSERT(!m_allocator);
	m_memBegin = uintptr_t(_mem);
	m_memEnd = m_memBegin + _memSize;
	m_curPtr = m_memBegin;
}


void LinearAllocator::Init(IAllocator* _allocator, size_t const _memSize)
{
	KT_ASSERT(!m_memBegin);
	KT_ASSERT(!m_allocator);

	m_allocator = _allocator;
	m_memBegin = uintptr_t(_allocator->Alloc(_memSize));
	m_curPtr = m_memBegin;
	m_memEnd = m_memBegin + _memSize;
}

void LinearAllocator::Reset()
{
	m_curPtr = m_memBegin;
}

size_t LinearAllocator::BytesAllocated() const
{
	return m_curPtr - m_memBegin;
}

void LinearAllocator::RewindToBytesAllocated(size_t const _num)
{
	KT_ASSERT(BytesAllocated() >= _num);
	m_curPtr = m_memBegin + _num;
}

void* LinearAllocator::BasePointer() const
{
	return (void*)m_memBegin;
}

void* LinearAllocator::CurrentPointer() const
{
	return (void*)m_curPtr;
}

void* LinearAllocator::Align(size_t const _align)
{
	uintptr_t const aligned = kt::AlignValue(m_curPtr, _align);
	if (aligned >= m_memEnd)
	{
		return nullptr;
	}

	m_curPtr = aligned;
	return (void*)aligned;
}

size_t LinearAllocator::MemorySize() const
{
	return (uintptr_t)m_memEnd - (uintptr_t)m_memBegin;
}

void* LinearAllocator::ReAlloc(void* _ptr, size_t const _sz)
{
	// Todo: ptr == 0 is valid? means alloc in crt?
	KT_ASSERT(_ptr);
	if (uintptr_t(_ptr) == m_curPtr - m_lastAllocSize)
	{
		KT_ASSERT(_sz > m_lastAllocSize);
		if (Alloc(_sz - m_lastAllocSize, 1) != nullptr)
		{
			m_lastAllocSize = _sz;
			return _ptr;
		}
		return nullptr;
	}

	return Alloc(_sz, KT_DEFAULT_ALIGN);
}

void LinearAllocator::Free(void* _ptr)
{
	// No-op - TODO: Track allocations in debug?
	KT_UNUSED(_ptr);
}

}