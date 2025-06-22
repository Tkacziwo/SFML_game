#include "Camp.h"

using namespace game;

Camp::Camp()
{
	CurrentOffset = sf::Vector2f(0, 0);
	CurrentId = 1;
	Background.setSize(sf::Vector2f(51 * 9, 200));
	Background.setFillColor(sf::Color::White);
}

Camp::Camp(const sf::Vector2f& offset)
{
	CurrentOffset = offset;
	GlobalOffset = offset;
	CurrentId = 1;
	Background.setSize(sf::Vector2f(51 * 9, 202));
	Background.setFillColor(sf::Color::White);
	Background.setPosition(CurrentOffset);
}

void Camp::AddCapturedPiece(game::IPiece* piece, sf::Texture& texture)
{
	ut::BoardTile tempTile(CurrentOffset);
	std::pair<int, ut::BoardTile> pair;

	if (piece->GetPieceId() > 62)
	{
		game::SpecialPiece* tempPiece = new game::SpecialPiece(piece);
		tempPiece->ReverseMoveset();
		tempPiece->ReverseIsBottom();
		tempTile.AssignSpecialPiece(*tempPiece, texture);
		pair = std::make_pair(CurrentId, tempTile);
		delete tempPiece;
	}
	else
	{
		game::SimplePiece* tempPiece = new game::SimplePiece(piece);
		tempPiece->ReverseMoveset();
		tempPiece->ReverseIsBottom();
		tempTile.AssignSimplePiece(*tempPiece, texture);
		pair = std::make_pair(CurrentId, tempTile);
		delete tempPiece;
	}
	CapturedPieces.insert(pair);
	CurrentId++;

	if (CapturedPieces.size() != 0 && CapturedPieces.size() % 9 == 0)
	{
		CurrentOffset.y += 51;
		CurrentOffset.x = GlobalOffset.x;
	}
	else
	{
		CurrentOffset.x += 51;
	}
}

game::IPiece* Camp::RemovePieceFromCapturedPieces(int id)
{
	auto piece = CapturedPieces[id].GetPiece();
	CapturedPieces.erase(id);

	return piece;
}

std::map<int, ut::BoardTile> Camp::GetCapturedPieces()
{
	return CapturedPieces;
}

game::IPiece* game::Camp::GetCapturedPiece(int id)
{
	int removedIterator{};
	game::IPiece* piece = CapturedPieces[id].GetPiece();
	for (auto& c : CapturedPieces)
	{
		if (c.first != id)
		{
			removedIterator++;
		}
		else
		{
			break;
		}
	}
	CapturedPieces.erase(id);
	return piece;
}

sf::Vector2f Camp::GetCurrentOffset() const
{
	return CurrentOffset;
}

void Camp::Draw(sf::RenderTarget& target)
{
	target.draw(Background);
	for (int i = 1; i < CurrentId; i++)
	{
		CapturedPieces[i].DrawTile(target);
	}
}

void Camp::HandleHover(sf::Vector2i mousePos)
{
	for (auto& p : CapturedPieces)
	{
		if (p.second.ContainsPieceInBounds(mousePos))
		{
			p.second.SetBackground(sf::Color{ 0x7F7F7FFF });
		}
		else
		{
			p.second.SetBackground(sf::Color::White);
		}
	}
}
game::IPiece* game::Camp::HandleButtonClick(sf::Vector2i mousePos)
{
	for (auto& p : CapturedPieces)
	{
		if (p.second.ContainsPieceInBounds(mousePos))
		{
			return p.second.GetPiece();
		}
	}
	return nullptr;
}
bool game::Camp::ClickedOnCapturedPiece(sf::Vector2i mousePos)
{
	for (auto& p : CapturedPieces)
	{
		if (p.second.ContainsPieceInBounds(mousePos))
		{
			return true;
		}
	}
	return false;
}
int game::Camp::GetKeyFromMap(sf::Vector2i mousePos)
{
	for (int i = 1; i <= CapturedPieces.size(); i++)
	{
		if (CapturedPieces[i].ContainsPieceInBounds(mousePos))
		{
			return i;
		}
	}
}

void game::Camp::FixCampAfterCapture(std::map<std::string, sf::Texture>& textureMap)
{
	if (CapturedPieces.size() != 0)
	{
		std::map<int, ut::BoardTile> temp = CapturedPieces;
		std::vector<ut::BoardTile> pieces;
		for (auto& t : temp)
		{
			pieces.push_back(t.second);
		}
		CapturedPieces.clear();
		CurrentOffset = GlobalOffset;
		CurrentId = 1;
		for (int i = 0; i < pieces.size(); i++)
		{
			ut::BoardTile tempTile(CurrentOffset);
			std::pair<int, ut::BoardTile> pair;
			game::IPiece* piece = pieces[i].GetPiece();
			if (piece->GetPieceId() > 62)
			{
				game::SpecialPiece* tempPiece = new game::SpecialPiece(piece);
				tempTile.AssignSpecialPiece(*tempPiece, textureMap[piece->GetPieceName()]);
				pair = std::make_pair(CurrentId, tempTile);
				delete tempPiece;
			}
			else
			{
				game::SimplePiece* tempPiece = new game::SimplePiece(piece);
				tempTile.AssignSimplePiece(*tempPiece, textureMap[piece->GetPieceName()]);
				pair = std::make_pair(CurrentId, tempTile);
				delete tempPiece;
			}
			CapturedPieces.insert(pair);
			CurrentId++;

			if (CapturedPieces.size() != 0 && CapturedPieces.size() % 9 == 0)
			{
				CurrentOffset.y += 51;
				CurrentOffset.x = GlobalOffset.x;
			}
			else
			{
				CurrentOffset.x += 51;
			}
		}
	}
	else
	{
		CapturedPieces.clear();
		CurrentOffset = GlobalOffset;
		CurrentId = 1;
	}
}

Camp::~Camp()
{
}