#pragma once

namespace kt { struct PlacementNewTag {}; }

inline void* operator new(size_t, kt::PlacementNewTag, void* _ptr)
{
	return _ptr;
}

inline void operator delete(void*, kt::PlacementNewTag, void*) throw() {}

namespace kt
{

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

template <typename T, typename... Args>
T* New(Args... _args)
{
	return New(GetDefaultAllocator(), std::forward<Args>(_args)...);
}

template <typename T>
void Delete(T*& _ptr)
{
	return Delete(GetDefaultAllocator(), _ptr);
}

template <size_t StaticSizeT, size_t AlignT>
void* InplaceContainerAllocator<StaticSizeT, AlignT>::Alloc(size_t const _sz, size_t const _align)
{
	KT_UNUSED(_align);

	if (m_used + _sz <= StaticSizeT)
	{
		// This expects that we allocated in multiples so that further alignment isn't needed.
		void* ret = (void*)(m_buff + m_used);
		m_used += _sz;
		return ret;
	}
	else
	{
		void* newMem = m_fallback->Alloc(_sz, AlignT);
		memcpy(newMem, m_buff, m_used);
		return newMem;
	}
}

template <size_t StaticSizeT, size_t AlignT>
void* InplaceContainerAllocator<StaticSizeT, AlignT>::ReAlloc(void* _ptr, size_t const _sz)
{
	if (IsPointerLocal(_ptr))
	{
		if (m_used + _sz <= StaticSizeT)
		{
			m_used += _sz;
			return _ptr;
		}
		else
		{
			void* newMem = m_fallback->Alloc(_sz, AlignT);
			memcpy(newMem, m_buff, m_used);
			return newMem;
		}
	}
	else
	{
		return m_fallback->ReAlloc(_ptr, _sz);
	}
}

template <size_t StaticSizeT, size_t AlignT>
void InplaceContainerAllocator<StaticSizeT, AlignT>::Free(void* _ptr)
{
	if (!IsPointerLocal(_ptr))
	{
		m_fallback->Free(_ptr);
	}
}

template <size_t StaticSizeT, size_t AlignT>
bool InplaceContainerAllocator<StaticSizeT, AlignT>::IsPointerLocal(void* _ptr) const
{
	return ((uintptr_t)_ptr >= (uintptr_t)m_buff) && ((uintptr_t)_ptr < (uintptr_t)(m_buff + StaticSizeT));
}

}