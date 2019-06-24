#pragma once
#include "kt.h"

namespace kt
{

template <typename T>
struct SliceConstHelper
{
	static bool const IsConst = false;

	using ConstIterator = T const*;
	using Iterator = T*;

	using Reference = T&;
	using ConstReference = T const&;

	using NonConstT = T;
	using ConstT = const T;
};

template <typename T>
struct SliceConstHelper<const T>
{
	static bool const IsConst = true;

	using ConstIterator = T const*;
	using Iterator = T const*;

	using Reference = T const&;
	using ConstReference = T const&;

	using NonConstT = T;
	using ConstT = const T;
};

template <typename T>
struct Slice
{
	Slice() = default;

	using Iterator			= typename SliceConstHelper<T>::Iterator;
	using ConstIterator		= typename SliceConstHelper<T>::ConstIterator;

	using Reference			= typename SliceConstHelper<T>::Reference;
	using ConstReference	= typename SliceConstHelper<T>::ConstReference;

	template <size_t CArraySizeT>
	Slice(T(&_cArray)[CArraySizeT])
	{
		m_begin = &_cArray[0];
		m_end = m_begin + CArraySizeT;
	}
	
	Slice(T& _elem)
		: m_begin(&_elem)
		, m_end((&_elem) + 1)
	{}
	
	Slice(T* _begin, T* _end)
		: m_begin(_begin)
		, m_end(_end)
	{
		KT_ASSERT(_end >= _begin);
	}

	Slice(T* _begin, uint32_t _count)
		: m_begin(_begin)
		, m_end(_begin + _count)
	{
	}

	Iterator Begin()
	{
		return m_begin;
	}
	
	Iterator End()
	{
		return m_end;
	}

	ConstIterator Begin() const
	{
		return m_begin;
	}

	ConstIterator End() const
	{
		return m_end;
	}

	ConstIterator Data() const
	{
		return m_begin;
	}

	Iterator Data()
	{
		return m_begin;
	}

	uint32_t Size() const
	{
		KT_ASSERT(m_end >= m_begin);
		return uint32_t(m_end - m_begin);
	}

	bool Empty() const
	{
		return Size() == 0;
	}

	void Clear()
	{
		m_begin = nullptr;
		m_end = nullptr;
	}

	Reference operator[](uint32_t _idx)
	{
		KT_ASSERT(_idx < Size());
		return m_begin[_idx];
	}

	ConstReference operator[](uint32_t _idx) const
	{
		KT_ASSERT(_idx < Size());
		return m_begin[_idx];
	}
private:
	T* m_begin = nullptr;
	T* m_end = nullptr;
};

template <typename T>
kt::Slice<T> MakeSlice(T& _elem)
{
	return kt::Slice<T>(_elem);
}

template <typename T>
kt::Slice<T> MakeSlice(T* _begin, T* _end)
{
	return kt::Slice<T>(_begin, _end);
}

template <typename T>
kt::Slice<T> MakeSlice(T* _begin, uint32_t _count)
{
	return kt::Slice<T>(_begin, _count);
}

template <typename T, size_t CArraySizeT>
kt::Slice<T> MakeSlice(T(&_cArray)[CArraySizeT])
{
	return kt::Slice<T>(_cArray);
}

}