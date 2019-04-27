#pragma once
#if KT_SSE2
	#include <xmmintrin.h>
#else
	#error simd not yet implemented
#endif

#if KT_SSE2
	#define KT_SIMD_V4F32_CONST1(_name, _f)							KT_ALIGNAS(16) static float const _name[4]		= {_f, _f, _f, _f};
	#define KT_SIMD_V4F32_CONST4(_name, _f0, _f1, _f2, _f3)			KT_ALIGNAS(16) static float const _name[4]		= {_f0, _f1, _f2, _f3};
	#define KT_SIMD_V4_BITWISE_CONST1(_name, _u)					KT_ALIGNAS(16) static uint32_t const _name[4]	= {_u, _u, _u, _u};
	#define KT_SIMD_V4_BITWISE_CONST4(_name, _u0, _u1, _u2, _u3)	KT_ALIGNAS(16) static uint32_t const _name[4]	= {_u0, _u1, _u2, _u3};
	#define KT_SIMD_V4F32_LOAD_CONST(_c)							_mm_load_ps((float*)_c)
	#define KT_SIMD_V4B32_LOAD_CONST(_c)							_mm_load_si128((__m128i*)_c)
#endif

namespace kt
{

namespace simd
{

struct v4f32;
struct v4b32;

}

}
