#pragma once
#include <math.h>

namespace kt
{

template <typename T>
T Lerp(T const _a, T const _b, T const _t)
{
	return (_b - _a) * _t + _a;
}

KT_FORCEINLINE float ToRadians(float const _f)
{
	float constexpr multiplier = (kPi / 180.0f);
	return _f * multiplier;
}

KT_FORCEINLINE float ToDegrees(float const _f)
{
	float constexpr multiplier = (180.0f / kPi);
	return _f * multiplier;
}

KT_FORCEINLINE float Abs(float const _f)
{
	union
	{
		float m_f;
		uint32_t m_u;
	} un;

	un.m_f = _f;
	un.m_u &= 0x7FFFFFFF;
	return un.m_f;
}

KT_FORCEINLINE float Sqrt(float const _f)
{
	return ::sqrtf(_f);
}

KT_FORCEINLINE float Cos(float const _f)
{
	return ::cosf(_f);
}


KT_FORCEINLINE float Sin(float const _f)
{
	return ::sinf(_f);
}

KT_FORCEINLINE float Tan(float const _f)
{
	return ::tanf(_f);
}

KT_FORCEINLINE float Atan(float const _f)
{
	return ::atanf(_f);
}

KT_FORCEINLINE float Atan2(float const _y, float const _x)
{
	return ::atan2f(_y, _x);
}

KT_FORCEINLINE float ACos(float const _f)
{
	return ::acosf(_f);
}

KT_FORCEINLINE float ASin(float const _f)
{
	return ::asinf(_f);
}

}