#ifndef CPPMATH_LINE2_HPP
#define CPPMATH_LINE2_HPP

#include "Vector2.hpp"
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
            typedef Intersection<Point2<T> > Point2Intersection;

        public:
            constexpr Line2() {};
            constexpr Line2(const Point2<T>& p_, const Vector2<T>& d_) :
                p(p_), d(d_) {};
            constexpr Line2(const Point2<T>& p1, const Point2<T>& p2) :
                p(p1), d(p2 - p1) {};

        public:
            template <bool isray_other>
            constexpr bool isIdentical(const Line2<T, isray_other>& line) const
            {
                return d.crossAlmostZero(line.d) && (intersect(line.p) || line.intersect(p));
            }

            template <bool isray_other>
            constexpr bool isParallel(const Line2<T, isray_other>& line) const
            {
                return d.crossAlmostZero(line.d) && !intersect(line.p) && (!isray || !isray_other || !line.intersect(p));
            }

            template <bool isray_other>
            Point2Intersection intersect(const Line2<T, isray_other>& line) const
            {
                if (d.crossAlmostZero(line.d)) // parallel or identical
                    return Point2Intersection();

                const T dx = line.p.x - p.x,
                        dy = line.p.y - p.y;
                const double m = d.y * line.d.x - d.x * line.d.y,
                             u = (line.d.x * dy - line.d.y * dx) / m;

                if ((!isray || u >= 0) && (!isray_other || (d.x * dy - d.y * dx) / m >= 0))
                    return Point2Intersection(p + d * u);

                return Point2Intersection();
            }

            constexpr bool intersect(const Point2<T>& p2) const
            {
                // If the area of the parallelogram created by these 2 vectors
                // is 0, then they are identical. This means p2 is on this line.
                return _intersectPointPart(p2 - p);
            }

        private:
            inline bool _intersectPointPart(const Vector2<T>& v) const
            {
                return d.crossAlmostZero(v) && (!isray || d.signs() == v.signs());
            }

        public:
            Point2<T> p; // start point
            Vector2<T> d; // direction (not normalized)
    };

    typedef Line2<float> Line2f;
    typedef Line2<int> Line2i;
}

#endif
