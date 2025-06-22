#include "SceneManager.h"

using namespace core;

//implement or leave be
SceneManager::SceneManager() {};

//Implement
void SceneManager::InitialiseScenes(sf::RenderWindow* window)
{
	Size = sf::Vector2f(window->getSize());
}

void SceneManager::InitTextures()
{
	//Textures.LoadPieceTextures("..\\Sprite\\Piece\\");
}

int SceneManager::ChangeScenes(int sceneId)
{
	CurrentScene = sceneId;
	return CurrentScene;
}

void SceneManager::DrawScene(sf::RenderTarget& target)
{
	CurrentSceneObject->Draw(target);
}

void SceneManager::SetScene(int sceneId, const std::vector<sf::Texture>& textureVec, sf::Font& font)
{
	switch (sceneId)
	{

	case 0:
	{
		CurrentSceneObject = new MainMenu(textureVec, font);
		break;
	}

	case 3:
		CurrentSceneObject = new scenes::Board(Textures.GetSpecificTextures(2), textureVec, font, Size);
	default:
		break;
	}
}

std::pair<int, std::string> SceneManager::HandleButtonClick(sf::Vector2i mousePosition)
{
	return CurrentSceneObject->HandleButtonClick(mousePosition);
}

void SceneManager::HandleButtonHover(sf::Vector2i mousePosition)
{
	CurrentSceneObject->HandleButtonHover(mousePosition);
}

SceneManager::~SceneManager()
{
	delete CurrentSceneObject;
}