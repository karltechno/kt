#pragma once
#include <kt/kt.h>

typedef void(*BenchFn)();

struct BenchEntry
{
	char const* m_name;
	BenchFn m_fn;
	uint32_t m_numIters;

	BenchEntry* m_next;
};

extern BenchEntry* g_benchList;

struct BenchEntryAutoReg
{
	BenchEntryAutoReg(BenchEntry* _entry)
	{
		_entry->m_next = g_benchList;
		g_benchList = _entry;
	}
};

namespace kt
{
void BenchStartIteration();
void BenchEndIteration();

struct ScopedBenchStart
{
	ScopedBenchStart()
	{
		BenchStartIteration();
	}
	~ScopedBenchStart()
	{
		BenchEndIteration();
	}
};
}

#define KT_BENCH(_name, _iters) void _name(); \
static BenchEntry KT_STRING_JOIN(KT_BENCH_ENTRY, _name){ #_name, _name, _iters, nullptr }; \
static BenchEntryAutoReg KT_UNIQUE_IDENTIFIER(KT_BENCH_AUTO_REG_)(&KT_STRING_JOIN(KT_BENCH_ENTRY, _name)); \
void _name() 
