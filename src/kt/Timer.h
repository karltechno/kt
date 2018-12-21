#pragma once
#include "kt.h"

namespace kt
{

struct Duration
{
	explicit Duration(int64_t const _t)
		: m_raw(_t)
	{}

	Duration() = default;

	// This duration as seconds.
	double Seconds() const;

	// This duration as milliseconds.
	double Milliseconds() const;

	// This duration as microseconds.
	double Microseconds() const;

	// Create a duration from seconds.
	static Duration FromSeconds(double _s);

	// Create a duration from milliseconds.
	static Duration FromMilliseconds(double _s);

	// Create a duration as microseconds.
	static Duration FromMicroseconds(double _s);

	// Duration representation of zero.
	static Duration Zero() { return Duration(0); }

	// Max possible duration representation.
	static Duration Max() { return Duration(INT64_MAX); }

	// The raw duration representation. 
	int64_t Raw() const { return m_raw; }

	Duration& operator-=(Duration const _other);
	Duration& operator+=(Duration const _other);
	Duration& operator*=(double const _scale);

private:
	int64_t m_raw = 0;
};

// Represents a time point obtained from OS high frequency timer.
struct TimePoint
{
	explicit TimePoint(int64_t const _t) 
		: m_raw(_t) {}

	TimePoint() = default;

	// The current time from OS high frequency timer.
	static TimePoint Now();

	// Raw tick representation.
	int64_t Raw() const { return m_raw; }

	// Duration between two timers (this - _other).
	Duration TimeSince(TimePoint const _other) { return Duration(Raw() - _other.Raw()); }

private:
	int64_t m_raw = 0;
};

KT_FORCEINLINE bool operator<(TimePoint _lhs, TimePoint _rhs)	{ return _lhs.Raw() < _rhs.Raw(); }
KT_FORCEINLINE bool operator<=(TimePoint _lhs, TimePoint _rhs)	{ return _lhs.Raw() <= _rhs.Raw(); }
KT_FORCEINLINE bool operator>(TimePoint _lhs, TimePoint _rhs)	{ return _lhs.Raw() > _rhs.Raw(); }
KT_FORCEINLINE bool operator>=(TimePoint _lhs, TimePoint _rhs)	{ return _lhs.Raw() >= _rhs.Raw(); }
KT_FORCEINLINE bool operator==(TimePoint _lhs, TimePoint _rhs)	{ return _lhs.Raw() == _rhs.Raw(); }
KT_FORCEINLINE bool operator!=(TimePoint _lhs, TimePoint _rhs)	{ return _lhs.Raw() != _rhs.Raw(); }

KT_FORCEINLINE Duration operator-(TimePoint _lhs, TimePoint _rhs) { return _lhs.TimeSince(_rhs); }

KT_FORCEINLINE bool operator<(Duration _lhs, Duration _rhs)		{ return _lhs.Raw() < _rhs.Raw(); }
KT_FORCEINLINE bool operator<=(Duration _lhs, Duration _rhs)	{ return _lhs.Raw() <= _rhs.Raw(); }
KT_FORCEINLINE bool operator>(Duration _lhs, Duration _rhs)		{ return _lhs.Raw() > _rhs.Raw(); }
KT_FORCEINLINE bool operator>=(Duration _lhs, Duration _rhs)	{ return _lhs.Raw() >= _rhs.Raw(); }
KT_FORCEINLINE bool operator==(Duration _lhs, Duration _rhs)	{ return _lhs.Raw() == _rhs.Raw(); }
KT_FORCEINLINE bool operator!=(Duration _lhs, Duration _rhs)	{ return _lhs.Raw() != _rhs.Raw(); }

KT_FORCEINLINE Duration operator+(Duration _lhs, Duration _rhs) { return Duration(_lhs.Raw() + _rhs.Raw()); }
KT_FORCEINLINE Duration operator-(Duration _lhs, Duration _rhs) { return Duration(_lhs.Raw() - _rhs.Raw()); }
KT_FORCEINLINE Duration operator*(Duration _lhs, double scale)	{ return Duration(int64_t(_lhs.Raw() * scale)); }


}