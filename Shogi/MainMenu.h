#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "Button.h"
#include "InfoText.h"

import FontManager;

class MainMenu : public Scene
{
public:
	MainMenu() {};

	MainMenu(const std::vector<sf::Texture>& textureVec, sf::Font& font)
	{
		SetScene(0, "MainMenu");

		InitSprites(textureVec);
		sf::Text temp;

		temp.setString("ZAGRAJ");
		temp.setCharacterSize(40);
		temp.setFillColor(sf::Color::Black);
		sf::Vector2i offset(200, 200);
		sf::Vector2f offset2(330, 50);
		Info.Initialize("S H O G I", font, offset2);
		Info.TransparentBackground();
		Info.SetTextSize(60);
		core::Button btn(textureVec[0], temp, font, offset, std::pair<int, std::string>(3, "GameWithPlayer"));

		Buttons.push_back(btn);
		temp.setString("WYJDZ");

		offset.y += 260;

		core::Button btn1(textureVec[0], temp, font, offset, std::pair<int, std::string>(999, "Quit"));
		Buttons.push_back(btn1);
	}

	void InitSprites(const std::vector<sf::Texture>& textureVec)
	{
		for (size_t i = 0; i < textureVec.size(); i++)
		{
			Textures.push_back(textureVec[i]);
		}
	}

	void InitBackground(sf::Vector2f& size)
	{
		Background.setSize(size);
		Background.setFillColor(sf::Color(221, 212, 163));
	}

	void Draw(sf::RenderTarget& target)
	{
		sf::Vector2f size = (sf::Vector2f)target.getSize();

		InitBackground(size);
		target.draw(Background);

		Info.Draw(target);
		for (size_t i = 0; i < Buttons.size(); i++)
		{
			Buttons[i].Draw(target);
		}
	}

	std::pair<int, std::string> HandleButtonClick(sf::Vector2i mousePosition)
	{
		std::pair<int, std::string> action;
		for (size_t i = 0; i < Buttons.size(); i++)
		{
			if (Buttons[i].IsMouseInBounds(mousePosition) == true)
			{
				action = Buttons[i].GetActionString();
				if (action.second != "")
				{
					return action;
					break;
				}
			}
		}

		action.first = 100;
		action.second = "NotBound";
		return action;
	}

	void HandleButtonHover(sf::Vector2i mousePosition)
	{
		for (size_t i = 0; i < Buttons.size(); i++)
		{
			if (Buttons[i].IsMouseInBounds(mousePosition) == true)
			{
				Buttons[i].Action(true);
			}
			else
			{
				Buttons[i].Action(false);
			}
		}
	}

private:
	std::vector<sf::Texture> Textures;
	std::vector<core::Button> Buttons;
	ut::InfoText Info;
};