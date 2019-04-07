#pragma once
#include "kt.h"
#include "Platform.h"
#include "SIMD.h"

#if KT_SSE2
#include <xmmintrin.h>
#else
#error simd not yet implemented
#endif

namespace kt
{

namespace simd
{

struct v4b32
{
	v4b32() = default;

	KT_FORCEINLINE v4b32& operator&=(v4b32 _other);
	KT_FORCEINLINE v4b32& operator^=(v4b32 _other);
	KT_FORCEINLINE v4b32& operator|=(v4b32 _other);

#if KT_SSE2
	KT_FORCEINLINE explicit v4b32(__m128i _xmm) : v(_xmm) {}
	__m128i v;
#endif
};

template <uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE v4b32 Shuf(v4b32 _v);

template <uint8_t A, uint8_t B, uint8_t C, uint8_t D>
KT_FORCEINLINE v4b32 Mask_v4b32();

KT_FORCEINLINE uint32_t MoveMask(v4b32 _v);
KT_FORCEINLINE bool AnyTrue(v4b32 _v);
KT_FORCEINLINE bool AllTrue(v4b32 _v);

KT_FORCEINLINE v4b32 operator==(v4b32 _lhs, v4b32 _rhs);

KT_FORCEINLINE v4b32 operator~(v4b32 _v);

KT_FORCEINLINE v4b32 operator|(v4b32 _lhs, v4b32 _rhs);
KT_FORCEINLINE v4b32 operator&(v4b32 _lhs, v4b32 _rhs);
KT_FORCEINLINE v4b32 operator^(v4b32 _lhs, v4b32 _rhs);


}

}


#if KT_SSE2
	#include "inl/simd_v4b32_sse.inl"
#else
	#error Not yet implemented
#endif
