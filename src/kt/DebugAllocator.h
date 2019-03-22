#pragma once
#include "kt.h"
#include "HashMap.h"
#include "StackTrace.h"
#include "Memory.h"


namespace kt
{

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
	LeakCheckAllocator(IAllocator* _allocator);
	~LeakCheckAllocator();

	void CheckForLeaks();

	void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) override;
	void* ReAllocUnsized(void* _ptr, size_t const _sz, size_t const _align) override;
	void FreeSized(void* _ptr, size_t const _size) override;
	void FreeUnsized(void* _ptr) override;

	void SetAllocatorAndClear(IAllocator* _allocator);
	IAllocator* GetAllocator() const;

private:
	AllocHeader* GetHeader(void* _ptr);
	size_t AdjustSizeForHeader(size_t _sz, size_t _align);

	void* HandleFreeAndGetRealPtr(void* _ptr);

	IAllocator* m_baseAllocator = nullptr;

	AllocHeader m_dummy;

	AllocHeader* m_head = nullptr;
};


}
