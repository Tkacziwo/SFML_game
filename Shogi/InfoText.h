#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

namespace ut
{
	class InfoText
	{
	private:
		sf::RectangleShape Base;
		sf::Text Text;
		sf::Font TextFont;
	public:
		InfoText() {};

		void UpdateText(const std::string& text)
		{
			Text.setString(text);
		}

		void Initialize(std::string text, sf::Font& font, sf::Vector2f& offset)
		{
			TextFont = font;
			Text.setString(text);
			Base.setSize(sf::Vector2f(250, 120));
			Base.setPosition(offset);
			Base.setFillColor(sf::Color::White);
		}

		void TransparentBackground()
		{
			Base.setFillColor(sf::Color::Transparent);
		}

		void SetTextSize(int size)
		{
			Text.setCharacterSize(size);
		}

		void Draw(sf::RenderTarget& target)
		{
			Text.setFont(TextFont);
			Text.setFillColor(sf::Color::Black);
			Text.setOrigin(Text.getGlobalBounds().getSize() / 2.f + Text.getLocalBounds().getPosition());
			Text.setPosition(Base.getPosition() + (Base.getSize() / 2.f));
			target.draw(Base);
			target.draw(Text);
		}

		~InfoText() {};
	};
}