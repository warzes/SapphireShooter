#pragma once

namespace math
{
	constexpr float RoundingError = 0.000001f;    // 1.0e-6 (alt. FLT_EPSILON)

    // Returns true if A and B are equal.
    template <typename T> inline bool Equal(const T& A, const T& B)
    {
        return A == B;
    }
    // Returns true if A and B are equal with the tolerance of math::RoundingError. This is a specialized template function for floats.
    template <> inline bool Equal(const float& A, const float& B)
    {
        return fabs(A - B) < RoundingError;
    }
} // namespace math