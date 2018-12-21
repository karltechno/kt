#include <stdio.h>

#include <kt/kt.h>
#include <kt/Timer.h>
#include "Bench.h"

BenchEntry* g_benchList;

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

	for (BenchEntry* entry = g_benchList; entry; entry = entry->m_next)
	{
		printf("Running benchmark: %s for %u iterations.\n", entry->m_name, entry->m_numIters);
		g_totalAccum = kt::Duration::Zero();
		g_bestIter = kt::Duration::Max();
		g_worstIter = kt::Duration::Zero();
		for (uint32_t i = 0; i < entry->m_numIters; ++i)
		{
			entry->m_fn();
		}

		double const avg = g_totalAccum.Microseconds() / (double)entry->m_numIters;
		printf("Avg: %.2fus, worst: %.2fus, best: %.2fus\n\n", avg, g_worstIter.Microseconds(), g_bestIter.Microseconds());
	}
}