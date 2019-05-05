#pragma once
#include "../AABB.h"
#include <type_traits>

namespace kt
{

template <typename T>
Array<T>::Array(IAllocator* _alloc)
	: m_allocator(_alloc)
{
}

template<typename T>
Array<T>::~Array<T>()
{
	ClearAndFree();
}

template<typename T>
auto Array<T>::Data() const -> ValueType const*
{
	return m_data;
}

template <typename T>
void kt::Array<T>::Clear()
{
	if (!KT_HAS_TRIVIAL_DTOR(T))
	{
		T* begin = m_data;
		T* end = m_data + m_size;

		while (begin != end)
		{
			begin->~T();
			++begin;
		}
	}

	m_size = 0;
}


template<typename T>
auto Array<T>::Data() -> ValueType*
{
	return m_data;
}


template <typename T>
auto kt::Array<T>::PushBack_Raw(uint32_t const _count) -> ValueType*
{
	uint32_t const oldSize = m_size;
	if (CapacityNeedsGrow(m_size + _count))
	{
		InternalGrowCapacity(m_size + _count);
	}

	m_size += _count;
	return m_data + oldSize;
}

template <typename T>
auto kt::Array<T>::PushBack_Raw() -> ValueType*
{
	uint32_t const oldSize = m_size;
	if (CapacityNeedsGrow(m_size + 1))
	{
		InternalGrowCapacity(m_size + 1);
	}

	++m_size;
	return m_data + oldSize;
}

template <typename T>
void Array<T>::ClearAndFree()
{
	Clear();

	if (m_data)
	{
		m_allocator->FreeSized(m_data, m_capacity * sizeof(T));
		m_data = nullptr;
		m_size = 0;
		m_capacity = 0;
	}
}


template<typename T>
inline Array<T>::Array(Array&& _other)
	: m_allocator(_other.m_allocator)
	, m_data(_other.m_data)
	, m_size(_other.m_size)
	, m_capacity(_other.m_capacity)
{
	_other.m_data = nullptr;
	_other.m_size = 0u;
	_other.m_capacity = 0u;
}

template <typename T>
kt::Array<T> kt::Array<T>::MakeCopy(IAllocator* _newAllocator) const
{
	kt::Array<T> copy(_newAllocator);
	T* newMem = copy.PushBack_Raw(m_size);

	if (KT_HAS_TRIVIAL_COPY(T))
	{
		memcpy(newMem, m_data, sizeof(T) * m_size);
	}
	else
	{
		for (uint32_t i = 0; i < m_size; ++i)
		{
			PlacementNew(newMem + i, m_data[i]);
		}
	}

	return copy;
}

template <typename T>
kt::Array<T> kt::Array<T>::MakeCopy() const
{
	return MakeCopy(m_allocator);
}

template<typename T>
Array<T>& Array<T>::operator=(Array<T>&& _other)
{
	ClearAndFree();
	
	m_data = _other.m_data;
	m_size = _other.m_size;
	m_capacity = _other.m_capacity;
	m_allocator = _other.m_allocator;

	_other.m_data = nullptr;
	_other.m_size = 0;
	_other.m_capacity = 0;

	return *this;
}

template<typename T>
void Array<T>::SetAllocator(IAllocator* _allocator)
{
	if (_allocator != m_allocator)
	{
		if (m_size == 0)
		{
			ClearAndFree();
			m_allocator = _allocator;
		}
		else
		{
			*this = std::move(MakeCopy(_allocator));
		}
	}
}

template<typename T>
inline void Array<T>::Reserve(uint32_t const _sz)
{
	if (CapacityNeedsGrow(_sz))
	{
		InternalGrowCapacity(_sz);
	}
}

template<typename T>
inline void Array<T>::Resize(uint32_t const _sz)
{
	if (_sz > m_size)
	{
		if (CapacityNeedsGrow(_sz))
		{
			InternalGrowCapacity(_sz);
		}

		if (!KT_HAS_TRIVIAL_CTOR(T))
		{
			T* begin = m_data + m_size;
			T* end = m_data + _sz;

			while (begin != end)
			{
				PlacementNew(begin++);
			}
		}

		m_size = _sz;
	}
	else if (_sz < m_size)
	{
		if (!KT_HAS_TRIVIAL_DTOR(T))
		{
			T* begin = m_data + _sz;
			T* end = m_data + m_size;

			while (begin != end)
			{
				begin->~T();
				++begin;
			}
		}

		m_size = _sz;
	}
}


template <typename T>
template <typename... Args>
auto Array<T>::PushBack(Args&&... _args) -> ValueType&
{
	// Todo: guard against reference to previous buffer during realloc...
	if (CapacityNeedsGrow(m_size + 1u))
	{
		InternalGrowCapacity(m_size + 1u);
	}

	return *PlacementNew(m_data + m_size++, std::forward<Args>(_args)...);
}

template <typename T>
KT_NO_INLINE void Array<T>::InternalGrowCapacity(uint32_t const _minimumCapacity)
{
	KT_ASSERT(m_capacity < _minimumCapacity);
	uint32_t const newCap = Max(Max(m_capacity + m_capacity / 2u, s_minSize), _minimumCapacity);

	if (KT_HAS_TRIVIAL_COPY(T))
	{
		// Realloc = malloc when m_data = nullptr.
		m_data = (T*)m_allocator->ReAllocSized(m_data, m_capacity * sizeof(T), newCap * sizeof(T), KT_ALIGNOF(T));
	}
	else
	{
		T* newData = (T*)m_allocator->Alloc(newCap * sizeof(T), KT_ALIGNOF(T));

		T* newItr = newData;
		T* begin = m_data;
		T* end = m_data + m_size;

		while (begin != end)
		{
			PlacementNew(newItr++, std::move(*begin));
			begin->~T();
			++begin;
		}

		if (m_data)
		{
			m_allocator->FreeSized(m_data, m_capacity * sizeof(T));
		}

		m_data = newData;
	}

	m_capacity = newCap;
}

template<typename T>
auto Array<T>::operator[](uint32_t const _idx) -> ValueType&
{
	KT_ASSERT(_idx < m_size);
	return m_data[_idx];
}

template<typename T>
auto Array<T>::operator[](uint32_t const _idx) const -> ValueType const&
{
	KT_ASSERT(_idx < m_size);
	return m_data[_idx];
}

template<typename T>
auto Array<T>::Begin() -> Iterator
{
	return m_data;
}

template<typename T>
auto Array<T>::End() -> Iterator
{
	return m_data + m_size;
}

template<typename T>
auto Array<T>::Begin() const -> ConstIterator
{
	return m_data;
}

template<typename T>
auto Array<T>::End() const -> ConstIterator
{
	return m_data + m_size;
}

template<typename T>
uint32_t Array<T>::Size() const
{
	return m_size;
}

template <typename T>
uint32_t Array<T>::Capacity() const
{
	return m_capacity;
}

template <typename T>
auto Array<T>::EraseSwap(Iterator _it) -> Iterator
{
	KT_ASSERT(IsIteratorInRange(_it));

	T* endMinusOne = m_data + m_size - 1u;
	if (_it != endMinusOne)
	{
		*_it = std::move(*endMinusOne);
		endMinusOne->~T();
		--m_size;
		return _it;
	}
	else
	{
		_it->~T();
		--m_size;
		return m_data + m_size;
	}
}

template <typename T>
auto kt::Array<T>::Erase(uint32_t const _idx) -> Iterator
{
	return Erase(Begin() + _idx);
}

template <typename T>
auto kt::Array<T>::EraseSwap(uint32_t const _idx) -> Iterator
{
	return EraseSwap(Begin() + _idx);
}

template <typename T>
bool Array<T>::IsIteratorInRange(ConstIterator _it) const
{
	return (uintptr_t)_it >= (uintptr_t)m_data && (uintptr_t)_it < (uintptr_t)(m_data + m_size);
}

template <typename T>
auto Array<T>::Erase(Iterator _it) -> Iterator
{
	KT_ASSERT(IsIteratorInRange(_it));
	
	if (_it + 1 == m_data + m_size)
	{
		_it->~T();
		return m_data + --m_size;
	}
	
	Iterator originalIt = _it;

	Iterator end = m_data + m_size;

	for (Iterator next = _it; ++next != end; ++_it)
	{
		*_it = std::move(*next);
	}

	_it->~T();

	--m_size;
	return originalIt;
}

template <typename T>
template <typename T_RelatedType>
auto Array<T>::Find(T_RelatedType const& _t) -> Iterator
{
	for (Iterator it = Begin(); it != End(); ++it)
	{
		if (*it == _t)
		{
			return it;
		}
	}
	return End();
}

template <typename T>
template <typename T_RelatedType>
auto Array<T>::Find(T_RelatedType const& _t) const -> ConstIterator
{
	for (ConstIterator it = Begin(); it != End(); ++it)
	{
		if (*it == _t)
		{
			return it;
		}
	}
	return End();
}

template <typename T>
auto Array<T>::Back() -> ValueType&
{
	KT_ASSERT(m_size);

	return m_data[m_size - 1];
}

template <typename T>
auto Array<T>::Front() -> ValueType&
{
	KT_ASSERT(m_size);

	return *m_data;
}

template <typename T>
void Array<T>::PopBack()
{
	KT_ASSERT(m_size);

	m_data[--m_size].~T();
}


template <typename T>
bool kt::Array<T>::CapacityNeedsGrow(uint32_t _cap) const
{
	return _cap > m_capacity;
}


template <typename T>
void Array<T>::Insert(ValueType const* _array, uint32_t const _num)
{
	if (CapacityNeedsGrow(m_size + _num))
	{
		InternalGrowCapacity(m_size + _num);
	}

	for (uint32_t i = 0; i < _num; ++i)
	{
		PlacementNew(&m_data[m_size++], _array[i]);
	}
}


}