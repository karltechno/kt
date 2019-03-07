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
		_t.PushBack_Raw(numElements);
	}

	if (KT_HAS_TRIVIAL_COPY(T))
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

template <typename K, typename V, typename KeyOps>
void Serialize(ISerializer* _s, kt::HashMap<K, V, KeyOps>& _t)
{
	// Uses internal knowledge of kt::HashMap for speed, keep in check!
	uint32_t numElements;

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

	if (KT_HAS_TRIVIAL_COPY(K) && KT_HAS_TRIVIAL_COPY(V))
	{
		size_t const allocSize = _t.AllocSizeForCapacity(_t.Capacity());
		_s->SerializeBytes(_t.m_data.m_ptr, allocSize);
	}
	else
	{
		if (_s->SerializeMode() == ISerializer::Mode::Write)
		{
			for (kt::HashMap<K, V, KeyOps>::Iterator it = _t.Begin();
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
				K key;
				V val;
				Serialize(_s, &key);
				Serialize(_s, &val);
				_t.Insert(key, val);
			}
		}

	}
}

}