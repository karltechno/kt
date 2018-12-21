#include "LinearAllocator.h"

namespace kt
{


LinearAllocator::LinearAllocator(void* _mem, size_t _memSize)
{
	Init(_mem, _memSize);
}

LinearAllocator::~LinearAllocator()
{
	KT_ASSERT(m_curPtr == m_memBegin);
}

void* LinearAllocator::Alloc(size_t const _sz, size_t const _align)
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

void LinearAllocator::Init(void* _mem, size_t const _memSize)
{
	m_memBegin = _mem;
	m_memEnd = (void*)((uintptr_t)_mem + _memSize);
	m_curPtr = _mem;
}

void LinearAllocator::Reset()
{
	m_curPtr = m_memBegin;
}

size_t LinearAllocator::BytesAllocated() const
{
	return (uintptr_t)m_curPtr - (uintptr_t)m_memBegin;
}

void* LinearAllocator::BasePointer() const
{
	return m_memBegin;
}

size_t LinearAllocator::MemorySize() const
{
	return (uintptr_t)m_memEnd - (uintptr_t)m_memBegin;
}

void* kt::LinearAllocator::ReAlloc(void* _ptr, size_t const _sz)
{
	KT_UNUSED(_ptr);
	return Alloc(_sz, KT_DEFAULT_ALIGN);
}

void kt::LinearAllocator::Free(void* _ptr)
{
	// No-op - TODO: Track allocations in debug?
	KT_UNUSED(_ptr);
}

BackedLinearAllocator::BackedLinearAllocator(IAllocator* _allocator, size_t _sz)
{
	Init(_allocator, _sz);
}

BackedLinearAllocator::~BackedLinearAllocator()
{
	if (m_allocator)
	{
		m_allocator->Free(BasePointer());
	}
}

void BackedLinearAllocator::Init(IAllocator* _allocator, size_t _sz)
{
	KT_ASSERT(BasePointer() == nullptr);
	m_allocator = _allocator;
	LinearAllocator::Init(_allocator->Alloc(_sz), _sz);
}

IAllocator* BackedLinearAllocator::GetBackingAllocator()
{
	return m_allocator;
}

}