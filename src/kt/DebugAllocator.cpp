#include "DebugAllocator.h"
#include "Logging.h"

namespace kt
{

LeakCheckAllocator::LeakCheckAllocator(IAllocator* _allocator)
{
	SetAllocatorAndClear(_allocator);
}

LeakCheckAllocator::~LeakCheckAllocator()
{
	CheckForLeaks();
}

LeakCheckAllocator::LeakCheckAllocator()
{
	SetAllocatorAndClear(nullptr);
}

void LeakCheckAllocator::FreeSized(void* _ptr, size_t const _sz)
{
	m_baseAllocator->FreeSized(HandleFreeAndGetRealPtr(_ptr), _sz);
}

void LeakCheckAllocator::FreeUnsized(void* _ptr)
{
	m_baseAllocator->FreeUnsized(HandleFreeAndGetRealPtr(_ptr));
}

void* kt::LeakCheckAllocator::HandleFreeAndGetRealPtr(void* _ptr)
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

void* LeakCheckAllocator::ReAllocUnsized(void* _ptr, size_t const _sz, size_t const _align)
{
	if (!_ptr)
	{
		return Alloc(_sz, _align);
	}

	// Todo: maybe actually support realloc
	AllocHeader* hdr = GetHeader(_ptr);
	void* newPtr = Alloc(_sz, _align);

	memcpy(newPtr, _ptr, hdr->m_allocSize);

	FreeSized(_ptr, _sz);
	return newPtr;
}


void* LeakCheckAllocator::Alloc(size_t const _sz, size_t const _align /*= KT_DEFAULT_ALIGN*/)
{
	size_t const newSize = AdjustSizeForHeader(_sz, _align);
	void* original = m_baseAllocator->Alloc(newSize, _align);
	uintptr_t adjustedRetPtr = kt::AlignUp(uintptr_t(original) + sizeof(AllocHeader), _align);
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


void LeakCheckAllocator::CheckForLeaks()
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



size_t kt::LeakCheckAllocator::AdjustSizeForHeader(size_t _sz, size_t _align)
{
	return _sz + kt::AlignUp(sizeof(AllocHeader), _align);
}


LeakCheckAllocator::AllocHeader* LeakCheckAllocator::GetHeader(void* _ptr)
{
	return (AllocHeader*)(uintptr_t(_ptr) - sizeof(AllocHeader));
}


void LeakCheckAllocator::SetAllocatorAndClear(IAllocator* _allocator)
{
	m_baseAllocator = _allocator;

	m_dummy.m_ptrToFree = nullptr;
	m_dummy.m_allocSize = 0;
	m_dummy.m_next = &m_dummy;
	m_dummy.m_prev = &m_dummy;
	m_head = &m_dummy;
}


IAllocator* LeakCheckAllocator::GetAllocator() const
{
	return m_baseAllocator;
}

}