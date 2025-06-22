#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

import Hitbox;

namespace core
{
	class Button
	{
	public:

		Button() {};

		Button(const sf::Texture& buttonTexture, sf::Text& buttonText, sf::Font& font, sf::Vector2i offset, std::pair<int, std::string> bind);

		sf::Vector2f PositionTextInButton() const;

		bool IsMouseInBounds(sf::Vector2i mousePos);

		void Action(bool isInside);

		void Draw(sf::RenderTarget& target);

		std::pair<int, std::string> GetActionString() const noexcept;

	private:

		sf::RectangleShape Base;
		sf::Vector2f Offset;
		sf::Text ButtonText;
		sf::Font ButtonFont;
		std::pair<int, std::string> Bind;
		ut::Hitbox Hitbox;
	};
}