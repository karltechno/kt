#pragma once
#include "../Serialization.h"

namespace kt
{

template <typename T>
void Serialize(ISerializer* _s, kt::Array<T>& _t)
{
	if (_s->SerializeMode() == ISerializer::Mode::Write)
	{
		uint32_t numElements = _t.Size();
		_s->SerializeBytes(&numElements, sizeof(uint32_t));
	}
	else
	{
		uint32_t numElements;
		_s->SerializeBytes(&numElements, sizeof(uint32_t));
		if (KT_IS_POD(T))
		{
			_t.PushBack_Raw(numElements);
		}
		else
		{
			_t.Resize(numElements);
		}
	}

	if (KT_IS_POD(T))
	{
		_s->SerializeBytes(_t.Data(), sizeof(T) * _t.Size());
	}
	else
	{
		for (T& element : _t)
		{
			Serialize(_s, element);
		}
	}
}

template <typename T, typename SerializeOneFn>
void Serialize(ISerializer* _s, Array<T>& _t, SerializeOneFn&& _fn)
{
	if (_s->SerializeMode() == ISerializer::Mode::Write)
	{
		uint32_t numElements = _t.Size();
		_s->SerializeBytes(&numElements, sizeof(uint32_t));
	}
	else
	{
		uint32_t numElements;
		_s->SerializeBytes(&numElements, sizeof(uint32_t));
		_t.Resize(numElements);
	}

	for (T& element : _t)
	{
		_fn(_s, element);
	}
}


template <typename K, typename V, typename KeyOps>
void Serialize(ISerializer* _s, kt::HashMap<K, V, KeyOps>& _t)
{
	// Uses internal knowledge of kt::HashMap for speed, keep in check!
	uint32_t numElements;

	using SerializeHashMap = kt::HashMap<K, V, KeyOps>;

	if (_s->SerializeMode() == ISerializer::Mode::Write)
	{
		numElements = _t.Size();
		_s->SerializeBytes(&numElements, sizeof(uint32_t));
	}
	else
	{
		_s->SerializeBytes(&numElements, sizeof(uint32_t));

		_t.Reserve(numElements);
	}

	using ValueType = typename SerializeHashMap::ValueType;
	using KeyType = typename SerializeHashMap::KeyType;

	if (KT_IS_POD(KeyType) && KT_IS_POD(ValueType))
	{
		size_t const allocSize = _t.AllocSizeForCapacity(_t.Capacity());
		_t.m_data.m_size = numElements;
		_s->SerializeBytes(_t.m_data.m_ptr, allocSize);
	}
	else
	{
		if (_s->SerializeMode() == ISerializer::Mode::Write)
		{
			for (typename SerializeHashMap::Iterator it = _t.Begin();
				 it != _t.End();
				 ++it)
			{
				Serialize(_s, it->m_key);
				Serialize(_s, it->m_val);
			}
		}
		else
		{
			for (uint32_t i = 0; i < numElements; ++i)
			{
				KeyType key;
				ValueType val;
				Serialize(_s, key);
				Serialize(_s, val);
				_t.Insert(key, val);
			}
		}

	}
}

template <uint32_t SizeT>
void Serialize(ISerializer* _s, StaticString<SizeT>& _t)
{
	uint32_t len = _t.Size();
	Serialize(_s, len);
	_t.Resize(len);
	_s->SerializeBytes(_t.Data(), len);
}



}