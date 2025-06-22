#pragma once
#include "IPiece.h"

namespace game
{

	class SpecialPiece : public IPiece
	{
	public:
		SpecialPiece() {};

		SpecialPiece(IPiece* piece);

		SpecialPiece(std::string& name, int pieceId, bool isBottom);

		void LoadMoveset(std::vector<std::vector<int>> moves);

		int CheckReversalArguments() const;

		bool IsItWorthToReverse() const;

		void ReverseMoveset();

		void PromoteToGoldGeneral();

		std::vector<std::vector<int>> GetPieceMoveset();

		void CheckForPromotion(int destX, std::map<std::string, std::vector<std::vector<int>>>& SpecialPiecePromotion);

		bool IsFacingUp();
	private:
		int Moveset[4][3];
	};
}