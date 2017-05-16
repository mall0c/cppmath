#ifndef CPPMATH_LINE_STRIP_HPP
#define CPPMATH_LINE_STRIP_HPP

#include "Polygon.hpp"
#include "Intersection.hpp"

namespace math
{
    template <typename T>
    class LineStrip : public Polygon<T>
    {
        protected:
            using Polygon<T>::_points;
            using Polygon<T>::_bbox;
            using Polygon<T>::_offset;
            using Polygon<T>::_getSegment;

        public:
            LineStrip() {};
            LineStrip(size_t size);

            // Callback signature: (const Line2<T>&) -> void
            template <typename F>
            auto foreachSegment(F callback) const -> void;

            auto intersect(const Line2<T>& line) const -> Intersection<T>;
            auto intersect(const Point2<T>& point) const -> bool;
    };
}

#include "LineStrip.inl"

#endif
