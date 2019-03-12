#ifndef CPPMATH_GEOMETRY_ALGORITHM_HPP
#define CPPMATH_GEOMETRY_ALGORITHM_HPP

namespace math
{
    template <typename>
    class AbstractPointSet;

    // Convert triangle strip formatted point sets to polygons and back.
    // The functions don't check if the input set is actually correctly formatted.
    // Input and output parameters must _not_ be the same.
    template <typename T> void polygonToTriangleStrip(const AbstractPointSet<T>& pol, AbstractPointSet<T>* out);
    template <typename T> void triangleStripToPolygon(const AbstractPointSet<T>& points, AbstractPointSet<T>* out);
}


#include "PointSet.hpp"
#include <cassert>

namespace math
{
    template <typename T>
    void polygonToTriangleStrip(const AbstractPointSet<T>& pol, AbstractPointSet<T>* out)
    {
        assert(out && "out is null");

        if (pol.size() == 0)
            return;

        out->clear();
        out->add(pol.get(0));

        for (size_t i = 1; i < pol.size(); ++i)
        {
            size_t b = pol.size() - i;

            if (b >= i)
                out->add(pol.get(i));

            if (b > i)
                out->add(pol.get(b));
            else
                break;
        }
    }

    template <typename T>
    void triangleStripToPolygon(const AbstractPointSet<T>& points, AbstractPointSet<T>* out)
    {
        assert(out && "out is null");

        if (points.size() == 0)
            return;

        out->clear();
        out->add(points.get(0));
        for (size_t i = 1; i < points.size(); i += 2)
            out->add(points.get(i));

        size_t end = points.size() - 1;
        if (end % 2 == 1)
            --end;

        for (size_t i = end; i != 0; i -= 2)
            out->add(points.get(i));
    }
}

#endif
