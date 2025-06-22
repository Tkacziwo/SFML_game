#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class Scene
{
public:
	virtual void Draw(sf::RenderTarget& target) = 0;

	virtual void HandleButtonHover(sf::Vector2i mousePosition) = 0;

	virtual std::pair<int, std::string> HandleButtonClick(sf::Vector2i mousePosition) = 0;

	void SetScene(int id, std::string name)
	{
		Id = id;
		Name = name;
	}

	int GetSceneId() const noexcept
	{
		return Id;
	}

	std::string GetSceneName() const noexcept
	{
		return Name;
	}

	std::pair<int, std::string> GetScenePair() const noexcept
	{
		return std::pair<int, std::string>(Id, Name);
	}

	int Id;
	std::string Name;
	sf::RectangleShape Background;
};