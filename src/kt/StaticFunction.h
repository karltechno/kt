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

	template <typename FunctorT, typename D = typename std::decay<FunctorT>::type,
		typename = typename std::enable_if<!std::is_same<D, StaticFunction>::value>::type>
	StaticFunction(FunctorT&& _ftor)
	{
		Set(std::forward<FunctorT>(_ftor));
	}

	~StaticFunction()
	{
		Clear();
	}

	StaticFunction(StaticFunction&& _other);
	StaticFunction& operator=(StaticFunction&& _other);

	StaticFunction(StaticFunction const& _other);
	StaticFunction& operator=(StaticFunction const& _other);

	template <typename FunctorT,
		typename D = typename std::decay<FunctorT>::type,
		typename = typename std::enable_if<!std::is_same<D, StaticFunction>::value>::type>
	void Set(FunctorT&& _ftor);

	void Clear();

	ReturnT operator()(ArgsT... _args) const
	{
		KT_ASSERT(m_call);
		return m_call(Storage(), std::forward<ArgsT>(_args)...);
	}

	explicit operator bool() const
	{
		return Valid();
	}

	bool Valid() const
	{
		return m_ops != nullptr;
	}

private:
	enum class Op
	{
		MoveConstruct_FromTo,
		CopyConstruct_FromTo,
		Dtor
	};

	using OpFn = void(*)(Op _op, FunctionType* _this, FunctionType* _other);
	OpFn m_ops = nullptr;

	using CallFn = ReturnT(*)(void*, ArgsT...);
	CallFn m_call = nullptr;

	// Todo: do we ever want more alignment?
	KT_ALIGNAS(16) char m_storage[StorageSizeT];

	void* Storage() const { return const_cast<void*>(reinterpret_cast<void const*>(m_storage)); }
};

}


#include "inl/StaticFunction.inl"