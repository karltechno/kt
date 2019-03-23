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

inline uintptr_t AlignUp(uintptr_t const _size, uintptr_t const _align);

inline bool IsAligned(uintptr_t const _val, uintptr_t const _align);

template <typename T, typename... Args>
T* PlacementNew(T* _mem, Args... _args);

struct IAllocator
{
	virtual ~IAllocator() {}

	virtual void* Alloc(size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) = 0;

	virtual void* ReAllocSized(void* _ptr, size_t const _oldSize, size_t const _newSize, size_t const _align = KT_DEFAULT_ALIGN) { KT_UNUSED(_oldSize); return ReAllocUnsized(_ptr, _newSize, _align); }
	virtual void* ReAllocUnsized(void* _ptr, size_t const _size, size_t const _align = KT_DEFAULT_ALIGN) = 0;

	virtual void FreeUnsized(void* _ptr) = 0;
	virtual void FreeSized(void* _ptr, size_t const _size) { KT_UNUSED(_size); FreeUnsized(_ptr); }
};

struct CrtAllocator : IAllocator
{
	void* Alloc(size_t const _sz, size_t const _align) override;
	void* ReAllocUnsized(void* _ptr, size_t const _sz, size_t const _align = KT_DEFAULT_ALIGN) override;
	void FreeUnsized(void* _ptr) override;
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

	void* Alloc(size_t const _size, size_t const _align = KT_DEFAULT_ALIGN) override;

	void FreeUnsized(void* _ptr) override;
	void FreeSized(void* _ptr, size_t const _size) override;

	void* ReAllocUnsized(void* _ptr, size_t const _size, size_t const _align = KT_DEFAULT_ALIGN) override;
	void* ReAllocSized(void* _ptr, size_t const _oldSize, size_t const _newSize, size_t const _align = KT_DEFAULT_ALIGN) override;

	bool IsPointerLocal(void* _ptr) const;

	IAllocator* m_fallback;
	size_t m_used = 0;
	KT_ALIGNAS(AlignT) char m_buff[StaticSizeT];
};

}

#include "inl/Memory.inl"