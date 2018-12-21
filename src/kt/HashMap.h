#pragma once
#include "kt.h"
#include "Memory.h"
#include "Hash.h"

namespace kt
{

template <typename T>
struct HashMap_KeyOps
{
	typedef uint64_t HashType;
	static_assert(KT_IS_POD(T), "HashMap_KeyOps should be specialized for non-POD types.");

	static bool Equal(T const& _lhs, T const& _rhs)
	{
		return _lhs == _rhs;
	}
	
	static HashType Hash(T const& _key)
	{
		return XXHash_64(&_key, sizeof(T));
	}
};

template <>
struct HashMap_KeyOps<char const*>
{
	typedef uint64_t HashType;

	static bool Equal(char const* _lhs, char const* _rhs)
	{
		return StrCmp(_lhs, _rhs) == 0;
	}

	static HashType Hash(char const* _key)
	{
		return XXHash_64(_key, StrLen(_key));
	}
};

template <>
struct HashMap_KeyOps<StringView>
{
	typedef uint64_t HashType; 

	static bool Equal(StringView const& _lhs, StringView const& _rhs)
	{
		return _lhs == _rhs;
	}

	static HashType Hash(StringView const& _view)
	{
		return XXHash_64(_view.Data(), _view.Size());
	}
};

template <uint32_t T_Size>
struct HashMap_KeyOps<StaticString<T_Size>>
{
	typedef uint64_t HashType;

	static bool Equal(StaticString<T_Size> const& _lhs, StaticString<T_Size> const& _rhs)
	{
		return _lhs == _rhs;
	}

	static HashType Hash(StaticString<T_Size> const& _str)
	{
		return XXHash_64(_str.Data(), _str.Size());
	}
};

// A linear probing/closed addressing hashtable. Uses round robin hashing.
// Performance is decent - but could benefit from some further optimisations. Particularly memory layout.
template <typename T_Key, typename T_Value, typename T_KeyOps = HashMap_KeyOps<T_Key>>
class HashMap
{
public:
	// Explicit copy function must be used. See 'MakeCopy'.
	KT_NO_COPY(HashMap);

	static constexpr float c_maxLoadFactor = 0.75f;

	typedef T_Value ValueType;
	typedef T_Key KeyType;
	typedef uint32_t IndexType;
	typedef T_KeyOps KeyOps;
	typedef typename KeyOps::HashType HashType;

	typedef HashMap<T_Key, T_Value, T_KeyOps> HashMapType;

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

	// Insert and set value to _default only if value was not already there, else leave it be.
	Iterator InsertIfMissing(KeyType const& _key, ValueType const& _default, bool* _didNewInsert = nullptr);

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

	// Set the hashmaps allocator. If it is the same, do nothing, else copy the current contents with the new allocator.
	void SetAllocator(IAllocator* _allocator);

private:
	static constexpr uint32_t c_defaultCapacity = 16u;
	static constexpr uint32_t c_invalidIndex = UINT32_MAX;

	struct InternalEntry
	{
		static constexpr int8_t s_maxProbeDist = 127;

		bool IsEmpty() const { return m_probeDist == -1; }
		void SetEmpty() { m_probeDist = -1; }
		void SetProbeDist(uint8_t const _dist) { KT_ASSERT(_dist <= s_maxProbeDist); m_probeDist = _dist; }
		int8_t GetProbeDist() const { return m_probeDist; }

		int8_t m_probeDist;
		KvPair m_kvPair;
	};


	IAllocator* m_allocator;
	
	InternalEntry* m_entries = nullptr;
	uint32_t m_size = 0;
	uint32_t m_capacity = 0;

	bool IsIndexValid(IndexType const _idx) const;

	void Release();

	void GrowCapacityAndRehash(uint32_t const newCap = 0u);

	IndexType InsertInternal(HashType const _hash, T_Key& _key, T_Value& _val);

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