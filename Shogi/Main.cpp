#include <SFML/Graphics.hpp>
#include "Engine.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    Engine engine;

    while (engine.window->isOpen())
    {
        engine.Update();

        engine.Render();
    }

    return 0;
}