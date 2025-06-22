#include "TextureManager.h"

using namespace core;

TextureManager::TextureManager()
{
	PieceTextures = new std::map<std::string, sf::Texture>();
	ButtonTextures = new std::map < std::string, sf::Texture>();
	std::fstream input("texturePaths.txt");
	std::string handler{};


	while (std::getline(input, handler))
	{
		LoadTextures(handler.substr(2), std::stoi(handler.substr(0, 2)));
	}

	input.close();
}

void TextureManager::LoadTextures(const std::filesystem::path& path, int option)
{
	switch (option)
	{
	case 1:
	{
		break;
	}
	case 2:
	{
		LoadSpecificTextures(path, PieceTextures);
		break;
	}
	case 3:
	{
		LoadSpecificTextures(path, DarkPieceTextures);
		break;
	}
	default:
		break;
	}
}

void TextureManager::LoadSpecificTextures(const std::filesystem::path& path, std::map< std::string, sf::Texture>* map)
{
	std::regex fileRegex("(([A-Za-z]+)(.png))");
	std::smatch match;

	for (std::filesystem::directory_entry files : std::filesystem::directory_iterator(path))
	{
		std::filesystem::path pathToFile(path);
		sf::Texture t;
		std::string handler = files.path().filename().string();

		std::regex_search(handler, match, fileRegex);

		pathToFile.append(match[0].str());
		t.loadFromFile(pathToFile.string());
		std::pair<std::string, sf::Texture> pair = std::make_pair(match[2].str(), t);

		map->insert(pair);
	}
}

std::map<std::string, sf::Texture> TextureManager::GetTextures()
{
	return *PieceTextures;
}

std::map<std::string, sf::Texture> TextureManager::GetSpecificTextures(int id)
{
	switch (id)
	{
	case 2:
		return *PieceTextures;
		break;
	default:
		return *DarkPieceTextures;
		break;
	}
}

TextureManager::~TextureManager()
{
	delete PieceTextures;
	delete DarkPieceTextures;
}