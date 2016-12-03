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

typedef Point2<float> Pointf;

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
        Shape() {}
        void render(sf::RenderTarget& target) const;
        void renderCollision(sf::RenderTarget& target, const Shape& shape) const;

    public:
        Shapes type;
        union {
            Vec2f pos;
            Line2f line;
            Ray2f ray;
            AABBf aabb;
        };
};


void create(std::vector<Shape>& vec);
void setCursor(Shape& cursor, Shape::Shapes type);


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

    sf::CircleShape c(3);
    c.setOrigin(3, 3);

    sf::Event ev;
    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
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
                }
            }
        }

        if (cursor.type != Shape::AABB)
        {
            cursor.line.d.x = sf::Mouse::getPosition(window).x - cursor.line.p.x;
            cursor.line.d.y = sf::Mouse::getPosition(window).y - cursor.line.p.y;
        }

        window.clear();

        for (size_t i = 0; i < shapes.size(); ++i)
        {
            shapes[i].render(window);

            if (i != 0)
                cursor.renderCollision(window, shapes[i]);
        }

        c.setPosition(cursor.pos.x, cursor.pos.y);
        c.setFillColor(sf::Color::Blue);
        window.draw(c);

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
                vec[i].line = Line2f(Pointf(rand() % WIDTH, rand() % HEIGHT),
                        Vec2f::fromDirection(1, rand() % 360));
                break;

            case 1:
                vec[i].type = Shape::Ray;
                vec[i].ray = Ray2f(Pointf(rand() % WIDTH, rand() % HEIGHT),
                        Vec2f::fromDirection(1, rand() % 360));
                break;

            case 2:
                vec[i].type = Shape::AABB;
                vec[i].aabb = AABBf(rand() % WIDTH, rand() % HEIGHT,
                        rand() % 200, rand() % 200);
                break;
        }
    }
}

void setCursor(Shape& cursor, Shape::Shapes type)
{
    // Positions should be all aligned at the same memory offset
    cursor.type = type;
    switch (type)
    {
        case Shape::Line:
            cursor.line = Line2f(Pointf(0, 0), Vec2f(1, 1));
            break;

        case Shape::Ray:
            cursor.ray = Ray2f(Pointf(0, 0), Vec2f(1, 1));
            break;

        case Shape::AABB:
            cursor.aabb = AABBf(0, 0, 100, 70);
            break;
    }
}


void Shape::render(sf::RenderTarget& target) const
{
    if (type == Ray || type == Line)
    {
        Pointf start, stop;
        if (type == Ray)
            start = ray.p;
        else
            start = line.p - line.d * 1000;

        stop = line.p + line.d * 1000;

        sf::Vertex v[] = {
            sf::Vertex(sf::Vector2f(start.x, start.y)),
            sf::Vertex(sf::Vector2f(stop.x, stop.y))
        };
        target.draw(v, 2, sf::Lines);
    }
    else
    {
        sf::Vertex v[] = {
            sf::Vertex(sf::Vector2f(aabb.pos.x, aabb.pos.y)),
            sf::Vertex(sf::Vector2f(aabb.pos.x + aabb.size.x, aabb.pos.y)),
            sf::Vertex(sf::Vector2f(aabb.pos.x + aabb.size.x, aabb.pos.y + aabb.size.y)),
            sf::Vertex(sf::Vector2f(aabb.pos.x, aabb.pos.y + aabb.size.y)),
            sf::Vertex(sf::Vector2f(aabb.pos.x, aabb.pos.y)),
        };
        target.draw(v, 5, sf::LineStrip);
    }
}

void Shape::renderCollision(sf::RenderTarget& target, const Shape& shape) const
{
    Intersection<float> isec;

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
                    // isec = line.intersect(shape.aabb);
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
                    // isec = ray.intersect(shape.aabb);
                    break;
            }
            break;

        case AABB:
            switch (shape.type)
            {
                // case Line:
                //     isec = aabb.intersect(shape.line);
                //     break;
                // case Ray:
                //     isec = aabb.intersect(shape.ray);
                //     break;
                case AABB:
                    isec = aabb.intersect(shape.aabb);
                    break;
            }
            break;
    }

    if (isec)
    {
        if (isec.type == Intersection<float>::LinexLine)
        {
            sf::CircleShape c(3);
            c.setOrigin(3, 3);
            c.setPosition(isec.p.x, isec.p.y);
            c.setFillColor(sf::Color::Red);
            target.draw(c);
        }
        else if (isec.type == Intersection<float>::AABBxAABB)
        {
            // sf::Vertex v[] = {
            //     sf::Vertex(sf::Vector2f(pos.x, pos.y)),
            //     sf::Vertex(sf::Vector2f(pos.x - isec.delta.x, pos.y - isec.delta.y))
            // };
            // target.draw(v, 2, sf::Lines);
            sf::RectangleShape c(sf::Vector2f(aabb.size.x, aabb.size.y));
            c.setPosition(pos.x, pos.y);
            if (abs(isec.delta.x) < abs(isec.delta.y))
                c.move(-isec.delta.x, 0);
            else
                c.move(0, -isec.delta.y);
            c.setFillColor(sf::Color::Red);
            target.draw(c);
        }
    }
}
