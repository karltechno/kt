#pragma once
#include "kt.h"
#include "Memory.h"
#include "Hash.h"
#include "HashMapDeclare.h"

namespace kt
{
struct ISerializer;

// A linear probing/closed addressing hashtable. Uses round robin hashing.
template <typename T_Key, typename T_Value, typename T_KeyOps = HashMap_KeyOps<T_Key>>
class HashMap
{
public:
	// Explicit copy function must be used. See 'MakeCopy'.
	KT_NO_COPY(HashMap);

	static constexpr float c_maxLoadFactor = 0.75f;

	template <typename K, typename V, typename KeyOps>
	friend void Serialize(ISerializer* _s, kt::HashMap<K, V, KeyOps>& _t);

	using ValueType = T_Value;;
	using KeyType = T_Key;
	using IndexType = uint32_t;
	using KeyOps = T_KeyOps;
	using HashType = typename KeyOps::HashType;

	using HashMapType = HashMap<T_Key, T_Value, T_KeyOps>;

	struct KvPair
	{
		KeyType m_key;
		ValueType m_val;
	};

	class IteratorBase
	{
	public:
		friend class HashMap;

		IteratorBase(HashMap* _map, IndexType _idx)
			: m_map(_map), m_idx(_idx)
		{
		}

		IteratorBase& operator++();
		IteratorBase operator++(int);

		bool operator==(IteratorBase const& _other) const { return m_map == _other.m_map && m_idx == _other.m_idx; }
		bool operator!=(IteratorBase const& _other) const { return !(*this == _other); }

	protected:
		HashMap* m_map = nullptr;
		IndexType m_idx;
	};

	class ConstIterator : public IteratorBase
	{
	public:
		ConstIterator(HashMap* _map, IndexType _idx)
			: IteratorBase(_map, _idx)
		{
		}

		ConstIterator(IteratorBase const& _base)
			: IteratorBase(_base)
		{}

		KvPair const* operator->() const;

		KvPair const& operator*() const;
	};

	class Iterator : public IteratorBase
	{
	public:
		Iterator(HashMap* _map, IndexType _idx)
			: IteratorBase(_map, _idx)
		{
		}

		Iterator(ConstIterator& _const)
			: IteratorBase(_const)
		{
		}

		KvPair* operator->() const;

		KvPair& operator*() const;
	};

	// Default constructor with allocator.
	HashMap(IAllocator* alloc = GetDefaultAllocator())
		: m_allocator(alloc)
	{}

	~HashMap();

	// Move construction and assignment.
	HashMap(HashMap&& _other);
	HashMap& operator=(HashMap&& _other);

	// Insert or mutate value if current key is already in map.
	Iterator Insert(KeyType const& _key, ValueType const& _value);

	// Begin iterator.
	Iterator Begin();
	ConstIterator Begin() const;

	// One past end iterator.
	Iterator End();
	ConstIterator End() const;

	// Find value for key, or return End.
	Iterator Find(KeyType const& _key);
	ConstIterator Find(KeyType const& _key) const;

	// Erase value at iterator.
	Iterator Erase(Iterator const& _it);

	// Get Value for Key, or create a new default entry.
	ValueType& operator[](KeyType const& _key);

	// Current HashMap size.
	uint32_t Size() const;

	// Current HashMap capacity.
	uint32_t Capacity() const;

	// Is this HashMap empty?
	bool Empty() const;

	// Reserve space for _sz elements.
	void Reserve(uint32_t const _sz);

	// Clear all elements.
	void Clear();

	// Make a copy with new allocator.
	HashMapType MakeCopy(IAllocator* _newAllocator) const;
	
	// Make a copy with existing allocator.
	HashMapType MakeCopy() const;

	// Get load factor (size / capacity)
	float GetLoadFactor() const;

	// The internal allocator.
	IAllocator* GetAllocator() const { return m_allocator; }

	// Set the HashMaps allocator. If it is the same, do nothing, else copy the current contents with the new allocator.
	void SetAllocator(IAllocator* _allocator);

private:
	static constexpr uint32_t c_defaultCapacity = 16u;
	static constexpr uint32_t c_invalidIndex = UINT32_MAX;

	IAllocator* m_allocator;
	
	struct MapData
	{
		void* m_ptr = nullptr;
		uint32_t m_capacity = 0;
		uint32_t m_size = 0;

		HashType* HashPtr() const;
		KvPair* KvPairPtr() const;
	} m_data;

	static MapData AllocMapData(IAllocator* _allocator, uint32_t _capacity);
	static void FreeMapData(MapData& _data, IAllocator* _allocator);
	static size_t AllocSizeForCapacity(uint32_t _capacity);

	bool IsIndexValid(IndexType const _idx) const;

	void Release();

	KT_NO_INLINE void GrowCapacityAndRehash(uint32_t const newCap = 0u);

	IndexType InsertInternal(HashType const _hash, T_Key& _key, T_Value& _val, bool _setValIfAlreadyInMap = true);

	IndexType RemoveIndex(IndexType const _idx);

	IndexType FindIndex(T_Key const& _key);
	IndexType FindIndex(T_Key const& _key, HashType const _hash);

	HashType HashKey(T_Key const& _key);
	IndexType DesiredHashPos(HashType const _hash);
	uint32_t CalcProbeDistance(HashType const _hash, IndexType const _idx);

	IndexType GetNextIndex(IndexType const _idx) const;
	IndexType FirstFreeIndex() const;
};


}

#include "inl/HashMap.inl"