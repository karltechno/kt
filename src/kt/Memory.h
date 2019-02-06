#pragma once
#include "kt.h"

#include <malloc.h>
#include <stddef.h>

#if KT_PLATFORM_POSIX
	#include <alloca.h>
#endif

#define KT_ALLOCA(_sz) ::alloca(_sz)

#define KT_DEFAULT_ALIGN (16)

#define KT_OVERRIDE_NEW (1)

#if KT_OVERRIDE_NEW

void* operator new  (std::size_t count);
void* operator new[](std::size_t count);
void* operator new  (std::size_t count, const std::nothrow_t&);
void* operator new[](std::size_t count, const std::nothrow_t&);

void operator delete  (void* ptr);
void operator delete[](void* ptr);
void operator delete  (void* ptr, const std::nothrow_t&);
void operator delete[](void* ptr, const std::nothrow_t&);
#endif

namespace kt
{

uintptr_t AlignValue(uintptr_t  const _size, uintptr_t const _align);

bool IsAligned(uintptr_t const _val, uintptr_t const _align);

template <typename T, typename... Args>
T* PlacementNew(T* _mem, Args... _args);

size_t GetPointerAlignment(void* _ptr);
						   
struct IAllocator
{
	virtual ~IAllocator() {}

	virtual void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) = 0;
	virtual void* ReAlloc(void* _ptr, size_t const _sz) = 0;
	virtual void Free(void* _ptr) = 0;
	virtual void Free(void* _ptr, size_t const _sz) { KT_UNUSED(_sz); Free(_ptr); }
};

struct CrtAllocator : IAllocator
{
	void* Alloc(size_t const _sz, size_t const _align) override;
	void* ReAlloc(void* _ptr, size_t const _sz) override;
	void Free(void* _ptr) override;
};

IAllocator* GetDefaultAllocator();
void SetDefaultAllocator(IAllocator* _allocator);

template <typename T, typename... Args>
T* New(IAllocator* _allocator, Args... _args);

template <typename T>
void Delete(IAllocator* _allocator, T*& _ptr);

template <typename T, typename... Args>
T* New(Args... _args);

template <typename T>
void Delete(T*& _ptr);

void* Malloc(size_t const _s, size_t const _align = KT_DEFAULT_ALIGN);
void Free(void* _p);
void* Realloc(void* _p, size_t const _sz);


// Simple allocator for adding an inplace buffer with fallback to a container. 
// Ideally the container should reserve all the memory up front otherwise any form of exponential growth might waste some of the local space.
// Note: Currently ReAlloc assumes allocations are all contiguous from the same container.
template <size_t StaticSizeT, size_t AlignT>
struct InplaceContainerAllocator : IAllocator
{
	InplaceContainerAllocator(IAllocator* _fallback = GetDefaultAllocator())
		: m_fallback(_fallback)
	{}

	void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) override;
	void* ReAlloc(void* _ptr, size_t const _sz) override;
	void Free(void* _ptr) override;

	bool IsPointerLocal(void* _ptr) const;

	IAllocator* m_fallback;
	size_t m_used = 0;
	KT_ALIGNAS(AlignT) char m_buff[StaticSizeT];
};


}

#include "inl/Memory.inl"