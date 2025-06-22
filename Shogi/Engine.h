#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Engine {
public:


	sf::RenderWindow* window;

	sf::Vector2i mousePos;

	Engine()
	{
		window = new sf::RenderWindow(sf::VideoMode(500, 500), "SFML Shooter");
	}

	void PollEvents()
	{
		sf::Event event;

		while (window->pollEvent(event))
		{
			switch (event.type)
			{

			case sf::Event::Closed:
				window->close();
				break;

			}
		}
	}

	void GetMousePosition()
	{
		this->mousePos = sf::Mouse::getPosition(*window);
	}

	void Render()
	{
		window->clear();

		window->display();
	}

	void Update()
	{
		this->PollEvents();

		this->GetMousePosition();
	}
};
