#include <stdio.h>

#include <kt/kt.h>
#include <kt/Timer.h>
#include <kt/Memory.h>
#include <kt/Strings.h>
#include <kt/Sort.h>
#include "Bench.h"

BenchEntry* g_benchList;
uint32_t g_numBenches = 0;

static kt::TimePoint g_thisIter;

static kt::Duration g_totalAccum;
static kt::Duration g_bestIter;
static kt::Duration g_worstIter;

namespace kt
{


void BenchStartIteration()
{
	g_thisIter = kt::TimePoint::Now();
}
void BenchEndIteration()
{
	kt::Duration const diff = kt::TimePoint::Now() - g_thisIter;
	g_totalAccum += diff;
	g_bestIter = kt::Min(diff, g_bestIter);
	g_worstIter = kt::Max(diff, g_worstIter);
}
}

int main(int argc, char** argv)
{
	KT_UNUSED2(argc, argv);

	if (!g_numBenches)
	{
		return 0;
	}

	BenchEntry** sortedEntries = (BenchEntry**)KT_ALLOCA(sizeof(BenchEntry*) * g_numBenches);

	BenchEntry* head = g_benchList;

	for (uint32_t i = 0; i < g_numBenches; ++i)
	{
		sortedEntries[i] = head;
		head = head->m_next;
	}

	kt::QuickSort(sortedEntries, sortedEntries + g_numBenches,
				  [](BenchEntry const* _lhs, BenchEntry const* _rhs) { return kt::StrCmp(_lhs->m_group, _rhs->m_group) < 0; });

	char const* prevStr = nullptr;

	for (uint32_t i = 0; i < g_numBenches; ++i)
	{
		BenchEntry const* entry = sortedEntries[i];

		if (!prevStr || kt::StrCmpI(entry->m_group, prevStr) != 0)
		{
			if (prevStr)
			{
				printf("\n\n");
			}

			prevStr = entry->m_group;
			printf("----------------------------------------------------------\n");
			printf("Benchmarking group: %s\n", entry->m_group);
			printf("----------------------------------------------------------\n");
		}

		printf("%s for %u iterations:     ", entry->m_name, entry->m_numIters);
		g_totalAccum = kt::Duration::Zero();
		g_bestIter = kt::Duration::Max();
		g_worstIter = kt::Duration::Zero();
		for (uint32_t iter = 0; iter < entry->m_numIters; ++iter)
		{
			entry->m_fn();
		}

		double const avg = g_totalAccum.Microseconds() / (double)entry->m_numIters;
		printf("Avg: %.2fus, worst: %.2fus, best: %.2fus\n", avg, g_worstIter.Microseconds(), g_bestIter.Microseconds());
	}
}