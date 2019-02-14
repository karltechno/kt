#pragma once
#include "kt.h"
#include "HashMap.h"
#include "StackTrace.h"
#include "Memory.h"


namespace kt
{

template <typename BaseAllocatorT>
struct LeakCheckAllocator : IAllocator
{
	struct AllocHeader
	{
		kt::StackTrace m_trace;
		AllocHeader* m_prev;
		AllocHeader* m_next;
		void* m_ptrToFree;
		size_t m_allocSize;
	};

	LeakCheckAllocator();
	~LeakCheckAllocator();

	void CheckForLeaks();

	void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) override;
	void* ReAlloc(void* _ptr, size_t const _sz) override;
	void Free(void* _ptr) override;
	void Free(void* _ptr, size_t const _sz) override;

	BaseAllocatorT m_baseAllocator;

private:
	AllocHeader* GetHeader(void* _ptr);
	size_t AdjustSizeForHeader(size_t _sz, size_t _align);

	void* HandleFreeAndGetRealPtr(void* _ptr);

	AllocHeader m_dummy;

	AllocHeader* m_head;
};



}

#include "inl/DebugAllocator.inl"