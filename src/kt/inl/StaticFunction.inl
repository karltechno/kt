#pragma once
#include <type_traits>
#include "../StaticFunction.h"

namespace kt
{

template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
template <typename FunctorT>
StaticFunction<ReturnT(ArgsT...), StorageSizeT>::StaticFunction(FunctorT&& _ftor)
{
	Set(std::move(_ftor));
}

template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
StaticFunction<ReturnT(ArgsT...), StorageSizeT>::StaticFunction(StaticFunction&& _other)
{
	if (_other.m_ops)
	{
		_other.m_ops(Op::MoveConstruct_FromTo, &_other, this);
		m_ops = _other.m_ops;
		m_call = _other.m_call;

		_other.Clear();
	}
}


template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
template <typename FunctorT>
void StaticFunction<ReturnT(ArgsT...), StorageSizeT>::Set(FunctorT&& _ftor)
{
	Clear();

	using FunctorType = typename std::remove_reference<FunctorT>::type;

	static_assert(sizeof(FunctorType) <= sizeof(m_storage), "Functor too big for storage.");

	m_ops = [](Op _op, FunctionType* _this, FunctionType* _other)
	{

		switch (_op)
		{
			case Op::Dtor:
			{
				((FunctorType*)(_this->Storage()))->~FunctorType();
			} break;

			case Op::MoveConstruct_FromTo:
			{
				FunctorType* from = (FunctorType*)(_this->Storage());
				FunctorType* to = (FunctorType*)(_other->Storage());
				kt::PlacementNew<FunctorType>(to, std::move(*from));
			} break;
		}
	};

	m_call = [](void* _storage, ArgsT... _args) -> ReturnT
	{
		FunctorType& ftor = *(FunctorType*)_storage;
		return ftor(std::forward<ArgsT...>(_args)...);
	};
	
	kt::PlacementNew<FunctorType>((FunctorType*)Storage(), std::move(_ftor));
}

template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
void StaticFunction<ReturnT(ArgsT...), StorageSizeT>::Clear()
{
	if (m_ops)
	{
		m_ops(Op::Dtor, this, nullptr);
	}

	m_ops = nullptr;
	m_call = nullptr;
}

}