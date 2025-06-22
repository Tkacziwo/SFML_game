#include "SimplePiece.h"

using namespace game;

SimplePiece::SimplePiece(IPiece* other)
{
	this->PieceId = other->GetPieceId();
	this->Name = other->GetPieceName();
	this->OriginalName = other->GetPieceOriginalName();
	LoadMoveset(other->GetPieceMoveset());
	this->FacesUp = other->IsFacingUp();
}

SimplePiece::SimplePiece(std::string name, int id, bool isBottom)
{
	Name = name;
	OriginalName = name;
	PieceId = id;
	FacesUp = isBottom;
}

void SimplePiece::PromoteToGoldGeneral()
{
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

	Name = (FacesUp) ? "GoldGeneral" : "DarkGoldGeneral";
	if (!FacesUp)
	{
		ReverseMoveset();
	}
}

void SimplePiece::LoadMoveset(std::vector<std::vector<int>> moves)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			Moveset[i][j] = moves[i][j];
		}
	}
}

std::vector<std::vector<int>> SimplePiece::GetPieceMoveset()
{
	std::vector<std::vector<int>> temp;
	for (size_t i = 0; i < 3; i++)
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

void SimplePiece::ReverseMoveset()
{
	std::vector<int> temp;
	for (size_t i = 0; i < 3; i++)
	{
		temp.push_back(Moveset[0][i]);
	}
	for (size_t i = 0; i < 3; i++)
	{
		Moveset[0][i] = Moveset[2][i];
		Moveset[2][i] = temp[i];
	}
}

void SimplePiece::CheckForPromotion(int destRow)
{
	if ((FacesUp && destRow < 3) || (!FacesUp && destRow > 5))
	{
		PromoteToGoldGeneral();
	}
}


bool SimplePiece::IsFacingUp()
{
	bool res = (FacesUp > 0) ? true : false;
	return res;
}