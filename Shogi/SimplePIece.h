#include "IPiece.h"

namespace game
{
	class SimplePiece : public IPiece
	{
	public:

		SimplePiece() {};

		SimplePiece(IPiece* piece);

		SimplePiece(std::string name, int id, bool isBottom);

		void LoadMoveset(std::vector<std::vector<int>> moves);
		
		void PromoteToGoldGeneral();

		void ReverseMoveset();

		std::vector<std::vector<int>> GetPieceMoveset();

		void CheckForPromotion(int destRow);

		bool IsFacingUp();

		~SimplePiece() {};

	private:
		int Moveset[3][3];
	};
}