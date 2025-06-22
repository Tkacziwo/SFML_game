#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "Imports.h"
#include <thread>
#include <mutex>

namespace ut
{
	class BoardUtilities
	{
	public:
		bool IsInBoard(int row, int col);

		bool IsInBoard(const ut::Position& pos);

		bool IsEnemy(bool sourcePieceFacesUp, int row, int col, std::vector<std::vector<ut::BoardTile>>& tiles);

		bool IsPieceInsideEnemy(const bool srcPieceFacesUp, ut::Position& checkedPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		void LoadPieceMovesetFromFile(std::map<std::string, std::vector<std::vector<int>>>& simpleMovesetMap, std::map <std::string, std::vector<std::vector<int>>>& specialMovesetMap, std::map<std::string, std::vector<std::vector<int>>>& specialPiecePromotion);

		void InitBackground(sf::Vector2f& size, sf::RectangleShape& background);

		void ClearPossibleMoves(const ut::Position& pos,
			std::vector<ut::Position>& moves,
			std::vector<std::vector<ut::BoardTile>>& tiles,
			std::map<std::string, sf::Texture>& textureMap);

		std::vector<int> CheckGoodColumnsForDrop(const std::string& name, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> CalculateDropPossibleMoves(game::IPiece* piece, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> CalculatePossibleMoves(game::IPiece* piece, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> CalculateProtectionMoves(game::IPiece* piece, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> CalculateSpecialPossibleMoves(game::IPiece* piece,
			ut::Position srcPos,
			std::vector < std::vector<ut::BoardTile>>& tiles);

		bool RookProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		bool BishopProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		bool LanceProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		bool HorseProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> ExtendSpecialPieceMoves(ut::Position srcPos, ut::Position destPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> MaximumExtension(int pieceId, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> MaximumAbsoluteExtensionRook(ut::Position kingPos, std::vector<std::vector<ut::BoardTile>>& tiles, ut::Position& rookPos);

		std::vector<ut::Position> MaximumAbsoluteExtensionBishop(ut::Position kingPos, std::vector<std::vector<ut::BoardTile>>& tiles, ut::Position& bishopPos);

		ut::Position SpecialPieceAttackCheck(ut::Position srcPos, ut::Position checkedPos, int& shift, std::vector<std::vector<ut::BoardTile>>& tiles);

		void DisplayPossibleMoves(std::vector<ut::Position>& moves,
			bool isSrcPieceFacingUp,
			std::vector<std::vector<ut::BoardTile>>& tiles,
			std::map<std::string, sf::Texture>& textureMap);
	};
}