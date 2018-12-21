#pragma once
#include "../Sort.h"
#include <string.h>

namespace kt
{

template <typename T>
void InsertionSort(T _begin, T _end)
{
	InsertionSort(_begin, _end, LessThanPred<decltype(*_begin)>{});
}

template <typename T, typename Pred>
void InsertionSort(T _begin, T _end, Pred _p)
{
	uint32_t const num = (uint32_t)(_end - _begin);
	for (uint32_t i = 1; i < num; ++i)
	{
		for (uint32_t j = i; j > 0; --j)
		{
			if (_p(*(_begin + j - 1), *(_begin + j)))
			{
				break;
			}

			Swap(*(_begin + j - 1), *(_begin + j));
		}
	}
}

template <typename T, typename Pred>
bool IsSorted(T _begin, T _end, Pred _p)
{
	if (_begin == _end)
	{
		return true;
	}

	for (T next = _begin; ++next != _end; ++_begin)
	{
		if (!_p(*_begin, *next))
		{
			return false;
		}
	}

	return true;
}

template <typename T>
bool IsSorted(T _begin, T _end)
{
	typedef decltype(*_begin) Type;

	return IsSorted(_begin, _end, [](Type const& _a, Type const& _b) { return _a <= _b; });
}

static constexpr uint32_t s_RadixWordSize = 8u;
static constexpr uint32_t s_RadixWordMask = (1u << s_RadixWordSize) - 1u;

template <typename T>
struct RadixSortTraits
{
	typedef T BitType;
	static BitType ValueToBit(T const _t) { return _t; }
};

template <>
struct RadixSortTraits<int8_t>
{
	typedef uint32_t BitType;
	static BitType ValueToBit(int8_t const _t) { return (BitType)(_t ^ 0x80); }
};

template <>
struct RadixSortTraits<int16_t>
{
	typedef uint32_t BitType;
	static BitType ValueToBit(int16_t const _t) { return (BitType)(_t ^ 0x8000); }
};

template <>
struct RadixSortTraits<int32_t>
{
	typedef uint32_t BitType;
	static BitType ValueToBit(int32_t const _t) { return (BitType)(_t ^ 0x80000000); }
};

template <>
struct RadixSortTraits<int64_t>
{
	typedef uint64_t BitType;
	static BitType ValueToBit(int64_t const _t) { return (BitType)(_t ^ 0x8000000000000000); }
};

template <>
struct RadixSortTraits<float>
{
	typedef uint32_t BitType;

	static BitType ValueToBit(float const _t)
	{
		union 
		{
			float f;
			uint32_t u;
		} un;

		un.f = _t;
		// Always flip sign bit, and flip exponent+mantissa if sign bit is set 
		un.u ^= (-int32_t(un.u >> 31u)) | 0x80000000;

		return un.u; 
	}
};

template <>
struct RadixSortTraits<double>
{
	typedef uint64_t BitType;

