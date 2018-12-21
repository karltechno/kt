#pragma once
#include "kt/MathUtil.h"

namespace kt
{

template <typename T_RNG>
float RandomUnitFloat(T_RNG& _rng)
{
	uint32_t const x = _rng.Gen32();

	union
	{
		float f;
		uint32_t u;
	} un;

	// Set exponent to 2^0 and stuff random bits into mantissa
	un.u = (0x7F << 23) | (x >> 9);
	return un.f - 1.0f;
}

template <typename T_RNG>
void RandomOnUnitCircleCircumference(T_RNG& _rng, float& o_x, float& o_y)
{
	float const theta = RandomUnitFloat(_rng) * kPi * 2.0f;

	o_x = kt::Cos(theta);
	o_y = kt::Sin(theta);
}


KT_FORCEINLINE uint32_t XorShift32::Gen32()
{
	uint32_t x = m_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	m_state = x;
	return x;
}

}