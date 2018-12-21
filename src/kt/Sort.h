#pragma once
#include "kt.h"

namespace kt
{

template <typename T>
struct LessThanPred
{
	bool operator()(T const& _a, T const& _b) const { return _a < _b; }
};

template <typename T, typename Pred>
void InsertionSort(T _begin, T _end, Pred _p);

template <typename T>
void InsertionSort(T _begin, T _end);

template <typename T, typename Pred>
void QuickSort(T _begin, T _end, Pred _p);

template <typename T>
void QuickSort(T _begin, T _end);

template <typename T, typename T_GetKeyFn>
void RadixSort(T* _begin, T* _end, T* _temp, T_GetKeyFn _getKey);

template <typename T_Key, typename T_Val, typename T_GetKeyFn>
void RadixSort(T_Key* _begin, T_Key* _end, T_Key* _temp, T_Val* _vals, T_Val* _tempVals, T_GetKeyFn _getKey);

template <typename T, typename T_Val>
void RadixSort(T* _begin, T* _end, T* _temp, T_Val* _vals, T_Val* _tempVals);

template <typename T, typename Pred>
bool IsSorted(T _begin, T _end, Pred _p);

template <typename T>
bool IsSorted(T _begin, T _end);

}

#include "inl/Sort.inl"