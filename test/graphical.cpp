#include <iostream>
#include <SFML/Graphics.hpp>
#include "math/geometry/Line2.hpp"
#include "math/geometry/AABB.hpp"
#include <time.h>
#include <cstdlib>
#include <vector>

#define WIDTH 640
#define HEIGHT 480
#define SPEED 5

using namespace std;
using namespace geometry;

typedef Vec2f VecType;
typedef AABBf AABBType;
typedef Point2f PointType;
typedef Line2f LineType;
typedef Ray2f RayType;
typedef Intersection<float> IsecType;

class Shape
{
    public:
        enum Shapes
        {
            Line,
            Ray,
            AABB
        };

    public:
        Shape() : p() {}
        void render(sf::RenderTarget& target) const;
        void renderCollision(sf::RenderTarget& target, const Shape& shape) const;

    public:
        Shapes type;
        union {
            PointType p;
            VecType pos;
            LineType line;
            RayType ray;
            AABBType aabb;
        };
};


void create(std::vector<Shape>& vec);
void setCursor(Shape& cursor, Shape::Shapes type);

void drawRect(sf::RenderTarget& target, const AABBType& aabb, sf::Color col = sf::Color(131, 148, 150));
void drawLine(sf::RenderTarget& target, const PointType& p1, const PointType& p2, sf::Color col = sf::Color(131, 148, 150));
void drawPoint(sf::RenderTarget& target, const PointType& p, sf::Color c = sf::Color::Red);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "test", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    std::vector<Shape> shapes(11);
    create(shapes);

    Shape& cursor = shapes[0];
    setCursor(cursor, Shape::Line);

    sf::Event ev;
    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
            if (!window.hasFocus())
                continue;

            if (ev.type == sf::Event::KeyPressed)
            {
                switch (ev.key.code)
                {
                    case sf::Keyboard::Q:
                        window.close();
                        break;

                    case sf::Keyboard::R:
                        create(shapes);
                        break;

                    case sf::Keyboard::W:
                        cursor.pos.y -= SPEED;
                        break;

                    case sf::Keyboard::S:
                        cursor.pos.y += SPEED;
                        break;

                    case sf::Keyboard::A:
                        cursor.pos.x -= SPEED;
                        break;

                    case sf::Keyboard::D:
                        cursor.pos.x += SPEED;
                        break;

                    case sf::Keyboard::Num1:
                        setCursor(cursor, Shape::Line);
                        break;

                    case sf::Keyboard::Num2:
                        setCursor(cursor, Shape::Ray);
                        break;

                    case sf::Keyboard::Num3:
                        setCursor(cursor, Shape::AABB);
                        break;

                    default:
                        if (cursor.type != Shape::AABB)
                        {
                            switch (ev.key.code)
                            {
                                case sf::Keyboard::H:
                                    cursor.line.d.fill(-2, 0);
                                    break;

                                case sf::Keyboard::J:
                                    cursor.line.d.fill(0, 2);
                                    break;

                                case sf::Keyboard::K:
                                    cursor.line.d.fill(0, -2);
                                    break;

                                case sf::Keyboard::L:
                                    cursor.line.d.fill(2, 0);
                                    break;
                            }
                        }
                        break;
                }
            }
            else if (ev.type == sf::Event::MouseMoved &&
                    cursor.type != Shape::AABB)
            {
                cursor.line.d.x = ev.mouseMove.x - cursor.line.p.x;
                cursor.line.d.y = ev.mouseMove.y - cursor.line.p.y;
            }
        }

        window.clear(sf::Color(0, 43, 54));

        cursor.render(window);
        for (size_t i = 1; i < shapes.size(); ++i)
        {
            shapes[i].render(window);
            cursor.renderCollision(window, shapes[i]);
        }

        drawPoint(window, cursor.p, sf::Color::Blue);

        window.display();
    }

    return 0;
}

