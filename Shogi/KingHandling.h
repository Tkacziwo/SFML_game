#pragma once
#include "Imports.h"
#include "InfoText.h"
#include <thread>
#include <mutex>

namespace ut
{
	class KingHandling
	{
	private:
		ut::BoardUtilities* Utility;
		game::SimplePiece King;
		ut::Position KingPosition;
		ut::Position AttackerPos;
		std::vector<ut::Position> KingPossibleMoves;
	public:
		KingHandling()
		{
			Utility = new BoardUtilities();
		}

		void SetKing(game::SimplePiece king);

		void SetKingPosition(const ut::Position& pos);

		ut::Position GetKingPosition() const noexcept;

		void SetAttackerPosition(const ut::Position& p);

		ut::Position GetAttackerPosition() const noexcept;

		void CheckPossibleMoves(std::vector<std::vector<ut::BoardTile>>& tiles);

		void CheckSafeMoves(std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> GetKingMoves() const noexcept;

		ut::Position FindKing(bool playerB, std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> FindBodyguards(std::vector<std::vector<ut::BoardTile>>& tiles);

		std::vector<ut::Position> FindSacrifices(std::vector<std::vector<ut::BoardTile>>& tiles, std::vector<ut::Position>& endangeredMoves);

		std::vector<int> CheckDropSacrifices(std::map<int, ut::BoardTile> pieces, std::vector<ut::Position>& endangeredMoves, std::vector<std::vector<ut::BoardTile>>& tiles);

		bool CanPieceKillAttacker(std::vector<std::vector<ut::BoardTile>>& tiles, ut::Position guardPos, ut::Position attackersPos);

		bool IsAttackingPieceProtected(ut::Position attackerPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		bool IsAttackingPieceProtectedInVicinity(ut::Position attackerPos, std::vector<std::vector<ut::BoardTile>>& tiles);

		~KingHandling()
		{
			delete Utility;
		}
	};
}