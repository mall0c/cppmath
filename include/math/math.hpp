#ifndef CPPMATH_MATH_HPP
#define CPPMATH_MATH_HPP

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "compat.hpp"

namespace math
{
    constexpr const double pi = M_PI;

    constexpr double radtodeg(double rad)
    {
        return rad * 180 / pi;
    }

    constexpr double degtorad(double deg)
    {
        return deg * pi / 180;
    }


    double pointDistance(double x1, double y1, double x2, double y2);
    double pointDirection(double x1, double y1, double x2, double y2);


    double lengthdirX(double len, double dir);
    double lengthdirY(double len, double dir);

    double adaptDirection(double dir);

    template <typename T>
    constexpr int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T>
    T clamp(const T& val, const T& min, const T& max)
    {
        return std::max(min, std::min(val, max));
    }
}

#endif

