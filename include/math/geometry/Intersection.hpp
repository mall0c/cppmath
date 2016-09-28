#ifndef CPPMATH_INTERSECTION_HPP
#define CPPMATH_INTERSECTION_HPP

#include "../compat.hpp"

namespace geometry
{
    template <class T>
    class Intersection
    {
        public:
            constexpr Intersection() : _valid(false) {}
            constexpr Intersection(const T& d) : _valid(true), data(d) {}

        public:
            // Prevents "constructing" data
            union {
                T data;
            };

        private:
            bool _valid;

        public:
            constexpr operator bool() const
            {
                return _valid;
            }

            constexpr const T& operator*() const
            {
                return data;
            }

            T& operator*()
            {
                return data;
            }

            constexpr const T* operator->() const
            {
                return &data;
            }

            T* operator->()
            {
                return &data;
            }
    };
}

#endif
