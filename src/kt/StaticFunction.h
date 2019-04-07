#pragma once
#include "kt.h"
#include "Memory.h"

namespace kt
{

template <typename ReturnArgsT, uint32_t StorageT>
struct StaticFunction;

template <uint32_t StorageSizeT, typename ReturnT, typename... ArgsT>
struct StaticFunction<ReturnT(ArgsT...), StorageSizeT>
{
	using FunctionType = StaticFunction<ReturnT(ArgsT...), StorageSizeT>;

	StaticFunction() = default;

	template <typename FunctorT>
	StaticFunction(FunctorT&& _ftor);

	~StaticFunction()
	{
		Clear();
	}

	StaticFunction(StaticFunction&& _other);

	StaticFunction& operator=(StaticFunction&& _other)
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

	template <typename FunctorT>
	void Set(FunctorT&& _ftor);

	void Clear();

	ReturnT operator()(ArgsT&&... _args)
	{
		KT_ASSERT(m_call);
		return m_call(m_storage, std::forward<ArgsT...>(_args)...);
	}

	explicit operator bool() const
	{
		return m_ops != nullptr;
	}

private:
	enum class Op
	{
		MoveConstruct_FromTo,
		Dtor
	};

	using OpFn = void(*)(Op _op, FunctionType* _this, FunctionType* _other);
	OpFn m_ops = nullptr;

	using CallFn = ReturnT(*)(void*, ArgsT...);
	CallFn m_call = nullptr;

	// Todo: do we ever want more alignment?
	KT_ALIGNAS(16) char m_storage[StorageSizeT];

	void* Storage() { return static_cast<void*>(m_storage); }
};

}


#include "inl/StaticFunction.inl"