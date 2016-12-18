#ifndef CPPMATH_VECTOR_2_HPP
#define CPPMATH_VECTOR_2_HPP

#include "Vector.hpp"

// This file exists only for compatibility reasons.

namespace math
{
    template <typename T>
    using Vector2 = Vector<T, 2>;

    typedef Vec2f Vector2f;
    typedef Vec2i Vector2i;
}

#endif

