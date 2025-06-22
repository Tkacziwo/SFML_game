#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include <regex>
#include <iostream>

namespace core
{
	class TextureManager
	{
	public:
		TextureManager();

		void LoadTextures(const std::filesystem::path& path, int option);

		void LoadSpecificTextures(const std::filesystem::path& path, std::map < std::string, sf::Texture>* map);

		std::map<std::string, sf::Texture> GetTextures();

		std::map<std::string, sf::Texture> GetSpecificTextures(int id);
		
		~TextureManager();
	private:
		std::map<std::string, sf::Texture>* PieceTextures;
		std::map<std::string, sf::Texture>* DarkPieceTextures;
		std::map<std::string, sf::Texture>* ButtonTextures;
	};
}