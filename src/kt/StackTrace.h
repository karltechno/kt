#pragma once
#include "kt.h"
#include "Platform.h"
#include "Strings.h"

#define KT_STACK_TRACE_ENABLED (1 && KT_PLATFORM_WINDOWS)

#if KT_COMPILER_MSVC
	#define KT_RETURN_ADDRESS _ReturnAddress();
#elif (KT_COMPILER_CLANG || KT_COMPILER_GCC)
	#define KT_RETURN_ADDRESS __builtin_return_address(0);
#endif

namespace kt
{

static const uint32_t c_maxStackTraceFrames = 12u;

struct StackTrace
{
	void Capture(uint32_t _skip = 0);

	bool ResolveSymbol(uint32_t _idx, kt::String512& o_symbol, uint32_t* o_line = nullptr, kt::String512* o_file = nullptr) const;

	void* m_frames[c_maxStackTraceFrames] = {};
};

}