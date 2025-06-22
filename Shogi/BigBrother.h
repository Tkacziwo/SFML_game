#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Scene.h"
#include "MainMenu.h"
#include <set>
#include <map>
#include "SceneManager.h"
#include "TextureManager.h"

import FontManager;

namespace bb
{
	class BigBrother
	{
	public:
		BigBrother();
		~BigBrother();

		void PollEvents();
		void GetMousePosition();
		void Update();
		void Render();
		bool IsWindowOpen() const noexcept;
		sf::RenderWindow* Window;
		sf::Vector2i MousePosition;

		std::vector<sf::Texture> Textures;

		//core::TextureManager TextureManager;
		core::SceneManager SceneManager;
		int CurrentScene;

		ut::FontManager FontManager;

	private:
		void InitVariables();
		void InitWindow();
		void InitSceneManager();
		void InitSprites();
		//void InitManagers();

		std::vector<std::set<int, std::string>> Scenes;
	};

	BigBrother::BigBrother()
	{
		InitVariables();
		InitWindow();
		InitSprites();
		InitSceneManager();
	}

	BigBrother::~BigBrother()
	{
		delete this->Window;
	}

	inline void BigBrother::Update()
	{
		this->PollEvents();

		this->GetMousePosition();
	}

	inline void BigBrother::PollEvents()
	{
		sf::Event event;

		while (Window->pollEvent(event))
		{
			switch (event.type)
			{

			case sf::Event::Closed:
				Window->close();
				break;

			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					Window->close();
					break;
				}
				if (event.key.code == sf::Keyboard::F)
				{
				}
			}
			case sf::Event::MouseButtonPressed:
			{
				std::pair<int, std::string> action = SceneManager.HandleButtonClick(MousePosition);

				std::cout << "Action Id: " << action.first << " -> " << action.second;
				std::cout << "\nMouse Pos -> X:" << MousePosition.x << " Y: " << MousePosition.y << "\n";

				if (action.first >= 0 && action.first <=10)
				{
					CurrentScene = SceneManager.ChangeScenes(action.first);
					InitSceneManager();
				}

				if (action.first == 999)
				{
					Window->close();
					break;
				}
			}
			case sf::Event::MouseMoved:
			{
				SceneManager.HandleButtonHover(MousePosition);
			}
			}
		}
	}

	inline void BigBrother::GetMousePosition()
	{
		this->MousePosition = sf::Mouse::getPosition(*this->Window);
	}

	inline void BigBrother::Render()
	{
		Window->clear();
		
		SceneManager.DrawScene(*Window);

		Window->display();
	}

	inline void BigBrother::InitVariables()
	{
		this->Window = nullptr;
		FontManager.LoadFontFromFile();
	}

	inline void BigBrother::InitWindow()
	{
		this->Window = new sf::RenderWindow(sf::VideoMode(900, 900), "Shogi");
		this->Window->setFramerateLimit(30);
	}

	inline void BigBrother::InitSceneManager()
	{
		SceneManager.InitialiseScenes(Window);

		sf::Font font = FontManager.GetFont();
		SceneManager.SetScene(CurrentScene, Textures, font);
	}

	/*inline void BigBrother::InitManagers()
	{
		TextureManagers
	}*/

	inline void BigBrother::InitSprites()
	{
		//tochange to read names from file

		std::filesystem::path path(".\\Sprite\\btn.png");

		sf::Texture tempTextures;
		tempTextures.loadFromFile(path.generic_string());
		Textures.push_back(tempTextures);
		std::filesystem::path path2(".\\Sprite\\btn2.png");
		tempTextures.loadFromFile(path2.generic_string());
		Textures.push_back(tempTextures);
	}

	inline bool BigBrother::IsWindowOpen() const noexcept
	{
		if (this->Window->isOpen())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}