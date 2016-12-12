#ifndef CPPMATH_FLOAT_HPP
#define CPPMATH_FLOAT_HPP

#include <type_traits>
#include <cmath>
#include "compat.hpp"

#ifndef CPPMATH_FLOAT_TOLERANCE
#define CPPMATH_FLOAT_TOLERANCE 1.0e-4f
#endif

#ifndef CPPMATH_DOUBLE_TOLERANCE
#define CPPMATH_DOUBLE_TOLERANCE 1.0e-12
#endif

namespace math
{
    // This type resolves the resulting type of an operation, e.g.
    // int + float -> float
    // int + int -> int
    // double + int -> double
    // ...
    template <typename T, typename T2>
    using OpResult = decltype(std::declval<T>() + std::declval<T2>());


    // Tolerance values for (floating point) comparison
    template <typename T>
    constexpr T epsilon()
    {
        return std::is_integral<T>::value ? 0 : T();
    }

    template <>
    constexpr float epsilon()
    {
        return CPPMATH_FLOAT_TOLERANCE;
    }

    template <>
    constexpr double epsilon()
    {
        return CPPMATH_DOUBLE_TOLERANCE;
    }


    // Adapted from http://floating-point-gui.de/errors/comparison/
    // template<typename T, typename U>
    template<typename T>
    bool almostEquals(const T& a, const T& b, T tolerance = epsilon<T>())
    {
        // Shortcut for binary equality (also infinites)
        if (a == b)
            return true;

        const T difference = std::abs(a - b);

        // One of the numbers is zero or both are extremely close to it,
        // relative error is meaningless
        if (a == 0 || b == 0 || difference < tolerance)
            return difference < tolerance;

        // Relative error
        return difference / (std::abs(a) + std::abs(b)) < tolerance;
    }
}

#endif
