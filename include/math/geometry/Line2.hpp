#ifndef CPPMATH_LINE2_HPP
#define CPPMATH_LINE2_HPP

#include "Vector.hpp"
#include "Point2.hpp"
#include "Intersection.hpp"
#include "../TypeTraits.hpp"
#include "../compat.hpp"

namespace geometry
{
    template <class T, bool isray = false>
    class Line2
    {
        public:
            Line2() {};
            Line2(const Point2<T>& p_, const Vec2<T>& d_);
            Line2(const Point2<T>& p1, const Point2<T>& p2);

        public:
            // TODO: Fix this. Until then, leave it disabled.
            // template <bool isray_other>
            // auto isIdentical(const Line2<T, isray_other>& line) const -> bool;

            template <bool isray_other>
            auto isParallel(const Line2<T, isray_other>& line) const -> bool;

            template <bool isray_other>
            auto intersect(const Line2<T, isray_other>& line) const -> Intersection<T>;

            auto intersect(const Point2<T>& p2) const -> bool;

        private:
            auto _intersectPointPart(const Vec2<T>& v) const -> bool;

        public:
            Point2<T> p; // start point
            Vec2<T> d; // direction (not normalized)
    };

    typedef Line2<float> Line2f;
    typedef Line2<int> Line2i;
    typedef Line2<float, true> Ray2f;
    typedef Line2<int, true> Ray2i;
}

#include "Line2.inl"

#endif
