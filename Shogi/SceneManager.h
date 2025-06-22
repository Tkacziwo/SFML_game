#pragma once
#include "MainMenu.h"
#include <map>
#include <string>
#include "Board.h"
#include "TextureManager.h"

namespace core
{
	class SceneManager
	{
	public:
		SceneManager();

		~SceneManager();

		void InitialiseScenes(sf::RenderWindow* window);

		void InitTextures();

		int ChangeScenes(int sceneId);

		void DrawScene(sf::RenderTarget& target);

		void SetScene(int sceneId, const std::vector<sf::Texture>& textureVec, sf::Font& font);

		std::pair<int, std::string> HandleButtonClick(sf::Vector2i mousePosition);

		void HandleButtonHover(sf::Vector2i mousePosition);

	private:

		int CurrentScene;
		TextureManager Textures;
		Scene* CurrentSceneObject;
		sf::Vector2f Size;
	};
}
