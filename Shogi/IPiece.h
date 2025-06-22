#pragma once
#include <string>
#include <vector>
#include <map>

import Position;

namespace game
{
	class IPiece
	{
	public:
		virtual int GetPieceId() { return PieceId; };

		virtual std::string GetPieceName() { return Name; };

		virtual std::string GetPieceOriginalName() { return OriginalName; };

		virtual void SetPieceName(const std::string& name) { Name = name; };

		virtual void SetPieceOriginalName(const std::string& name) { OriginalName = name; };

		virtual std::vector<std::vector<int>> GetPieceMoveset() = 0;

		virtual void LoadMoveset(std::vector<std::vector<int>> moves) = 0;

		//virtual void SetPosition(int x, int y) = 0;

		//virtual std::pair<int, int> GetPosition() = 0;

		virtual bool IsFacingUp() = 0;

		virtual void SetIsBottom(bool isBottom)
		{
			FacesUp = isBottom;
		}

		virtual void ReverseIsBottom()
		{
			FacesUp = (FacesUp == true) ? false : true;
		}

		virtual void RestorePiece() { Name = OriginalName; }

		virtual void PromoteToGoldGeneral() = 0;

		virtual void ReverseMoveset() = 0;

	protected:
		std::string Name;
		std::string OriginalName;
		int PieceId;
		bool FacesUp;
	};

}