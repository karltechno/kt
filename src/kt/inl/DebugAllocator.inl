#pragma once
#include "../DebugAllocator.h"
#include "../Logging.h"

namespace kt
{

template <typename BaseAllocatorT>
LeakCheckAllocator<BaseAllocatorT>::~LeakCheckAllocator()
{
	CheckForLeaks();
}

template <typename BaseAllocatorT>
LeakCheckAllocator<BaseAllocatorT>::LeakCheckAllocator()
{
	m_dummy.m_ptrToFree = nullptr;
	m_dummy.m_allocSize = 0;
	m_dummy.m_next = &m_dummy;
	m_dummy.m_prev = &m_dummy;
	m_head = &m_dummy;
}

template <typename BaseAllocatorT>
void LeakCheckAllocator<BaseAllocatorT>::Free(void* _ptr, size_t const _sz)
{
	m_baseAllocator.Free(HandleFreeAndGetRealPtr(_ptr), _sz);
}

template <typename BaseAllocatorT>
void LeakCheckAllocator<BaseAllocatorT>::Free(void* _ptr)
{
	m_baseAllocator.Free(HandleFreeAndGetRealPtr(_ptr));
}


template <typename BaseAllocatorT>
void* kt::LeakCheckAllocator<BaseAllocatorT>::HandleFreeAndGetRealPtr(void* _ptr)
{
	if (!_ptr)
	{
		return nullptr;
	}

	AllocHeader* header = GetHeader(_ptr);
	header->m_prev->m_next = header->m_next;
	header->m_next->m_prev = header->m_prev;
	return header->m_ptrToFree;
}

template <typename BaseAllocatorT>
void* LeakCheckAllocator<BaseAllocatorT>::ReAlloc(void* _ptr, size_t const _sz)
{
	// Todo: maybe actually support realloc
	AllocHeader* hdr = GetHeader(_ptr);
	void* newPtr = Alloc(_sz);

	memcpy(newPtr, _ptr, hdr->m_allocSize);

	Free(_ptr, _sz);
	return newPtr;
}

template <typename BaseAllocatorT>
void* LeakCheckAllocator<BaseAllocatorT>::Alloc(size_t const _sz, size_t const _align /*= KT_DEFAULT_ALIGN*/)
{
	size_t const newSize = AdjustSizeForHeader(_sz, _align);
	void* original = m_baseAllocator.Alloc(newSize, _align);
	uintptr_t adjustedRetPtr = kt::AlignValue(uintptr_t(original) + sizeof(AllocHeader), _align);
	AllocHeader* header = (AllocHeader*)(adjustedRetPtr - sizeof(AllocHeader));

	void* retPtr = (void*)(adjustedRetPtr);
	
	KT_ASSERT(uintptr_t(original) + newSize >= adjustedRetPtr + _sz);

	header->m_trace.Capture(1);
	header->m_ptrToFree = original;
	header->m_allocSize = _sz;

	header->m_next = m_head->m_next;
	header->m_prev = m_head;

	m_head->m_next->m_prev = header;
	m_head->m_next = header;

	return retPtr;
}

template <typename BaseAllocatorT>
void LeakCheckAllocator<BaseAllocatorT>::CheckForLeaks()
{
	AllocHeader* cur = m_head;

	if (cur->m_next == cur && cur->m_prev == cur)
	{
		return;
	}

	cur = m_head->m_next;

	kt::LogError("Memory leaks detected! Printing callstacks.");

	do
	{
		kt::StackTrace const& trace = cur->m_trace;

		kt::LogError("Leak - alloc size: %u, ptr: %p\n", cur->m_allocSize, cur->m_ptrToFree);

		for (uint32_t i = 0; i < kt::c_maxStackTraceFrames; ++i)
		{
			kt::String512 sym, path;
			uint32_t line;
			if (!trace.ResolveSymbol(i, sym, &line, &path))
			{
				break;
			}

			kt::LogError("%s - %s - %u", sym.Data(), path.Data(), line);
		}

		cur = cur->m_next;
		kt::LogError("\n"); 
	} while (cur != &m_dummy);
}


template <typename BaseAllocatorT>
size_t kt::LeakCheckAllocator<BaseAllocatorT>::AdjustSizeForHeader(size_t _sz, size_t _align)
{
	return _sz + kt::AlignValue(sizeof(AllocHeader), _align);
}

template <typename BaseAllocatorT>
auto kt::LeakCheckAllocator<BaseAllocatorT>::GetHeader(void* _ptr) -> AllocHeader*
{
	return (AllocHeader*)(uintptr_t(_ptr) - sizeof(AllocHeader));
}

}