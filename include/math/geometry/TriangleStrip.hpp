#ifndef CPPMATH_TRIANGLE_STRIP_HPP
#define CPPMATH_TRIANGLE_STRIP_HPP

#include "Polygon.hpp"
#include "Intersection.hpp"

namespace math
{
    template <typename T>
    class TriangleStrip : public Polygon<T>
    {
        protected:
            using Polygon<T>::_points;
            using Polygon<T>::_bbox;
            using Polygon<T>::_offset;
            using Polygon<T>::_getSegment;

        public:
            TriangleStrip() {};
            TriangleStrip(size_t size);

            template <typename F>
            auto foreachSegment(F callback) const -> void;

            // NOTE: Only intersections with the individual segments of the
            // polygon will be detected. If a line segment is completely
            // inside the polygon, it won't collide.
            // TODO: Maybe fix this
            auto intersect(const Line2<T>& line) const -> Intersection<T>;
            auto intersect(const Point2<T>& point) const -> bool;
    };
}

#include "TriangleStrip.inl"

#endif
