#pragma once
#include "Button.h"
#include "Scene.h"
#include "Camp.h"
#include "BoardUtilities.h"
#include "KingHandling.h"
#include "InfoText.h"
#include "BoardClickHandler.h"

namespace scenes
{
	class Board : public Scene
	{
	public:
		std::vector<core::Button> Buttons;

		Board(const std::map<std::string, sf::Texture>& textureMap,const std::vector<sf::Texture>& btnTextures, sf::Font& font, sf::Vector2f screenSize);

		void GenerateField(sf::Vector2f& offset);

		void CapturePiece(game::Camp& camp, game::IPiece* piece);

		void LoadPositioning();

		void CreateAndPlacePiece(bool isBottom, int pieceId, std::string pieceName, int row, int column, bool isSpecial);

		void MovePiece(const ut::Position& srcPos, const ut::Position& destPos);

		void DropPiece(game::IPiece* piece, const ut::Position& destPos);

		void Draw(sf::RenderTarget& target);

		void HandleTile(const sf::Vector2i& mousePos, bool playerB);

		void HandleClickedOnPossibleMoves(const sf::Vector2i& mousePos, int row, int col, bool playerB);

		void HandleClickedOnOtherPiece(const sf::Vector2i& mousePos, game::IPiece* piece, ut::Position pos, bool playerB);

		void HandleClickedOnKing(const sf::Vector2i& mousePos, game::IPiece* piece, ut::Position pos);

		std::pair<int, std::string> HandleButtonClick(sf::Vector2i mousePosition);

		void HandleButtonHover(sf::Vector2i mousePosition);

		void KingInDanger(game::IPiece* piece, const ut::Position& srcPos);

		std::pair<int, std::string> HandleKingEscape(const sf::Vector2i& mousePos, bool playerB);

		void ProtectTheKing(std::vector<ut::Position> bodyguardsPos, std::vector<ut::Position> sacrifices, std::map<int, ut::BoardTile> drops, std::vector<ut::Position> endangeredMoves, sf::Vector2i mousePos);

		~Board();

	private:
		ut::BoardClickHandler ClickHandler;
		ut::KingHandling KingHandling;
		game::Camp CampA;
		game::Camp CampB;
		std::vector<std::vector<ut::BoardTile>> Tiles;
		std::map<std::string, sf::Texture> TextureMap;
		std::map <std::string, std::vector<std::vector<int>>> MovesetSimplePiece;
		std::map <std::string, std::vector<std::vector<int>>> MovesetSpecialPiece;
		std::map<std::string, std::vector<std::vector<int>>> SpecialPiecePromotion;
		ut::InfoText Info;
		ut::Position ChosenPiecePosition;
		std::vector<ut::Position> NewPossibleMoves;
		bool PieceChosen = false;
		int CapturedPieceMapKey;
		bool PlayerB;
		bool BoolKingInDanger;
		bool BoolDropPiece;
		bool GameEnd;
		game::IPiece* DroppedPiece;
		ut::BoardUtilities* Utility;
		sf::Vector2f UniversalOffset;
	};
}