void create(std::vector<Shape>& vec)
{
    for (size_t i = 1; i <= 10; ++i)
    {
        // vec[i] = Shape();
        switch (i % 3)
        {
            case 0:
                vec[i].type = Shape::Line;
                vec[i].line = LineType(PointType(rand() % WIDTH, rand() % HEIGHT),
                        VecType::fromDirection(10, rand() % 360));
                break;

            case 1:
                vec[i].type = Shape::Ray;
                vec[i].ray = RayType(PointType(rand() % WIDTH, rand() % HEIGHT),
                        VecType::fromDirection(10, rand() % 360));
                break;

            case 2:
                vec[i].type = Shape::AABB;
                vec[i].aabb = AABBType(rand() % WIDTH, rand() % HEIGHT,
                        rand() % 200, rand() % 200);
                break;
        }
    }
}

void setCursor(Shape& cursor, Shape::Shapes type)
{
    // Positions should be all aligned at the same memory offset
    PointType old = cursor.p;

    cursor.type = type;
    switch (type)
    {
        case Shape::Line:
            cursor.line = LineType(old, VecType(1, 1));
            break;

        case Shape::Ray:
            cursor.ray = RayType(old, VecType(1, 1));
            break;

        case Shape::AABB:
            cursor.aabb = AABBType(old.x, old.y, 100, 70);
            break;
    }
}


void Shape::render(sf::RenderTarget& target) const
{
    if (type == AABB)
        drawRect(target, aabb);
    else
        drawLine(target,
                (type == Ray) ? (ray.p) : (line.p - line.d * 1000),
                line.p + line.d * 1000);
}

void Shape::renderCollision(sf::RenderTarget& target, const Shape& shape) const
{
    IsecType isec;

    switch (type)
    {
        case Line:
            switch (shape.type)
            {
                case Line:
                    isec = line.intersect(shape.line);
                    break;
                case Ray:
                    isec = line.intersect(shape.ray);
                    break;
                case AABB:
                    isec = line.intersect(shape.aabb);
                    break;
            }
            break;

        case Ray:
            switch (shape.type)
            {
                case Line:
                    isec = ray.intersect(shape.line);
                    break;
                case Ray:
                    isec = ray.intersect(shape.ray);
                    break;
                case AABB:
                    isec = ray.intersect(shape.aabb);
                    break;
            }
            break;

        case AABB:
            switch (shape.type)
            {
                case Line:
                    isec = shape.line.intersect(aabb);
                    break;
                case Ray:
                    isec = shape.ray.intersect(aabb);
                    break;
                case AABB:
                    isec = aabb.intersect(shape.aabb);
                    break;
            }
            break;
    }

    if (isec)
    {
        if (isec.type == LinexLine)
        {
            drawPoint(target, isec.p);
        }
        else if (isec.type == AABBxAABB)
        {
            AABBType rect(aabb);
            if (abs(isec.delta.x) < abs(isec.delta.y))
                rect.pos.x -= isec.delta.x;
            else
                rect.pos.y -= isec.delta.y;
            drawRect(target, rect, sf::Color::Magenta);
        }
        else if (isec.type == LinexAABB)
        {
            const PointType& p1 = isec.seg.p;
            PointType p2 = p1 + isec.seg.d;

            drawPoint(target, p1);
            drawLine(target, p1, p2, sf::Color::Magenta);
        }
    }
}

void drawRect(sf::RenderTarget& target, const AABBType& aabb, sf::Color col)
{
    sf::RectangleShape c(sf::Vector2f(aabb.size.x, aabb.size.y));
    c.setPosition(aabb.pos.x, aabb.pos.y);
    c.setFillColor(sf::Color::Transparent);
    c.setOutlineColor(col);
    c.setOutlineThickness(1);
    target.draw(c);
}

void drawLine(sf::RenderTarget& target, const PointType& p1, const PointType& p2, sf::Color col)
{
    sf::Vertex v[] = {
        sf::Vertex(sf::Vector2f(p1.x, p1.y), col),
        sf::Vertex(sf::Vector2f(p2.x, p2.y), col)
    };
    target.draw(v, 2, sf::Lines);
}

void drawPoint(sf::RenderTarget& target, const PointType& p, sf::Color col)
{
    sf::CircleShape c(3);
    c.setOrigin(3, 3);
    c.setPosition(p.x, p.y);
    c.setFillColor(col);
    target.draw(c);
}
