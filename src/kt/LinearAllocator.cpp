#include "LinearAllocator.h"

#include <string.h>

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
		m_allocator->FreeSized((void*)m_memBegin, m_memEnd - m_memBegin);
	}
}


void* LinearAllocator::Alloc(size_t const _sz, size_t const _align)
{
	uintptr_t const current = m_curPtr;

	uintptr_t const aligned = AlignUp(current, _align);
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
	uintptr_t const aligned = kt::AlignUp(m_curPtr, _align);
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

bool LinearAllocator::IsPointerFrom(void* _ptr) const
{
	uintptr_t const ptr = uintptr_t(_ptr);
	return ptr >= m_memBegin && ptr < m_memEnd;
}

void* LinearAllocator::ReAllocUnsized(void* _ptr, size_t const _size, size_t const _align)
{
	KT_UNUSED3(_ptr, _size, _align);
	KT_ASSERT(false && "ReAllocUnsized not implemented.");
	return nullptr;
}

void* LinearAllocator::ReAllocSized(void* _ptr, size_t const _oldSize, size_t const _newSize, size_t const _align)
{
	if (!_ptr)
	{
		return Alloc(_newSize, _align);
	}

	KT_ASSERT(IsPointerFrom(_ptr));

	void* lastAlloc = (void*)(uintptr_t(CurrentPointer()) - m_lastAllocSize);
	if (lastAlloc == _ptr && IsAligned(uintptr_t(_ptr), _align))
	{
		KT_ASSERT(_newSize >= m_lastAllocSize);
		uintptr_t const allocSizeDiff = _newSize - m_lastAllocSize;
		if (allocSizeDiff + m_curPtr >= m_memEnd)
		{
			return nullptr;
		}

		m_lastAllocSize += allocSizeDiff;
		m_curPtr += allocSizeDiff;
		return _ptr;
	}
	else
	{
		void* newMem = Alloc(_newSize, _align);
		if (newMem)
		{
			memcpy(newMem, _ptr, _oldSize);
		}

		return newMem;
	}
}


}