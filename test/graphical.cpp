#include <iostream>
#include <SFML/Graphics.hpp>
#include "math/geometry/Line2.hpp"
#include <time.h>
#include <cstdlib>
#include <vector>

#define WIDTH 640
#define HEIGHT 480
#define SPEED 5

using namespace std;
using namespace geometry;

typedef Line2<float> Linef;
typedef Line2<float, true> Rayf;
typedef Point2<float> Pointf;
typedef Vector2<float> Vectorf;

struct RayLine
{
    union
    {
        Linef l;
        Rayf r;
    };
    bool isray;

    RayLine() : isray(false) {}
};

void create(std::vector<RayLine>& vec);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "test", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    std::vector<RayLine> lines(11);
    create(lines);

    Linef& line(lines[0].l);
    line = Linef(Pointf(0, 0), Vectorf(1, 1));

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
                        create(lines);
                        break;

                    case sf::Keyboard::W:
                        line.p.y -= SPEED;
                        break;

                    case sf::Keyboard::S:
                        line.p.y += SPEED;
                        break;

                    case sf::Keyboard::A:
                        line.p.x -= SPEED;
                        break;

                    case sf::Keyboard::D:
                        line.p.x += SPEED;
                        break;

                }
            }
        }

        line.d.x = sf::Mouse::getPosition(window).x - line.p.x;
        line.d.y = sf::Mouse::getPosition(window).y - line.p.y;

        window.clear();

        for (size_t i = 0; i < lines.size(); ++i)
        {
            const RayLine& l(lines[i]);
            Pointf start = l.isray ? l.r.p : l.l.p - l.l.d * 1000;
            Pointf stop = l.isray ? l.r.p + l.r.d * 1000 : l.l.p + l.l.d * 1000;
            sf::Vertex v[] = {
                sf::Vertex(sf::Vector2f(start.x, start.y)),
                sf::Vertex(sf::Vector2f(stop.x, stop.y))
            };
            window.draw(v, 2, sf::Lines);

            if (i != 0)
            {
                Linef::Line2Intersection isec;
                if (l.isray)
                    isec = line.intersect(l.r);
                else
                    isec = line.intersect(l.l);

                if (isec)
                {
                    c.setPosition(isec->x, isec->y);
                    c.setFillColor(sf::Color::Red);
                    window.draw(c);
                }
            }
        }

        c.setPosition(line.p.x, line.p.y);
        c.setFillColor(sf::Color::Blue);
        window.draw(c);

        window.display();
    }

    return 0;
}

void create(std::vector<RayLine>& vec)
{
    for (size_t i = 1; i <= 10; ++i)
    {
        vec[i] = RayLine();
        if (i % 2)
        {
            vec[i].isray = false;
            vec[i].l = Linef(Pointf(rand() % WIDTH, rand() % HEIGHT),
                    Vectorf::fromDirection(1, rand() % 360));
        }
        else
        {
            vec[i].isray = true;
            vec[i].r = Rayf(Pointf(rand() % WIDTH, rand() % HEIGHT),
                    Vectorf::fromDirection(1, rand() % 360));
        }
    }
}
