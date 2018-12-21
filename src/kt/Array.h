#pragma once
#include "kt.h"
#include "Memory.h"

namespace kt
{

template <typename T>
class Array
{
public:
	// Explicit copy function must be used. See 'MakeCopy'.
	KT_NO_COPY(Array);

	typedef T* Iterator;
	typedef T const* ConstIterator;

	typedef T ValueType;

	// Default construct taking an Allocator instance.
	Array(kt::IAllocator* _alloc = GetDefaultAllocator());
	~Array();

	// Move constructor and assignment.
	Array(Array&& _other);
	Array& operator=(Array&& _other);

	// Reserve capacity up to _sz.
	void Reserve(uint32_t const _sz);

	// Resize to _sz. Destroys or default constructs elements when _sz is less than or greater than current size.
	void Resize(uint32_t const _sz);

	// Construct and append new element.
	template <typename... Args>
	ValueType& PushBack(Args&&... _args);

	// Allocate and return uninitialized memory for one element.
	ValueType* PushBack_Raw();

	// Allocate and return uninitialized memory for _count elements.
	ValueType* PushBack_Raw(uint32_t const _count);

	// Insert _num elements.
	void Insert(ValueType const* _array, uint32_t const _num);

	// Find an element. Returns End if not found.
	template <typename T_RelatedType>
	Iterator Find(T_RelatedType const& _t);

	// Find an element. Returns End if not found.
	template <typename T_RelatedType>
	ConstIterator Find(T_RelatedType const& _t) const;

	// Erase from index. Keeps the remaining of elements in order.
	Iterator Erase(uint32_t const _idx);

	// Erase from iterator. Swaps last element in place of _idx.
	Iterator EraseSwap(uint32_t const _idx);

	// Erase from iterator. Keeps the remaining of elements in order.
	Iterator Erase(Iterator _it);

	// Erase from iterator. Swaps last element in place of _it.
	Iterator EraseSwap(Iterator _it);

	// Remove the last element. Undefined if Array is empty.
	void PopBack();

	// Make a copy of the array with the current allocator.
	Array<T> MakeCopy() const;

	// Make a copy of the array with a new allocator.
	Array<T> MakeCopy(IAllocator* _newAllocator) const;

	// Indexing operator. Undefined if _idx is out of bounds.
	ValueType& operator[](uint32_t const _idx);

	// Indexing operator. Undefined if _idx is out of bounds.
	ValueType const& operator[](uint32_t const _idx) const;

	// First element in array or nullptr when empty.
	Iterator Begin();
	ConstIterator Begin() const;

	// One past the last element in the array.
	Iterator End();
	ConstIterator End() const;

	// First element in the array. Undefined when array is empty.
	ValueType& Front();

	// Last element in the array. Undefined when array is empty.
	ValueType& Back();

	// Current array size.
	uint32_t Size() const;

	// Current array capacity.
	uint32_t Capacity() const;

	// Pointer to internal data. Null when empty.
	ValueType const* Data() const;
	ValueType* Data();

	// Destroy and set size to 0.
	void Clear();

	// Destroy, set size to 0 and free memory.
	void ClearAndFree();

	// The allocator used for this array.
	kt::IAllocator* GetAllocator() const { return m_allocator; }

	// Set the Arrays allocator. If it is the same, do nothing, else copy the current contents with the new allocator.
	void SetAllocator(IAllocator* _allocator);

private:
	static const uint32_t s_minSize = 4u;

	bool IsIteratorInRange(ConstIterator _it) const;

	void EnsureCapacity(uint32_t const _sz);

	kt::IAllocator* m_allocator;
	T* m_data = nullptr;
	uint32_t m_size = 0u;
	uint32_t m_capacity = 0u;
};

template <typename T, uint32_t InplaceCapacityT>
class InplaceArray : public Array<T>
{
public:
	InplaceArray(IAllocator* _fallback = GetDefaultAllocator())
		: Array<T>(&m_allocator) // Array is initialized before m_allocator, but it does not allocate until m_allocator is initialized.
		, m_allocator(_fallback)
	{
		Reserve(InplaceCapacityT);
	}

private:
	InplaceContainerAllocator<sizeof(T) * InplaceCapacityT, KT_ALIGNOF(T)> m_allocator;
};

}

#include "inl/Array.inl"