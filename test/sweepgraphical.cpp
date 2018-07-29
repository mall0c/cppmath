#include <iostream>
#include <SFML/Graphics.hpp>
#include "math/geometry/Line2.hpp"
#include "math/geometry/AABB.hpp"
#include <time.h>
#include <cstdlib>
#include <vector>
#include <cassert>

#define WIDTH 640
#define HEIGHT 480
#define SPEED 5
#define NUM_OBJECTS 12

using namespace std;
using namespace math;

typedef Vec2f VecT;
typedef AABBf AABBT;
typedef Point2f PointT;
typedef Line2f LineT;
typedef Intersection<float> IsecT;

void create(std::vector<LineT>& vec);

void drawRect(sf::RenderTarget& target, const AABBT& aabb, sf::Color col = sf::Color(131, 148, 150));
void drawLine(sf::RenderTarget& target, const PointT& p1, const PointT& p2, sf::Color col = sf::Color(131, 148, 150));
void drawPoint(sf::RenderTarget& target, const PointT& p, sf::Color c = sf::Color::Red);


int main(int argc, char *argv[])
{
    srand(time(NULL));

    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "test", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    AABBT aabb(0, 0, 100, 70);
    VecT speed;
    std::vector<LineT> shapes(NUM_OBJECTS);
    shapes[0].p = PointT(150, HEIGHT / 2);
    shapes[1].p = PointT(WIDTH / 2, 100);
    shapes[2].p = PointT(WIDTH - 150, HEIGHT / 2);
    shapes[3].p = PointT(WIDTH / 2, HEIGHT - 100);
    shapes[0].d = shapes[1].p - shapes[0].p;
    shapes[1].d = shapes[2].p - shapes[1].p;
    shapes[2].d = shapes[3].p - shapes[2].p;
    shapes[3].d = shapes[0].p - shapes[3].p;
    shapes[0].type = shapes[1].type = Segment;
    shapes[2].type = Ray;
    shapes[3].type = Line;
    // create(shapes);

    Polygon<float> pol(LineStrip, NormalLeft);
    for (int i = 0; i < 4; ++i)
        pol.add(shapes[i].p);
    pol.add(shapes[0].p);
    pol.move(VecT(100, 0));

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
                    case sf::Keyboard::Escape:
                    case sf::Keyboard::Q:
                        window.close();
                        break;

                    case sf::Keyboard::R:
                        create(shapes);
                        break;

                    case sf::Keyboard::W:
                        aabb.pos.y -= SPEED;
                        break;

                    case sf::Keyboard::S:
                        aabb.pos.y += SPEED;
                        break;

                    case sf::Keyboard::A:
                        aabb.pos.x -= SPEED;
                        break;

                    case sf::Keyboard::D:
                        aabb.pos.x += SPEED;
                        break;

                    case sf::Keyboard::Up:
                        aabb.size.y -= 10;
                        break;

                    case sf::Keyboard::Right:
                        aabb.size.x += 10;
                        break;

                    case sf::Keyboard::Down:
                        aabb.size.y += 10;
                        break;

                    case sf::Keyboard::Left:
                        aabb.size.x -= 10;
                        break;

                    case sf::Keyboard::H:
                        speed.fill(-100, 0);
                        break;

                    case sf::Keyboard::J:
                        speed.fill(0, 100);
                        break;

                    case sf::Keyboard::K:
                        speed.fill(0, -100);
                        break;

                    case sf::Keyboard::L:
                        speed.fill(100, 0);
                        break;
                }
            }
            else if (ev.type == sf::Event::MouseMoved)
            {
                speed.x = ev.mouseMove.x - (aabb.x + aabb.w / 2);
                speed.y = ev.mouseMove.y - (aabb.y + aabb.h / 2);
            }
        }

        window.clear(sf::Color(0, 43, 54));

        auto pos = aabb.pos.asPoint() + aabb.size / 2;
        drawPoint(window, pos, sf::Color::Blue);
        drawRect(window, aabb);
        drawLine(window, pos, pos + speed, sf::Color::Green);

        IsecT nearest;

        for (size_t i = 0; i < shapes.size(); ++i)
        {
            const auto& line = shapes[i];

            if (line.type == LineType::Ray)
                continue;

            drawPoint(window, line.p);
            if (line.type == LineType::Ray)
                drawLine(window, line.p, line.p + line.d * 1000);
            else if (line.type == LineType::Line)
                drawLine(window, line.p - line.d * 1000, line.p + line.d * 1000);
            else if (line.type == LineType::Segment)
                drawLine(window, line.p, line.p + line.d);

            auto isec = aabb.sweep(speed, line);
            // if (nearest && isec && std::abs(isec.time - nearest.time) < 0.01)
            // {
            //     cout<<"isec.time: "<<isec.time<<endl;
            //     cout<<"nearest.time: "<<nearest.time<<endl;
            //     cout<<"--------------"<<endl;
            // }
            if (!nearest || (isec && isec.time < nearest.time))
                nearest = isec;
        }

        pol.foreachSegment([&](const LineT& seg) {
            drawLine(window, seg.p, seg.p + seg.d);
            return false;
        });

        auto isec = aabb.sweep(speed, pol);
        if (!nearest || (isec && isec.time < nearest.time))
            nearest = isec;

        if (nearest)
        {
            // if (nearest.time < 0)
            //     drawRect(window, aabb, sf::Color::Red);
            auto center = nearest.p + aabb.size / 2;
            AABBT col(aabb);
            col.pos = nearest.p.asVector();
            drawRect(window, col, sf::Color::Red);
            drawPoint(window, center);
            // drawPoint(window, pos + speed * nearest.times[1]);
            drawLine(window, center, center + nearest.normal * 10, sf::Color::Green);
        }
        else
        {
            AABBT col(aabb);
            col.pos += speed;
            drawRect(window, col, sf::Color::Magenta);
        }

        window.display();
    }

    return 0;
}

void create(std::vector<LineT>& vec)
{
    for (size_t i = 0; i < NUM_OBJECTS; ++i)
    {
        vec[i] = LineT(PointT(rand() % WIDTH, rand() % HEIGHT),
                       VecT::fromAngle(rand() % 300, rand() % 360),
                       static_cast<LineType>((i + 2) % 3));

        if (i % 5 == 3)
        {
            vec[i].type = Segment;
            vec[i].d = VecT(0, rand() % 300);
        }
        else if (i % 5 == 4)
        {
            vec[i].type = Segment;
            vec[i].d = VecT(rand() % 300, 0);
        }
    }
}


void drawRect(sf::RenderTarget& target, const AABBT& aabb, sf::Color col)
{
    sf::RectangleShape c(sf::Vector2f(aabb.size.x, aabb.size.y));
    c.setPosition(aabb.pos.x, aabb.pos.y);
    c.setFillColor(sf::Color::Transparent);
    c.setOutlineColor(col);
    c.setOutlineThickness(1);
    target.draw(c);
}

void drawLine(sf::RenderTarget& target, const PointT& p1, const PointT& p2, sf::Color col)
{
    sf::Vertex v[] = {
        sf::Vertex(sf::Vector2f(p1.x, p1.y), col),
        sf::Vertex(sf::Vector2f(p2.x, p2.y), col)
    };
    target.draw(v, 2, sf::Lines);
}

void drawPoint(sf::RenderTarget& target, const PointT& p, sf::Color col)
{
    sf::CircleShape c(3);
    c.setOrigin(3, 3);
    c.setPosition(p.x, p.y);
    c.setFillColor(col);
    target.draw(c);
}
