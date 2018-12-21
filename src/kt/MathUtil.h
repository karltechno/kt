#pragma once
#include "kt.h"

namespace kt
{

constexpr float kPi = 3.14159265358979323846f;
constexpr float kPiOverTwo = 1.57079632679489661923f;
constexpr float kPiOverFour = 0.785398163397448309616f;
constexpr float kSqrt2 = 1.41421356237309504880f;
constexpr float kSqrt2Over2 = 0.707106781186547524401f;
constexpr float kE = 2.71828182845904523536f;

KT_FORCEINLINE float ToRadians(float const _f);
KT_FORCEINLINE float ToDegrees(float const _f);

template <typename T>
T Lerp(T const _a, T const _b, T const _t);

KT_FORCEINLINE float Abs(float const _f);

KT_FORCEINLINE float Sqrt(float const _f);
KT_FORCEINLINE float Cos(float const _f);
KT_FORCEINLINE float Sin(float const _f);
KT_FORCEINLINE float Tan(float const _f);
KT_FORCEINLINE float Atan(float const _f);
KT_FORCEINLINE float Atan2(float const _y, float const _x);
KT_FORCEINLINE float ACos(float const _f);
KT_FORCEINLINE float ASin(float const _f);

KT_FORCEINLINE float MaxComponent(float const _f) { return _f; };

}

#include "inl/MathUtil.inl"