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
    template <class T>
    struct TypeTraits
    {
        constexpr static T epsilon()
        {
            return std::is_integral<T>::value ? 0 : T();
        }
    };

    template <>
    constexpr float TypeTraits<float>::epsilon()
    {
        return CPPMATH_FLOAT_TOLERANCE;
    }

    template <>
    constexpr double TypeTraits<double>::epsilon()
    {
        return CPPMATH_DOUBLE_TOLERANCE;
    }


    // Adapted from http://floating-point-gui.de/errors/comparison/
    template<class T>
    bool almostEquals(const T& a, const T& b, T tolerance = TypeTraits<T>::epsilon())
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
