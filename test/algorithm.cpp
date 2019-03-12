#include "math/geometry/algorithm.hpp"
#include <iostream>

using namespace math;
using namespace std;

int main(int argc, char *argv[])
{
    math::PointSet<float> trianglestrip;
    math::PointSet<float> polygon;
    math::PointSet<float> secondtrianglestrip;

    Point2f testpoints[] = {
        Point2f(0, 0),
        Point2f(10, -10),
        Point2f(10, 10),
        Point2f(20, -20),
        Point2f(20, 20),
        Point2f(30, -30),
        Point2f(30, 30),
    };

    for (size_t k = 0; k < sizeof(testpoints) / sizeof(*testpoints); ++k)
    {
        cout<<"testing with "<<k<<" points"<<endl;
        trianglestrip.clear();

        for (size_t i = 0; i < k; ++i)
            trianglestrip.add(testpoints[i]);

        triangleStripToPolygon(trianglestrip, &polygon);
        polygonToTriangleStrip(polygon, &secondtrianglestrip);

        assert(polygon.size() == trianglestrip.size() && "Wrong size");
        assert(polygon.size() == secondtrianglestrip.size() && "Wrong size");

        for (size_t i = 0; i < k; ++i)
            cout<<"polygon "<<i<<": "<<polygon.get(i)<<endl;

        for (size_t i = 0; i < k; ++i)
            cout<<"secondtrianglestrip "<<i<<": "<<secondtrianglestrip.get(i)<<endl;

        for (size_t i = 0; i < k; ++i)
            assert(secondtrianglestrip.get(i) == trianglestrip.get(i));
    }

    return 0;
}
