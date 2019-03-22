#include <malloc.h>

#include "Memory.h"
#include "Platform.h"
#include "kt.h"
#include "Macros.h"
#include "DebugAllocator.h"

namespace kt
{

#if KT_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib) 
#pragma warning(pop)
#endif

static CrtAllocator s_defaultCrtAllocator;
static IAllocator* s_defaultAllocator = &s_defaultCrtAllocator;


void* CrtAllocator::Alloc(size_t const _size, size_t const _align)
{
#if KT_PLATFORM_WINDOWS
	return ::_aligned_malloc(_size, Min<size_t>(KT_DEFAULT_ALIGN, _align));
#else
	void* pmem = nullptr;
	if (posix_memalign(&pmem, Min<size_t>(KT_DEFAULT_ALIGN, _align), _size) == 0)
	{
		return pmem;
	}
	return nullptr;
#endif
}

/* static */ size_t CrtAllocSize (void* _p, size_t const _align)
{
#if KT_PLATFORM_WINDOWS
	return _aligned_msize(_p, _align, 0);
#elif KT_PLATFORM_OSX
	KT_UNUSED(_align);
	return malloc_size(_p);
#elif KT_PLATFORM_POSIX
	KT_UNUSED(_align);
	return malloc_usable_size(_p);
#endif
}

void* CrtAllocator::ReAllocUnsized(void* _ptr, size_t const _sz, size_t const _align)
{
#if KT_PLATFORM_WINDOWS
	return _aligned_realloc(_ptr, _sz, _align);
#else
	size_t const oldSize = CrtAllocSize(_ptr, _align);

	void* newP = Alloc(_sz, _align);
	memcpy(newP, _ptr, oldSize < _sz ? oldSize : _sz);
	Free(_ptr);
	return newP;
#endif
}

void CrtAllocator::FreeUnsized(void* _ptr)
{
#if KT_PLATFORM_WINDOWS
	return ::_aligned_free(_ptr);
#else
	::free(_ptr);
#endif
}

void* Malloc(size_t const _s, size_t const _align)
{
	return s_defaultAllocator->Alloc(_s, _align);
}

void Free(void* _p)
{
	s_defaultAllocator->FreeUnsized(_p);
}

void* Realloc(void* _p, size_t const _sz)
{
	return s_defaultAllocator->ReAllocUnsized(_p, _sz);
}

IAllocator* GetDefaultAllocator()
{
	return s_defaultAllocator;
}

void SetDefaultAllocator(IAllocator* _allocator)
{
	s_defaultAllocator = _allocator;
}

}

#if KT_OVERRIDE_NEW

void* operator new[](std::size_t count, const std::nothrow_t&) noexcept
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}

void* operator new[](std::size_t count)
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}

void operator delete(void* ptr) noexcept
{
	kt::s_defaultAllocator->FreeUnsized(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
	kt::s_defaultAllocator->FreeUnsized(ptr);
}

void operator delete(void* ptr, std::size_t sz) noexcept
{
	kt::s_defaultAllocator->FreeSized(ptr, sz);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
	kt::s_defaultAllocator->FreeUnsized(ptr);
}

void operator delete[](void* ptr) noexcept
{
	kt::s_defaultAllocator->FreeUnsized(ptr);
}

void operator delete[](void* ptr, std::size_t sz) noexcept
{
	kt::s_defaultAllocator->FreeSized(ptr, sz);
}

void* operator new(std::size_t count, const std::nothrow_t&) noexcept
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}

void* operator new(std::size_t count)
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}

#if (defined(_HAS_ALIGNED_NEW) && _HAS_ALIGNED_NEW) || KT_CPP17
void* operator new  (std::size_t count, std::align_val_t al)
{
	return kt::s_defaultAllocator->Alloc(count, (size_t)al);
}

void* operator new[](std::size_t count, std::align_val_t al)
{
	return kt::s_defaultAllocator->Alloc(count, (size_t)al);
}

void* operator new  (std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept
{
	return kt::s_defaultAllocator->Alloc(count, (size_t)al);
}

void* operator new[](std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept
{
	return kt::s_defaultAllocator->Alloc(count, (size_t)al);
}

void operator delete  (void* ptr, std::size_t sz, std::align_val_t al)
{
	KT_UNUSED(al);
	kt::s_defaultAllocator->FreeSized(ptr, sz);
}

void operator delete[](void* ptr, std::size_t sz, std::align_val_t al)
{
	KT_UNUSED(al);
	kt::s_defaultAllocator->FreeSized(ptr, sz);
}

void operator delete  (void* ptr, std::align_val_t al)
{
	KT_UNUSED(al);
	kt::s_defaultAllocator->FreeUnsized(ptr);
}

void operator delete[](void* ptr, std::align_val_t al)
{
	KT_UNUSED(al);
	kt::s_defaultAllocator->FreeUnsized(ptr);
}

#endif

#endif