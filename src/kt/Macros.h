#pragma once
#include <assert.h>
#include <stddef.h>
#include "Platform.h"


#ifndef KT_DEBUG
	#if (defined (DEBUG) || defined (_DEBUG)) || !(defined (NDEBUG) || defined (_NDEBUG))
		#define KT_DEBUG		(1)
	#else
		#define KT_DEBUG		(0)
	#endif
#endif

#if KT_COMPILER_MSVC
	#define KT_BREAK ::__debugbreak();
#elif (KT_COMPILER_GCC || KT_COMPILER_CLANG) && KT_CPU_X86
	#define KT_BREAK __asm__ volatile("int $0x03");
#endif

#if KT_DEBUG
	#define KT_ASSERT(_x) if(!(_x)) {KT_BREAK;} 
#else
	#define KT_ASSERT(_x) (void)0
#endif

#if (KT_COMPILER_CLANG || KT_COMPILER_GCC)
	#define KT_UNLIKELY(expr) __builtin_expect((expr), 0)
	#define KT_LIKELY(expr) __builtin_expect((expr), 1)
#else
	#define KT_UNLIKELY(expr) expr
	#define KT_LIKELY(expr) expr
#endif

// https://www.g-truc.net/post-0708.html
template <typename T, size_t N>
char(&COUNTOF_REQUIRES_ARRAY_ARGUMENT(T(&)[N]))[N];
#define KT_ARRAY_COUNT(x) sizeof(COUNTOF_REQUIRES_ARRAY_ARGUMENT(x))

namespace kt
{
template <typename T>
struct ScopeExit
{
	ScopeExit(T code)
		: m_code(code)
	{}

	~ScopeExit()
	{
		if (!m_dismissed)
		{
			m_code();
		}
	}

	void Dismiss()
	{
		m_dismissed = true;
	}

	T m_code;
	bool m_dismissed = false;
};

template <typename T>
ScopeExit<T> MakeScopeExit(T _code) { return ScopeExit<T>(_code); }

}

#define _KT_STRING_JOIN_2_(_x, _y) _x ## _y
#define KT_STRING_JOIN(_x, _y) _KT_STRING_JOIN_2_(_x, _y)

#define KT_UNIQUE_IDENTIFIER(_name) KT_STRING_JOIN(_name, __COUNTER__)
#define KT_NOOP void(0)

#define KT_SCOPE_EXIT(_code) auto KT_UNIQUE_IDENTIFIER(scopeExit) = kt::MakeScopeExit([&]{ _code; })

#define KT_UNUSED(_0) ((void)_0);
#define KT_UNUSED2(_0, _1) KT_UNUSED(_0); KT_UNUSED(_1);
#define KT_UNUSED3(_0, _1, _2) KT_UNUSED(_0); KT_UNUSED2(_1, _2);
#define KT_UNUSED4(_0, _1, _2, _3) KT_UNUSED(_0); KT_UNUSED3(_1, _2, _3);
#define KT_UNUSED5(_0, _1, _2, _3, _4) KT_UNUSED(_0); KT_UNUSED4(_1, _2, _3, _4);

#define KT_MACRO_BLOCK_BEGIN do {
#define KT_MACRO_BLOCK_END } while(0);

#if (KT_COMPILER_MSVC || KT_COMPILER_CLANG || KT_COMPILER_GCC)
	#define KT_HAS_TRIVIAL_COPY(_t) __has_trivial_copy(_t)
	#define KT_IS_POD(_t) __is_pod(_t)
	#define KT_HAS_TRIVIAL_DTOR(_t) __has_trivial_destructor(_t)
	#define KT_HAS_TRIVIAL_CTOR(_t) __has_trivial_constructor(_t)
#else
	#error Not implemented
#endif

#define KT_CPP17 (__cplusplus >= 201703L) 

#if KT_COMPILER_MSVC
	#define KT_ALIGNAS(_align) __declspec(align(_align))
	#define KT_ALIGNOF(_align) alignof(_align)
	#define KT_FORCEINLINE __forceinline

	#if KT_CPP17
		#define KT_FALLTHROUGH [[fallthrough]]
	#else
		#define KT_FALLTHROUGH
	#endif

#elif (KT_COMPILER_GCC || KT_COMPILER_CLANG)
	#define KT_FORCEINLINE inline __attribute__((__always_inline__))
	#define KT_ALIGNAS(_align) alignas(_align)
	#define KT_ALIGNOF(_align) alignof(_align)
	#define KT_FALLTHROUGH 
#else
	#error Not implemented.
#endif

#define KT_NO_COPY(_class) _class(_class const&) = delete; _class operator=(_class const& _other) = delete;

