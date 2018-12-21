#pragma once
#include "kt.h"
#include "Array.h"
#include "HashMap.h"

namespace kt
{

template <typename T_Key, typename T_Value, typename T_KeyOps = HashMap_KeyOps<T_Key>>
class ArrayHashMap
{
public:
	KT_NO_COPY(ArrayHashMap);

	using KeyType = T_Key;
	using KeyOps = T_KeyOps;
	using ValueType = T_Value;
	using ArrayIndexType = uint32_t;

	using ArrayMapType = ArrayHashMap<KeyType, ValueType, KeyOps>;
	using ArrayType = Array<ValueType>;
	using HashMapType = HashMap<KeyType, ArrayIndexType, HashMap_KeyOps<T_Key>>;

	using ValueIterator = ValueType*;
	using ValueConstIterator = ValueType*;

	struct KeyIterator : private HashMapType::Iterator
	{
		friend class ArrayHashMap;

		KeyIterator(typename HashMapType::Iterator it)
			: HashMapType::Iterator(it) {}

		using HashMapType::Iterator::operator ++;

		bool operator==(KeyIterator const& _other) const { return HashMapType::Iterator::operator ==(_other); }
		bool operator!=(KeyIterator const& _other) const { return HashMapType::Iterator::operator !=(_other); }

		KeyType const& Key() const { return HashMapType::Iterator::operator*().m_key; }

	private:
		ArrayIndexType InternalGetIndex() const { return this->operator*().m_val; }
	};

	struct KeyConstIterator : private HashMapType::ConstIterator
	{
		friend class ArrayHashMap;

		KeyConstIterator(typename HashMapType::ConstIterator it)
			: HashMapType::ConstIterator(it) {}

		KeyConstIterator(KeyIterator const& _constIt)
			: HashMapType::ConstIterator(_constIt) {}

		using HashMapType::ConstIterator::operator ++;

		bool operator==(KeyConstIterator const& _other) const { return HashMapType::ConstIterator::operator ==(_other); }
		bool operator!=(KeyConstIterator const& _other) const { return HashMapType::ConstIterator::operator !=(_other); }

		KeyType const& Key() const { return this->operator*.m_key; }

	private:
		ArrayIndexType InternalGetIndex() const { return this->operator*.m_val; }
	};

	// Default constructor with allocator.
	ArrayHashMap(IAllocator* alloc = GetDefaultAllocator())
		: m_array(alloc)
		, m_map(alloc)
	{
	}

	~ArrayHashMap() = default;

	// Move construction and assignment.
	ArrayHashMap(ArrayHashMap&& _other);
	ArrayHashMap& operator=(ArrayHashMap&& _other);

	// Insert or mutate value if current key is already in map.
	ValueIterator Insert(KeyType const& _key, ValueType const& _value);

	// Begin iterator for underlying values.
	ValueIterator BeginValues();
	ValueConstIterator BeginValues() const;

	// One past end iterator for underlying values.
	ValueIterator EndValues();
	ValueConstIterator EndValues() const;

	// Begin iterator for underlying keys.
	KeyIterator BeginKeys();
	KeyConstIterator BeginKeys() const;

	// One past end iterator for underlying keys.
	KeyIterator EndKeys();
	KeyConstIterator EndKeys() const;

	// Find value for key, or return End.
	ValueIterator Find(KeyType const& _key);
	ValueConstIterator Find(KeyType const& _key) const;

	// Find a key and return the key iterator.
	KeyIterator FindKeyIt(KeyType const& _key);
	KeyConstIterator FindKeyIt(KeyType const& _key) const;

	ValueIterator KeyToValueIt(KeyIterator const& _keyIt);
	ValueConstIterator KeyToValueIt(KeyConstIterator const& _keyIt) const;

	// Erase with a key iterator. Fairly slow.
	KeyIterator Erase(KeyIterator _keyIt);

	// Get Value for Key, or create a new default entry.
	ValueType& operator[](KeyType const& _key);

	// Current HashMap size.
	uint32_t Size() const;

	// Reserve space for _sz elements.
	void Reserve(uint32_t const _sz);

	// Clear all elements.
	void Clear();

	// Make a copy with new allocator.
	ArrayMapType MakeCopy(IAllocator* _newAllocator) const;

	// Make a copy with existing allocator.
	ArrayMapType MakeCopy() const;

	// Get load factor (size / capacity)
	float GetLoadFactor() const;

	// The internal allocator.
	IAllocator* GetAllocator() const { return m_array.GetAllocator(); }

	// Set the hashmaps allocator. If it is the same, do nothing, else copy the current contents with the new allocator.
	void SetAllocator(IAllocator* _allocator);

private:
	HashMap<KeyType, ArrayIndexType, KeyOps> m_map;
	Array<T_Value> m_array;
};



}

#include "inl/ArrayHashMap.inl"