#include "StackTrace.h"
#include "Concurrency.h"
#include "Strings.h"

#if KT_PLATFORM_WINDOWS && KT_STACK_TRACE_ENABLED
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib,"dbghelp.lib")

struct WindowsStackTraceStaticState
{
	void Init()
	{
		if (m_init)
		{
			return;
		}
		::SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
		::SymInitialize(::GetCurrentProcess(), nullptr, TRUE);
	}

	~WindowsStackTraceStaticState()
	{
		if (m_init)
		{
			::SymCleanup(::GetCurrentProcess());
		}
	}

	bool m_init = false;
} s_state;

#endif

namespace kt
{



void StackTrace::Capture(uint32_t _skip)
{
	memset(m_frames, 0, sizeof(m_frames));

#if KT_STACK_TRACE_ENABLED

#if KT_PLATFORM_WINDOWS
	::CaptureStackBackTrace(1 + _skip, c_maxStackTraceFrames, m_frames, nullptr);
#endif

#endif
}


bool StackTrace::ResolveSymbol(uint32_t _idx, kt::String512& o_symbol, uint32_t* o_line, kt::String512* o_file) const
{
#if KT_STACK_TRACE_ENABLED

#if KT_PLATFORM_WINDOWS
	s_state.Init();

	KT_ASSERT(_idx < c_maxStackTraceFrames);

	if (!m_frames[_idx])
	{
		return false;
	}

	DWORD64 rip = (DWORD64)m_frames[_idx];

	KT_ALIGNAS(KT_ALIGNOF(IMAGEHLP_SYMBOL64)) char buff[sizeof(SYMBOL_INFO) + o_symbol.c_capacity];

	SYMBOL_INFO* psym = (SYMBOL_INFO*)buff;
	psym->SizeOfStruct = sizeof(SYMBOL_INFO);
	psym->MaxNameLen = o_symbol.c_capacity;
	
	DWORD64 disp = 0;
	HANDLE proc = ::GetCurrentProcess();

	if (::SymFromAddr(proc, rip, &disp, psym))
	{
		o_symbol = kt::StringView{ psym->Name, psym->NameLen };

		if (!(o_line || o_file))
		{
			return true;
		}

		IMAGEHLP_LINE64 line = {};
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		DWORD disp2 = 0;
		if (::SymGetLineFromAddr64(proc, rip, &disp2, &line))
		{
			if (o_line)
			{
				*o_line = line.LineNumber;
			}

			if (o_file)
			{
				*o_file = kt::String512(line.FileName);
			}
			return true;
		}
		return false;
	}
#endif
	return false;
}

#else
	return false;
#endif

}