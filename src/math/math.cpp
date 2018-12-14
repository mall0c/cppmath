#include "math/math.hpp"
#include "math/geometry/Vector.hpp"

namespace math
{
    double pointDistance(double x1, double y1, double x2, double y2)
    {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    double pointDirection(double x1, double y1, double x2, double y2)
    {
        return (Vec2d(x2, y2) - Vec2d(x1, y1)).angle();
    }


    double lengthdirX(double len, double dir)
    {
        return len * cos(degtorad(dir));
    }

    double lengthdirY(double len, double dir)
    {
        return -len * sin(degtorad(dir));
    }


    double adaptDirection(double dir)
    {
        return (dir < 0 ? 360 : 0) + fmod(dir, 360.0);
    }
}