	static BitType ValueToBit(float const _t)
	{
		union
		{
			double f;
			uint64_t u;
		} un;

		un.f = _t;
		un.u ^= (-int64_t(un.u >> 63u)) | 0x8000000000000000;

		return un.u;
	}
};

template <typename T>
void RadixSort(T* _begin, T* _end, T* _temp)
{
	return RadixSort(_begin, _end, _temp, [](T const& _t) { return _t; });
}

template <typename T, typename T_Val>
void RadixSort(T* _begin, T* _end, T* _temp, T_Val* _vals, T_Val* _tempVals)
{
	return RadixSort(_begin, _end, _temp, _vals, _tempVals, [](T const& _key) { return _key; });
}

template <typename T, typename T_GetKeyFn>
void RadixSort(T* _begin, T* _end, T* _temp, T_GetKeyFn _getKey)
{
	uint32_t const numKeys = (uint32_t)(_end - _begin);
	T* tempEnd = _temp + numKeys;

	typedef RadixSortTraits<decltype(_getKey(*_begin))> Traits;

	uint32_t histogram[1u << s_RadixWordSize];

	uint32_t const passes = (sizeof(T) * 8u) / s_RadixWordSize; 

	uint32_t pass = 0u;

	T* currentKeyBegin = _begin;
	T* currentKeyEnd = _end;

	T* otherKeyBegin = _temp;

	for (; pass < passes; ++pass)
	{
		uint32_t const shift = (pass * s_RadixWordSize);
		memset(histogram, 0, sizeof(histogram));

		// Build histogram
		{
			T* it = currentKeyBegin;

			bool sorted = true;

			typename Traits::BitType prevVal = Traits::ValueToBit(_getKey(*it++));
			histogram[(prevVal >> shift) & s_RadixWordMask]++;

			while (it != currentKeyEnd)
			{
				typename Traits::BitType val = Traits::ValueToBit(_getKey(*it++));
				histogram[(val >> shift) & s_RadixWordMask]++;

				sorted &= val >= prevVal;
				prevVal = val;
			}

			if (sorted)
			{
				break;
			}
		}

		// Convert histogram to offsets
		{
			uint32_t accumHist = 0;

			for (uint32_t i = 0; i < (1u << s_RadixWordSize); ++i)
			{
				uint32_t const thisHist = histogram[i];
				histogram[i] = accumHist;
				accumHist += thisHist;
			}
		}

		// Sort pass
		{
			T* it = currentKeyBegin;

			while (it != currentKeyEnd)
			{
				typename Traits::BitType bitVal = Traits::ValueToBit(_getKey(*it));
				uint32_t const destIdx = histogram[(bitVal >> shift) & s_RadixWordMask]++;
				otherKeyBegin[destIdx] = *it++;
			}

			if (pass & 1u)
			{
				currentKeyBegin = _begin;
				currentKeyEnd = _end;
				otherKeyBegin = _temp;
			}
			else
			{
				currentKeyBegin = _temp;
				currentKeyEnd = tempEnd;
				otherKeyBegin = _begin;
			}
		}
	}

	if (pass & 0x1)
	{
		memcpy(_begin, _temp, numKeys * sizeof(T));
	}
}

template <typename T_Key, typename T_Val, typename T_GetKeyFn>
void RadixSort(T_Key* _begin, T_Key* _end, T_Key* _temp, T_Val* _vals, T_Val* _tempVals, T_GetKeyFn _getKey)
{
	uint32_t const numKeys = (uint32_t)(_end - _begin);
	T_Key* tempEnd = _temp + numKeys;

	typedef RadixSortTraits<decltype(_getKey(*_begin))> Traits;
	uint32_t histogram[1u << s_RadixWordSize];

	uint32_t const passes = (sizeof(T_Key) * 8u) / s_RadixWordSize;

	uint32_t pass = 0u;

	T_Key* currentKeyBegin = _begin;
	T_Key* currentKeyEnd = _end;
	T_Val* currentValBegin = _vals;

	T_Key* otherKeyBegin = _temp;
	T_Val* otherValBegin = _tempVals;

	for (; pass < passes; ++pass)
	{
		uint32_t const shift = (pass * s_RadixWordSize);
		memset(histogram, 0, sizeof(histogram));

		// Build histogram
		{
			T_Key* it = currentKeyBegin;

			bool sorted = true;

			typename Traits::BitType prevVal = Traits::ValueToBit(_getKey(*it++));
			histogram[(prevVal >> shift) & s_RadixWordMask]++;

			while (it != currentKeyEnd)
			{
				typename Traits::BitType const val = Traits::ValueToBit(_getKey(*it++));
				histogram[(val >> shift) & s_RadixWordMask]++;

				sorted &= val >= prevVal;
				prevVal = val;
			}

			if (sorted)
			{
				break;
			}
		}

		// Convert histogram to offsets
		{
			uint32_t accumHist = 0;

			for (uint32_t i = 0; i < (1u << s_RadixWordSize); ++i)
			{
				uint32_t const thisHist = histogram[i];
				histogram[i] = accumHist;
				accumHist += thisHist;
			}
		}

		// Do a sort pass
		{
			T_Key* it = currentKeyBegin;

			T_Val* valIt = currentValBegin;

			while (it != currentKeyEnd)
			{
				typename Traits::BitType bitVal = Traits::ValueToBit(_getKey(*it));
				uint32_t const destIdx = histogram[(bitVal >> shift) & s_RadixWordMask]++;
				otherKeyBegin[destIdx] = *it++;
				otherValBegin[destIdx] = std::move(*valIt++);
			}

			if (pass & 1u)
			{
				currentKeyBegin = _begin;
				currentKeyEnd = _end;
				currentValBegin = _vals;

				otherKeyBegin = _temp;
				otherValBegin = _tempVals;
			}
			else
			{
				currentKeyBegin = _temp;
				currentKeyEnd = tempEnd;
				currentValBegin = _tempVals;

				otherValBegin = _vals;
				otherKeyBegin = _begin;
			}
		}
	}

	if (pass & 0x1)
	{
		memcpy(_begin, _temp, numKeys * sizeof(T_Key));
		memcpy(_vals, _tempVals, numKeys * sizeof(T_Val));
	}
}

template <typename T>
void QuickSort(T _begin, T _end)
{
	return QuickSort(_begin, _end, LessThanPred<decltype(*_begin)>{});
}

// Todo: Intro sort / (swap to heap sort after recursion threshold)
template <typename T, typename Pred>
void QuickSort(T _begin, T _end, Pred _p)
{
	uint32_t const num = (uint32_t)(_end - _begin);

	if (num < 2)
	{
		return; 
	}
	else if (num <= 16u)
	{
		InsertionSort(_begin, _end, _p);
	}
	else
	{
		uint32_t const midIdx = num / 2u;

		T iters[3] = {_begin, _begin + midIdx, _end - 1};

		if (!_p(*iters[0], *iters[1]))
		{
			Swap(iters[0], iters[1]);
		}
		if (!_p(*iters[0], *iters[2]))
		{
			Swap(iters[0], iters[2]);
		}
		if (!_p(*iters[1], *iters[2]))
		{
			Swap(iters[1], iters[2]);
		}

		// Move pivot to the end
		T pivotIter = _end - 1;
		Swap(*iters[1], *pivotIter);

		uint32_t swapIdx = 0u;

		T it = _begin;
		while (it != pivotIter)
		{
			if (_p(*it, *pivotIter))
			{
				Swap(*it, *(_begin + swapIdx++));
			}
			++it;
		}

		Swap(*pivotIter, *(_begin + swapIdx));

		QuickSort(_begin, _begin + swapIdx, _p);
		QuickSort(_begin + swapIdx, _end, _p);
	}
}



}