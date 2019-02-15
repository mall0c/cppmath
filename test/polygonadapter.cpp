#include <cassert>
#include "math/geometry/Polygon.hpp"
#include "math/geometry/OffsetPolygon.hpp"

using namespace math;

class Adapter : public PolygonAdapter<float, OffsetPolygon<float>>
{
    public:
        Adapter(OffsetPolygon<float>& pol);
        virtual ~Adapter() {};

        Point2f get(size_t i) const final;

    protected:
        void _add(const Point2f& point) final;
        void _edit(size_t i, const Point2f& p) final;
        void _insert(size_t i, const Point2f& p) final;
};



int main(int argc, char *argv[])
{
    OffsetPolygon<float> pol;
    Adapter adapter(pol);

    // Test that both are in sync

    for (size_t i = 0; i < 100; ++i)
    {
        pol.add(Point2f(random(), random()));
        assert(pol.get(i) == adapter.get(i) && "Should be identical");
    }

    for (size_t i = 0; i < 100; ++i)
    {
        adapter.edit(i, Point2f(random(), random()));
        assert(pol.get(i) == adapter.get(i) && "Should be identical");
    }

    for (auto i : { NormalBoth, NormalLeft, NormalRight })
    {
        adapter.setNormalDir(i);
        assert(pol.getNormalDir() == adapter.getNormalDir() && pol.getNormalDir() == i && "Should be identical");
    }

    for (auto i : { Open, Closed, Filled })
    {
        pol.setFillType(i);
        assert(pol.getFillType() == adapter.getFillType() && pol.getFillType() == i && "Should be identical");
    }



    return 0;
}



Adapter::Adapter(OffsetPolygon<float>& pol) :
    PolygonAdapter(pol)
{ }

Point2f Adapter::get(size_t i) const
{
    return _pol->get(i);
}

void Adapter::_add(const Point2f& point)
{
    _pol->add(point);
}

void Adapter::_edit(size_t i, const Point2f& p)
{
    _pol->edit(i, p);
}

void Adapter::_insert(size_t i, const Point2f& p)
{
    _pol->insert(i, p);
}

