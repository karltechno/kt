#pragma once
#include "../Memory.h"

namespace kt { struct PlacementNewTag {}; }

inline void* operator new(size_t, kt::PlacementNewTag, void* _ptr)
{
	return _ptr;
}

inline void operator delete(void*, kt::PlacementNewTag, void*) throw() {}

namespace kt
{

inline uintptr_t AlignUp(uintptr_t const _size, uintptr_t const _align)
{
	KT_ASSERT(IsPow2(_align));
	return ((_size - 1u) + _align) & ~(_align - 1u);
}

inline bool IsAligned(uintptr_t const _val, uintptr_t const _align)
{
	KT_ASSERT(IsPow2(_align));
	return (_val & (_align - 1u)) == 0;
}

template <typename T, typename... Args>
T* PlacementNew(T* _mem, Args... _args)
{
	return new (PlacementNewTag{}, _mem) T(std::forward<Args>(_args)...);
}

template <typename T, typename... Args>
T* New(IAllocator* _allocator, Args... _args)
{
	T* mem = (T*)_allocator->Alloc(sizeof(T), KT_ALIGNOF(T));
	return PlacementNew(mem, std::forward<Args>(_args)...);
}

template <typename T>
void Delete(IAllocator* _allocator, T*& _ptr)
{
	_ptr->~T();
	_allocator->Free(_ptr);
	_ptr = nullptr;
}

template <size_t StaticSizeT, size_t AlignT>
void* InplaceContainerAllocator<StaticSizeT, AlignT>::Alloc(size_t const _sz, size_t const _align)
{
	KT_UNUSED(_align);

	if (m_used + _sz <= StaticSizeT)
	{
		void* ret = (void*)(m_buff + m_used);
		m_used += _sz;
		return ret;
	}
	else
	{
		void* newMem = m_fallback->Alloc(_sz, _align);
		memcpy(newMem, m_buff, m_used);
		return newMem;
	}
}

template <size_t StaticSizeT, size_t AlignT>
void* InplaceContainerAllocator<StaticSizeT, AlignT>::ReAllocUnsized(void* _ptr, size_t const _sz, size_t const _align)
{
	if (IsPointerLocal(_ptr))
	{
		if (m_used + _sz <= StaticSizeT)
		{
			KT_ASSERT(IsAligned(uintptr_t(m_buff), _align));
			m_used += _sz;
			return _ptr;
		}
		else
		{
			void* newMem = m_fallback->Alloc(_sz, _align);
			memcpy(newMem, m_buff, m_used);
			return newMem;
		}
	}
	else
	{
		return m_fallback->ReAllocUnsized(_ptr, _sz, _align);
	}
}

template <size_t StaticSizeT, size_t AlignT>
void InplaceContainerAllocator<StaticSizeT, AlignT>::FreeUnsized(void* _ptr)
{
	if (!IsPointerLocal(_ptr))
	{
		m_fallback->FreeUnsized(_ptr);
	}
}


template <size_t StaticSizeT, size_t AlignT>
void kt::InplaceContainerAllocator<StaticSizeT, AlignT>::FreeSized(void* _ptr, size_t const _size)
{
	if (!IsPointerLocal(_ptr))
	{
		m_fallback->FreeSized(_ptr, _size);
	}
}


template <size_t StaticSizeT, size_t AlignT>
void* kt::InplaceContainerAllocator<StaticSizeT, AlignT>::ReAllocSized(void* _ptr, size_t const _oldSize, size_t const _newSize, size_t const _align /*= KT_DEFAULT_ALIGN*/)
{
	if (IsPointerLocal(_ptr))
	{
		size_t const newUsed = (uintptr_t(_ptr) - uintptr_t(m_buff)) + _newSize;

		if (newUsed <= StaticSizeT)
		{
			KT_ASSERT(IsAligned(uintptr_t(m_buff), _align));
			m_used = newUsed;
			return _ptr;
		}
		else
		{
			void* newMem = m_fallback->Alloc(_newSize, _align);
			memcpy(newMem, m_buff, m_used);
			return newMem;
		}
	}
	else
	{
		return m_fallback->ReAllocSized(_ptr, _oldSize, _newSize, _align);
	}
}



template <size_t StaticSizeT, size_t AlignT>
bool InplaceContainerAllocator<StaticSizeT, AlignT>::IsPointerLocal(void* _ptr) const
{
	return ((uintptr_t)_ptr >= (uintptr_t)m_buff) && ((uintptr_t)_ptr < (uintptr_t)(m_buff + StaticSizeT));
}

}