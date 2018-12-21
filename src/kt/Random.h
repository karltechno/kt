#pragma once
#include "kt.h"

namespace kt
{

struct XorShift32
{
	XorShift32(uint32_t const _s = 254693456u)
		: m_state(_s)
	{
	}

	static constexpr uint32_t MinValue = 1u;
	static constexpr uint32_t MaxValue = 0xFFFFFFFF;

	KT_FORCEINLINE uint32_t Gen32();

	uint32_t m_state;
};

// Returns float in half open range [0, 1)
template <typename T_RNG>
float RandomUnitFloat(T_RNG& _rng);

template <typename T_RNG>
void RandomOnUnitCircleCircumference(T_RNG& _rng, float& o_x, float& o_y);

}

#include "inl/Random.inl"