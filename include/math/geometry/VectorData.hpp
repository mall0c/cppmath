#ifndef CPPMATH_VECTOR_DATA_HPP
#define CPPMATH_VECTOR_DATA_HPP

namespace math
{
    template <typename T, unsigned int N>
    struct VectorData
    {
        T _data[N];
    };

    template <typename T>
    struct VectorData<T, 2>
    {
        union {
            T _data[2];
            struct {
                T x, y;
            };
        };
    };

    template <typename T>
    struct VectorData<T, 3>
    {
        union {
            T _data[3];
            struct { T x, y, z; };
            struct { T r, g, b; };
        };
    };

    template <typename T>
    struct VectorData<T, 4>
    {
        union {
            T _data[4];
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
        };
    };
}

#endif
