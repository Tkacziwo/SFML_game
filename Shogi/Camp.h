#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "IPiece.h"
#include "Imports.h"

namespace game
{
	class Camp
	{
	public:
		Camp();

		Camp(const sf::Vector2f& offset);

		void AddCapturedPiece(game::IPiece* piece, sf::Texture&);

		game::IPiece* RemovePieceFromCapturedPieces(int id);

		std::map<int, ut::BoardTile> GetCapturedPieces();

		game::IPiece* GetCapturedPiece(int id);

		sf::Vector2f GetCurrentOffset() const;
		
		void Draw(sf::RenderTarget& target);

		void HandleHover(sf::Vector2i mousePos);

		game::IPiece* HandleButtonClick(sf::Vector2i mousePos);

		bool ClickedOnCapturedPiece(sf::Vector2i mousePos);

		int GetKeyFromMap(sf::Vector2i mousePos);

		void FixCampAfterCapture(std::map<std::string, sf::Texture>& textureMap);

		~Camp();

	private:
		std::map<int, ut::BoardTile> CapturedPieces;
		sf::RectangleShape Background;
		sf::Vector2f CurrentOffset;
		sf::Vector2f GlobalOffset;
		int CurrentId;
	};
}