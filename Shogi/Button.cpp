#include "Button.h"

using namespace core;

Button::Button(const sf::Texture& buttonTexture, sf::Text& buttonText, sf::Font& font, sf::Vector2i offset, std::pair<int, std::string> bind)
{
	Offset = sf::Vector2f(offset);
	Bind = bind;
	ButtonFont = font;
	Base.setSize((sf::Vector2f)buttonTexture.getSize());
	Base.setTexture(&buttonTexture);
	ButtonText = buttonText;
	Base.setPosition(Offset);
	ButtonText.setPosition(Offset);
	Hitbox = ut::Hitbox(Base);
}

sf::Vector2f Button::PositionTextInButton() const
{
	sf::Vector2f size = Base.getSize();
	auto characterSize = ButtonText.getCharacterSize();
	std::string text = ButtonText.getString();
	int stringLength = text.length();
	int x = Offset.x;
	int y = Offset.y;
	int centerX = x + size.x/2 - (stringLength*characterSize);
	int centerY = y + size.y/2 - characterSize;
	sf::Vector2f buttonPos(centerX, centerY);
	return buttonPos;
}

bool Button::IsMouseInBounds(sf::Vector2i mousePos)
{
	return Hitbox.CheckHitbox(mousePos);
}

void Button::Action(bool isInside)
{
	if (isInside == true)
	{
		Base.setFillColor(sf::Color{ 0x7F7F7FFF });
	}
	else
	{
		Base.setFillColor(sf::Color::White	);
	}
}

void Button::Draw(sf::RenderTarget& target)
{
	ButtonText.setFont(ButtonFont);
	ButtonText.setFillColor(sf::Color::Black);
	ButtonText.setOrigin(ButtonText.getGlobalBounds().getSize() / 2.f + ButtonText.getLocalBounds().getPosition());
	ButtonText.setPosition(Base.getPosition() + (Base.getSize() / 2.f));
	target.draw(Base);
	target.draw(ButtonText);
}

std::pair<int, std::string> Button::GetActionString() const noexcept
{
	return Bind;
}