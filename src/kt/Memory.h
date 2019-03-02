#pragma once
#include "kt.h"

#include <malloc.h>
#include <cstddef>
#include <new>

#if KT_PLATFORM_POSIX
	#include <alloca.h>
#endif

#define KT_ALLOCA(_sz) ::alloca(_sz)

#define KT_DEFAULT_ALIGN (16)

#ifndef KT_OVERRIDE_NEW
	#define KT_OVERRIDE_NEW (1)
#endif

#if KT_OVERRIDE_NEW

void* operator new  (std::size_t count);
void* operator new[](std::size_t count);
void* operator new  (std::size_t count, const std::nothrow_t&) noexcept;
void* operator new[](std::size_t count, const std::nothrow_t&) noexcept;

void operator delete  (void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete  (void* ptr, const std::nothrow_t&) noexcept;
void operator delete[](void* ptr, const std::nothrow_t&) noexcept;
void operator delete  (void* ptr, std::size_t sz) noexcept;
void operator delete[](void* ptr, std::size_t sz) noexcept;

#if (defined(_HAS_ALIGNED_NEW) && _HAS_ALIGNED_NEW) || KT_CPP17
void* operator new  (std::size_t count, std::align_val_t al);
void* operator new[](std::size_t count, std::align_val_t al);
void* operator new  (std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept;
void* operator new[](std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept;

void operator delete  (void* ptr, std::size_t sz, std::align_val_t al);
void operator delete[](void* ptr, std::size_t sz, std::align_val_t al);
void operator delete  (void* ptr, std::align_val_t al);
void operator delete[](void* ptr, std::align_val_t al);
#endif

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
	void Free(void* _ptr, size_t const _sz) override { KT_UNUSED(_sz); Free(_ptr); }
};

IAllocator* GetDefaultAllocator();
void SetDefaultAllocator(IAllocator* _allocator);

template <typename T, typename... Args>
T* New(IAllocator* _allocator, Args... _args);

template <typename T>
void Delete(IAllocator* _allocator, T*& _ptr);


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