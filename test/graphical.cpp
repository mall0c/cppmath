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
#define NUM_OBJECTS 12

using namespace std;
using namespace geometry;

typedef Vec2f VecType;
typedef AABBf AABBType;
typedef Point2f PointType;
typedef Line2f LineT;
typedef Intersection<float> IsecType;

class Shape
{
    public:
        enum Shapes
        {
            Line,
            Ray,        // Only used for setCursor()
            Segment,    // Only used for setCursor()
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
            LineT line;
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

    std::vector<Shape> shapes(NUM_OBJECTS + 1);
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
                        setCursor(cursor, Shape::Segment);
                        break;

                    case sf::Keyboard::Num4:
                        setCursor(cursor, Shape::AABB);
                        break;

                    default:
                        if (cursor.type != Shape::AABB)
                        {
                            switch (ev.key.code)
                            {
                                case sf::Keyboard::H:
                                    cursor.line.d.fill(-100, 0);
                                    break;

                                case sf::Keyboard::J:
                                    cursor.line.d.fill(0, 100);
                                    break;

                                case sf::Keyboard::K:
                                    cursor.line.d.fill(0, -100);
                                    break;

                                case sf::Keyboard::L:
                                    cursor.line.d.fill(100, 0);
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
    for (size_t i = 1; i <= NUM_OBJECTS; ++i)
    {
        if (i % 4 == 3)
        {
            vec[i].type = Shape::AABB;
            vec[i].aabb = AABBType(rand() % WIDTH, rand() % HEIGHT,
                    50 + rand() % 200, 50 + rand() % 200);
        }
        else
        {
            vec[i].type = Shape::Line;
            vec[i].line = LineT(PointType(rand() % WIDTH, rand() % HEIGHT),
                    VecType::fromDirection(rand() % 300, rand() % 360),
                    static_cast<LineType>(i % 4));
        }
    }
}

void setCursor(Shape& cursor, Shape::Shapes type)
{
    // Positions should be all aligned at the same memory offset
    PointType old = cursor.p;

    cursor.type = (type == Shape::AABB) ? Shape::AABB : Shape::Line;
    switch (type)
    {
        case Shape::Line:
            cursor.line = LineT(old, VecType(1, 0));
            break;

        case Shape::Ray:
            cursor.line = LineT(old, VecType(1, 0), Ray);
            break;

        case Shape::Segment:
            cursor.line = LineT(old, VecType(100, 0), Segment);
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
    {
        if (line.type == LineType::Ray)
            drawLine(target, line.p, line.p + line.d * 1000);
        else if (line.type == LineType::Line)
            drawLine(target, line.p - line.d * 1000, line.p + line.d * 1000);
        else if (line.type == LineType::Segment)
            drawLine(target, line.p, line.p + line.d);
    }
}

void Shape::renderCollision(sf::RenderTarget& target, const Shape& shape) const
{
    IsecType isec;

    if (type == Line)
    {
        if (shape.type == Line)
            isec = line.intersect(shape.line);
        else
            isec = line.intersect(shape.aabb);
    }
    else
    {
        if (shape.type == Line)
            isec = shape.line.intersect(aabb);
        else
            isec = aabb.intersect(shape.aabb);
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
