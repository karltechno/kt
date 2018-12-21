#include "Timer.h"

#if KT_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif KT_PLATFORM_POSIX
	#include <time.h>
#endif



namespace kt
{

static int64_t s_perfFreq;
static int64_t s_startupTime;

#if KT_PLATFORM_WINDOWS
struct QueryFreqBeforeMain
{
	QueryFreqBeforeMain()
	{
		LARGE_INTEGER l;
		::QueryPerformanceFrequency(&l);
		s_perfFreq = l.QuadPart;
		::QueryPerformanceCounter(&l);
		s_startupTime = l.QuadPart;
	}
} queryFreqBeforeMain;

#endif


// https://gist.github.com/jspohr/3dc4f00033d79ec5bdaf67bc46c813e3
static int64_t int64_MulDiv(int64_t value, int64_t numer, int64_t denom)
{
	int64_t q = value / denom;
	int64_t r = value % denom;
	return q * numer + r * numer / denom;
}


TimePoint TimePoint::Now()
{
#if KT_PLATFORM_WINDOWS
	LARGE_INTEGER largeInt;
	::QueryPerformanceCounter(&largeInt);
	return TimePoint(int64_MulDiv(largeInt.QuadPart, 1000000, s_perfFreq));
#elif KT_PLATFORM_POSIX
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((int64_t)ts.tv_sec * 1000000000 + (int64_t)ts.tv_nsec);
#endif
}


double Duration::Seconds() const
{
	return (double)m_raw / 1000000.0;
}

double Duration::Milliseconds() const
{
	return (double)m_raw / 1000.0;
}

double Duration::Microseconds() const
{
	return (double)m_raw;
}

Duration Duration::FromSeconds(double _s)
{
	return Duration(int64_t(_s * 1000000.0));
}

Duration Duration::FromMilliseconds(double _s)
{
	return Duration(int64_t(_s * 1000.0));
}

Duration Duration::FromMicroseconds(double _s)
{
	return Duration(int64_t(_s));
}

Duration& Duration::operator-=(Duration const _other)
{
	*this = (*this - _other);
	return *this;
}

Duration& Duration::operator+=(Duration const _other)
{
	*this = (*this + _other);
	return *this;
}

Duration& Duration::operator*=(double const _scale)
{
	*this = (*this * _scale);
	return *this;
}

}