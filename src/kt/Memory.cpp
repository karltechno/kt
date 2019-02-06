#include <malloc.h>

#include "Memory.h"
#include "Platform.h"
#include "kt.h"
#include "Macros.h"

namespace kt
{

static CrtAllocator s_defaultCrtAllocator;
static IAllocator* s_defaultAllocator = &s_defaultCrtAllocator;

uintptr_t AlignValue(uintptr_t const _size, uintptr_t const _align)
{
	KT_ASSERT(IsPow2(_align));
	return ((_size - 1u) + _align) & ~(_align - 1u);
}

bool IsAligned(uintptr_t const _val, uintptr_t const _align)
{
	KT_ASSERT(IsPow2(_align));
	return (_val & (_align - 1u)) == 0;
}

size_t GetPointerAlignment(void* _ptr)
{
	if (!_ptr)
	{
		return 0;
	}

	return (uintptr_t)1 << Cnttz((uintptr_t)_ptr);
}

void* CrtAllocator::Alloc(size_t const _sz, size_t const _align)
{
#if KT_COMPILER_MSVC
	return ::_aligned_malloc(_sz, Min<size_t>(KT_DEFAULT_ALIGN, _align));
#else
	void* pmem = nullptr;
	if (posix_memalign(&pmem, Min<size_t>(KT_DEFAULT_ALIGN, _align), _sz) == 0)
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

void* CrtAllocator::ReAlloc(void* _ptr, size_t const _sz)
{
#if KT_PLATFORM_WINDOWS
	return _aligned_realloc(_ptr, _sz, KT_DEFAULT_ALIGN);
#else
	size_t const oldSize = CrtAllocSize(_ptr, _align);

	void* newP = Alloc(_sz, _align);
	memcpy(newP, _ptr, oldSize < _sz ? oldSize : _sz);
	Free(_ptr);
	return newP;
#endif
}

void CrtAllocator::Free(void* _ptr)
{
#if KT_COMPILER_MSVC
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
	s_defaultAllocator->Free(_p);
}

void* Realloc(void* _p, size_t const _sz)
{
	return s_defaultAllocator->ReAlloc(_p, _sz);
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

void* operator new[](std::size_t count, const std::nothrow_t&)
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}

void* operator new[](std::size_t count)
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}

void operator delete(void* ptr)
{
	kt::s_defaultAllocator->Free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&)
{
	kt::s_defaultAllocator->Free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&)
{
	kt::s_defaultAllocator->Free(ptr);
}

void operator delete[](void* ptr)
{
	kt::s_defaultAllocator->Free(ptr);
}

void* operator new(std::size_t count, const std::nothrow_t&)
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}

void* operator new(std::size_t count)
{
	return kt::s_defaultAllocator->Alloc(count, KT_DEFAULT_ALIGN);
}
