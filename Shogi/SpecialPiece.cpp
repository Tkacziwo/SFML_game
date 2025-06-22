#include "SpecialPiece.h"

using namespace game;

SpecialPiece::SpecialPiece(IPiece* other)
{
	this->PieceId = other->GetPieceId();
	this->Name = other->GetPieceName();
	this->OriginalName = other->GetPieceOriginalName();
	LoadMoveset(other->GetPieceMoveset());
	this->FacesUp = other->IsFacingUp();
}

SpecialPiece::SpecialPiece(std::string& name, int pieceId, bool isBottom)
{
	Name = name;
	OriginalName = name;
	PieceId = pieceId;
	FacesUp = isBottom;
}

void SpecialPiece::LoadMoveset(std::vector<std::vector<int>> moves)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			Moveset[i][j] = moves[i][j];
		}
	}
}

int SpecialPiece::CheckReversalArguments() const
{
	if (OriginalName != "DarkHorse")
		return 1;
	else
		return 0;
}

bool SpecialPiece::IsItWorthToReverse() const
{
	if (Name == "DarkBishop" || Name == "DarkRook")
		return false;
	else
		return true;
}

void SpecialPiece::ReverseMoveset()
{
	if (IsItWorthToReverse())
	{
		int rowNumber = CheckReversalArguments(),
			movesetCell{};

		if (OriginalName == "DarkHorse" && Name == "DarkGoldGeneral")
		{
			for (size_t i = 0; i < 3; i++)
			{
				movesetCell = Moveset[rowNumber][i];
				Moveset[rowNumber][i] = Moveset[2][i];
				Moveset[2][i] = movesetCell;
			}
		}
		else
		{
			for (size_t i = 0; i < 3; i++)
			{
				movesetCell = Moveset[rowNumber][i];
				Moveset[rowNumber][i] = Moveset[3][i];
				Moveset[3][i] = movesetCell;
			}
		}
	}
}

void SpecialPiece::PromoteToGoldGeneral()
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			Moveset[row][col] = 0;
		}
	}
	for (size_t i = 0; i < 3; i++)
	{
		Moveset[0][i] = 1;
	}
	Moveset[1][0] = 1;
	Moveset[1][1] = 0;
	Moveset[1][2] = 1;
	Moveset[2][0] = 0;
	Moveset[2][1] = 1;
	Moveset[2][2] = 0;
	//PieceId = 60;
	if (FacesUp)
	{
		Name = "GoldGeneral";
	}
	else
	{
		Name = "DarkGoldGeneral";
		ReverseMoveset();
	}
}

std::vector<std::vector<int>> SpecialPiece::GetPieceMoveset()
{
	std::vector<std::vector<int>> temp;
	for (size_t i = 0; i < 4; i++)
	{
		std::vector<int> temp2;
		for (size_t j = 0; j < 3; j++)
		{
			temp2.push_back(Moveset[i][j]);
		}
		temp.push_back(temp2);
		temp2.clear();
	}
	return temp;
}

void SpecialPiece::CheckForPromotion(int destX, std::map<std::string, std::vector<std::vector<int>>>& SpecialPiecePromotion)
{
	if (Name == "Horse" || Name == "DarkHorse" || Name == "DarkLance" || Name == "Lance")
	{
		if ((FacesUp && destX < 3) || (!FacesUp && destX > 5))
		{
			PromoteToGoldGeneral();
		}
	}
	else
	{
		if (FacesUp && destX < 3)
		{
			if (Name == "Bishop")
			{
				LoadMoveset(SpecialPiecePromotion["Pegasus"]);
				Name = "Pegasus";
			}
			else if (Name == "Rook")
			{
				LoadMoveset(SpecialPiecePromotion["Dragon"]);
				Name = "Dragon";
			}
		}
		else if (!FacesUp && destX > 5)
		{
			if (Name == "DarkBishop")
			{
				LoadMoveset(SpecialPiecePromotion["Pegasus"]);
				Name = "DarkPegasus";
			}
			else if (Name == "DarkRook")
			{
				LoadMoveset(SpecialPiecePromotion["Dragon"]);
				Name = "DarkDragon";
			}
		}
	}
}

bool SpecialPiece::IsFacingUp()
{
	bool res = (FacesUp > 0) ? true : false;
	return res;
}