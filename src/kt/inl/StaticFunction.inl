#pragma once
#include <type_traits>
#include "../StaticFunction.h"

namespace kt
{

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
auto StaticFunction<ReturnT(ArgsT...), StorageSizeT>::operator=(StaticFunction&& _other) -> StaticFunction&
{
	Clear();
	if (_other.m_ops)
	{
		m_ops = _other.m_ops;
		m_call = _other.m_call;
		_other.m_ops(Op::MoveConstruct_FromTo, &_other, this);
		_other.Clear();
	}
	return *this;
}

template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
StaticFunction<ReturnT(ArgsT...), StorageSizeT>::StaticFunction(StaticFunction const& _other)
{
	if (_other.m_ops)
	{
		_other.m_ops(Op::CopyConstruct_FromTo, const_cast<FunctionType*>(&_other), this);
		m_ops = _other.m_ops;
		m_call = _other.m_call;
	}
}

template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
auto StaticFunction<ReturnT(ArgsT...), StorageSizeT>::operator=(StaticFunction const& _other) -> StaticFunction&
{
	Clear();
	if (_other.m_ops)
	{
		_other.m_ops(Op::CopyConstruct_FromTo, const_cast<StaticFunction*>(&_other), this);
		m_ops = _other.m_ops;
		m_call = _other.m_call;
	}
	return *this;
}

template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
template <typename FunctorT, typename D, typename>
void StaticFunction<ReturnT(ArgsT...), StorageSizeT>::Set(FunctorT&& _ftor)
{
	Clear();

	using FunctorType = typename std::decay<FunctorT>::type;

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

			case Op::CopyConstruct_FromTo:
			{
				FunctorType* from = (FunctorType*)(_this->Storage());
				FunctorType* to = (FunctorType*)(_other->Storage());
				kt::PlacementNew<FunctorType>(to, *from);
			} break;
		}
	};

	m_call = [](void* _storage, ArgsT... _args) -> ReturnT
	{
		FunctorType& ftor = *(FunctorType*)_storage;
		return ftor(std::forward<ArgsT...>(_args)...);
	};
	
	kt::PlacementNew<FunctorType>((FunctorType*)Storage(), std::forward<FunctorT>(_ftor));
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