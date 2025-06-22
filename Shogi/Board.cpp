#include "Board.h"

using namespace scenes;

Board::Board(const std::map<std::string, sf::Texture>& textureMap, const std::vector<sf::Texture>& btnTextures, sf::Font& font, sf::Vector2f screenSize)
	: TextureMap(textureMap)
{
	sf::Text text;
	Utility = new ut::BoardUtilities();
	SetScene(3, "Game");
	UniversalOffset = sf::Vector2f(100, 10);
	PlayerB = true;
	CampA = game::Camp(UniversalOffset);
	UniversalOffset.y += 205;
	sf::Vector2f temptemp(580, UniversalOffset.y);
	Info.Initialize("string", font, temptemp);
	text.setString("ZRESETUJ");
	auto y = UniversalOffset.y;
	core::Button btn(btnTextures[1], text, font, sf::Vector2i(580, y + 140), std::pair<int, std::string>(3, "GameWithPlayer"));
	Buttons.push_back(btn);
	text.setString("WYJDZ");
	core::Button btn2(btnTextures[1], text, font, sf::Vector2i(580, y + 280), std::pair<int, std::string>(999, "Wyjdz"));
	Buttons.push_back(btn2);
	GenerateField(UniversalOffset);
	UniversalOffset.y += 2;
	CampB = game::Camp(UniversalOffset);
	Utility->LoadPieceMovesetFromFile(MovesetSimplePiece, MovesetSpecialPiece, SpecialPiecePromotion);
	LoadPositioning();

}

void Board::GenerateField(sf::Vector2f& offset)
{
	sf::Vector2f backup(offset);
	for (int i = 0; i < 9; i++)
	{
		std::vector<ut::BoardTile> tempVec;
		for (int j = 0; j < 9; j++)
		{
			ut::BoardTile tile(offset);
			offset.x += 51;
			tempVec.push_back(tile);
			tile.DeallocateMemory();
		}
		Tiles.push_back(tempVec);
		tempVec.clear();
		offset.y += 51;
		offset.x = backup.x;
	}
}

void Board::LoadPositioning()
{
	std::fstream file("positioning.txt");
	std::string handler;
	int row{ 0 };
	bool isBottom{ false };
	while (std::getline(file, handler))
	{
		int column{ 0 };
		std::stringstream ss(handler);
		int pieceId;
		while (ss >> pieceId)
		{
			std::string pieceName;
			switch (pieceId)
			{
			case 0:
				pieceName = "Unassigned";
				break;
			case 10:
				pieceName = "Pawn";
				break;
			case 60:
				pieceName = "GoldGeneral";
				break;
			case 61:
				pieceName = "SilverGeneral";
				break;
			case 62:
				pieceName = "King";
				break;
			case 63:
				pieceName = "Bishop";
				break;
			case 64:
				pieceName = "Rook";
				break;
			case 65:
				pieceName = "Lance";
				break;
			case 66:
				pieceName = "Horse";
				break;
			default:
				break;
			}
			if (row > 2)
			{
				isBottom = true;
			}
			if (pieceId == 63 || pieceId == 64 || pieceId == 65 || pieceId == 66)
			{
				CreateAndPlacePiece(isBottom, pieceId, pieceName, row, column, true);
			}
			else if (pieceName != "Unassigned")
			{
				CreateAndPlacePiece(isBottom, pieceId, pieceName, row, column, false);
			}
			column++;
		}
		row++;
	}
	file.close();
}

void Board::CapturePiece(game::Camp& camp, game::IPiece* piece)
{
	piece->RestorePiece();
	std::string tempRestoreName;
	std::string originalName = piece->GetPieceOriginalName();
	bool reverse = false;
	if (originalName.substr(0, 4) == "Dark")
	{
		tempRestoreName = originalName.substr(4);
		reverse = true;
	}
	else
	{
		tempRestoreName = originalName;
	}
	if (piece->GetPieceId() > 62 || originalName == "Horse" || originalName == "DarkHorse")
	{
		piece->LoadMoveset(MovesetSpecialPiece[tempRestoreName]);
		if (piece->GetPieceOriginalName() == "DarkLance")
		{
			piece->ReverseMoveset();
		}
	}
	else
	{
		piece->LoadMoveset(MovesetSimplePiece[tempRestoreName]);
		if (reverse)
		{
			piece->ReverseMoveset();
		}
	}
	if (originalName.substr(0, 4) == "Dark")
	{
		piece->SetPieceName(originalName.substr(4));
		piece->SetPieceOriginalName(originalName.substr(4));
	}
	else
	{
		piece->SetPieceName("Dark" + originalName);
		piece->SetPieceOriginalName("Dark" + originalName);
	}

	camp.AddCapturedPiece(piece, TextureMap[piece->GetPieceName()]);
}

void Board::CreateAndPlacePiece(bool isBottom, int pieceId, std::string pieceName, int row, int column, bool isSpecial)
{
	std::string newName = pieceName;
	bool reverseMoveset{ false };
	if (!isBottom)
	{
		newName = "Dark" + pieceName;
		reverseMoveset = true;
	}

	if (!isSpecial)
	{
		game::SimplePiece piece(newName, pieceId, isBottom);
		piece.LoadMoveset(MovesetSimplePiece[pieceName]);
		if (reverseMoveset)
		{
			piece.ReverseMoveset();
		}
		Tiles[row][column].AssignSimplePiece(piece, TextureMap[newName]);
	}
	else
	{
		game::SpecialPiece piece(newName, pieceId, isBottom);
		piece.LoadMoveset(MovesetSpecialPiece[pieceName]);
		if (reverseMoveset)
		{
			piece.ReverseMoveset();
		}
		Tiles[row][column].AssignSpecialPiece(piece, TextureMap[newName]);
	}
}

void Board::MovePiece(const ut::Position& srcPos, const ut::Position& destPos)
{
	bool promoted = false;
	game::IPiece* piece = Tiles[srcPos.Row][srcPos.Col].GetPiece();
	Tiles[srcPos.Row][srcPos.Col].ReleasePiece();
	std::string originalName = piece->GetPieceOriginalName();
	if (piece->GetPieceId() > 62 || originalName == "Horse" || originalName == "DarkHorse")
	{
		game::SpecialPiece* specialPiece = new game::SpecialPiece(piece);
		specialPiece->CheckForPromotion(destPos.Row, SpecialPiecePromotion);
		std::string tempName = specialPiece->GetPieceName();
		if (tempName != piece->GetPieceName())
		{
			piece->SetPieceName(tempName);
			piece->LoadMoveset(specialPiece->GetPieceMoveset());
		}
		Tiles[destPos.Row][destPos.Col].AssignSpecialPiece(*specialPiece, TextureMap[specialPiece->GetPieceName()]);
		delete specialPiece;
	}
	else
	{
		game::SimplePiece* simplePiece = new game::SimplePiece(piece);
		simplePiece->CheckForPromotion(destPos.Row);
		if (simplePiece->GetPieceName() != piece->GetPieceName())
		{
			piece->SetPieceName(simplePiece->GetPieceName());
			piece->LoadMoveset(simplePiece->GetPieceMoveset());
		}
		Tiles[destPos.Row][destPos.Col].AssignSimplePiece(*simplePiece, TextureMap[simplePiece->GetPieceName()]);
		delete simplePiece;
	}
	Utility->ClearPossibleMoves(destPos, NewPossibleMoves, Tiles, TextureMap);
	if (piece->GetPieceId() != 62)
	{
		KingInDanger(piece, destPos);
	}
}

void scenes::Board::DropPiece(game::IPiece* piece, const ut::Position& destPos)
{
	int destRow{ destPos.Row }, destCol{ destPos.Col };
	if (piece->GetPieceId() > 62)
	{
		game::SpecialPiece* specialPiece = new game::SpecialPiece(piece);
		Tiles[destRow][destCol].AssignSpecialPiece(*specialPiece, TextureMap[specialPiece->GetPieceName()]);
		delete specialPiece;
	}
	else
	{
		game::SimplePiece* simplePiece = new game::SimplePiece(piece);
		Tiles[destRow][destCol].AssignSimplePiece(*simplePiece, TextureMap[piece->GetPieceName()]);
		delete simplePiece;
	}
	Utility->ClearPossibleMoves(destPos, NewPossibleMoves, Tiles, TextureMap);
	KingInDanger(piece, destPos);
}

void Board::Draw(sf::RenderTarget& target)
{
	sf::Vector2f size = (sf::Vector2f)target.getSize();
	Utility->InitBackground(size, Background);
	target.draw(Background);

	CampA.Draw(target);

	for (size_t col = 0; col < 9; col++)
	{
		for (size_t row = 0; row < 9; row++)
		{
			Tiles[row][col].DrawTile(target);
		}
	}
	if (!GameEnd)
	{
		std::string text = (PlayerB) ? "Tura bialego" : "Tura czarnego";
		Info.UpdateText(text);
	}
	Info.Draw(target);
	for (auto& b : Buttons)
	{
		b.Draw(target);
	}

	CampB.Draw(target);
}

void scenes::Board::HandleTile(const sf::Vector2i& mousePos, bool playerB)
{
	for (int col = 0; col < 9; col++)
	{
		for (int row = 0; row < 9; row++)
		{
			if (Tiles[row][col].ContainsPieceInBounds(mousePos) && !Tiles[row][col].IsPossibleMove())
			{
				game::IPiece* piece = Tiles[row][col].GetPiece();
				if (playerB != piece->IsFacingUp())
				{
					break;
				}
				if (PieceChosen)
				{
					Utility->ClearPossibleMoves(ChosenPiecePosition, NewPossibleMoves, Tiles, TextureMap);
				}
				if (piece->GetPieceId() == 62)
				{
					HandleClickedOnKing(mousePos, piece, ut::Position(row, col));
				}
				else
				{
					HandleClickedOnOtherPiece(mousePos, piece, ut::Position(row, col), playerB);
				}
			}
			else
			{
				HandleClickedOnPossibleMoves(mousePos, row, col, playerB);
			}
		}
	}
}

void scenes::Board::HandleClickedOnPossibleMoves(const sf::Vector2i& mousePos, int row, int col, bool playerB)
{
	if (Tiles[row][col].ContainsPossibleMoveInBounds(mousePos))
	{
		bool sourcePieceFacesUp = Tiles[ChosenPiecePosition.Row][ChosenPiecePosition.Col].GetIsPieceFacingUp();
		ut::Position destPosition = ut::Position(row, col);
		if (Tiles[row][col].IsPieceInside() && Utility->IsEnemy(sourcePieceFacesUp, row, col, Tiles))
		{
			game::IPiece* piece = Tiles[row][col].GetPiece();
			if (sourcePieceFacesUp)
				CapturePiece(CampB, piece);
			else
				CapturePiece(CampA, piece);
			Utility->ClearPossibleMoves(destPosition, NewPossibleMoves, Tiles, TextureMap);
		}
		MovePiece(ChosenPiecePosition, destPosition);
		PlayerB = !playerB;
		PieceChosen = false;
	}
}

void scenes::Board::HandleClickedOnOtherPiece(const sf::Vector2i& mousePos, game::IPiece* piece, ut::Position pos, bool playerB)
{
	if (piece->GetPieceId() > 62)
	{
		NewPossibleMoves = Utility->CalculateSpecialPossibleMoves(piece, pos, Tiles);
	}
	else
	{
		NewPossibleMoves = Utility->CalculatePossibleMoves(piece, pos, Tiles);
	}
	ut::Position foundKingPosition = KingHandling.FindKing(playerB, Tiles);
	ut::Position rookPos;
	ut::Position bishopPos;
	std::vector<ut::Position> rookRes;
	std::vector<ut::Position> bishopRes;
	bool removeMoves = false;
	auto maxMovesRook = Utility->MaximumAbsoluteExtensionRook(foundKingPosition, Tiles, rookPos);
	auto maxMovesBishop = Utility->MaximumAbsoluteExtensionBishop(foundKingPosition, Tiles, bishopPos);
	if (maxMovesRook.size() > 0)
	{
		game::IPiece* tempRook = Tiles[rookPos.Row][rookPos.Col].GetPiece();
		int count = 0;
		for (auto& m : maxMovesRook)
		{
			if (Tiles[m.Row][m.Col].IsPieceInside() && Tiles[m.Row][m.Col].GetPieceId() != 62
				&& Utility->IsEnemy(tempRook->IsFacingUp(), m.Row, m.Col, Tiles))
			{
				count++;
			}
		}
		if (count == 1)
		{
			if (pos.Equals(rookPos))
			{
				for (int i = 0; i < NewPossibleMoves.size(); i++)
				{
					for (int j = 0; j < maxMovesRook.size(); j++)
					{
						if (NewPossibleMoves[i].Equals(maxMovesRook[j]))
						{
							rookRes.push_back(NewPossibleMoves[i]);
							break;
						}
					}
				}
				if (rookRes.size() == 0)
				{
					removeMoves = true;
				}
			}
		}
	}
	if (maxMovesBishop.size() > 0)
	{
		game::IPiece* tempBishop = Tiles[bishopPos.Row][bishopPos.Col].GetPiece();
		int count = 0;
		for (auto& m : maxMovesBishop)
		{
			if (Tiles[m.Row][m.Col].IsPieceInside() && Tiles[m.Row][m.Col].GetPieceId() != 62
				&& Utility->IsEnemy(tempBishop->IsFacingUp(), m.Row, m.Col, Tiles))
			{
				count++;
			}
		}
		if (count == 1)
		{
			ut::Position dump;
			auto check = Utility->MaximumAbsoluteExtensionBishop(pos, Tiles, dump);
			for (auto& c : check)
			{
				if (c.Equals(bishopPos))
				{
					for (int i = 0; i < NewPossibleMoves.size(); i++)
					{
						for (int j = 0; j < maxMovesBishop.size(); j++)
						{
							if (NewPossibleMoves[i].Equals(maxMovesBishop[j]))
							{
								bishopRes.push_back(NewPossibleMoves[i]);
								break;
							}
						}
					}
					if (bishopRes.size() == 0)
					{
						removeMoves = true;
					}
				}
			}
		}
	}
	if (rookRes.size() != 0 || bishopRes.size() != 0)
	{
		NewPossibleMoves.clear();
		for (auto& r : rookRes)
		{
			NewPossibleMoves.push_back(r);
		}
		for (auto& b : bishopRes)
		{
			NewPossibleMoves.push_back(b);
		}
	}
	else if ((maxMovesBishop.size() != 0 || maxMovesRook.size() != 0) && removeMoves)
	{
		NewPossibleMoves.clear();
	}
	Utility->DisplayPossibleMoves(NewPossibleMoves, piece->IsFacingUp(), Tiles, TextureMap);
	ChosenPiecePosition.UpdatePosition(pos);
	PieceChosen = true;
}

void scenes::Board::HandleClickedOnKing(const sf::Vector2i& mousePos, game::IPiece* piece, ut::Position pos)
{
	KingHandling.SetKing(piece);
	KingHandling.SetKingPosition(pos);
	KingHandling.CheckPossibleMoves(Tiles);
	auto kingPossibleMoves = KingHandling.GetKingMoves();
	for (int i = 0; i < kingPossibleMoves.size(); i++)
	{
		ut::Position p = kingPossibleMoves[i].GetPosition();
		if (Tiles[p.Row][p.Col].IsPieceInside() && Utility->IsEnemy(piece->IsFacingUp(), p.Row, p.Col, Tiles))
		{
			if (!KingHandling.IsAttackingPieceProtected(p, Tiles))
			{
				Tiles[p.Row][p.Col].AssignTexture(TextureMap["Attacked" + Tiles[p.Row][p.Col].GetPieceName()]);
				Tiles[p.Row][p.Col].SetPossibleMove(true);
			}
		}
		else
		{
			Tiles[p.Row][p.Col].AssignTexture(TextureMap["PossibleMove"]);
			Tiles[p.Row][p.Col].SetPossibleMove(true);
		}

	}
	NewPossibleMoves.clear();
	for (size_t i = 0; i < kingPossibleMoves.size(); i++)
	{
		NewPossibleMoves.push_back(kingPossibleMoves[i]);
	}
	ChosenPiecePosition.UpdatePosition(pos.Row, pos.Col);
	PieceChosen = true;
}

std::pair<int, std::string> Board::HandleButtonClick(sf::Vector2i mousePosition)
{
	std::pair<int, std::string> action;
	if (GameEnd)
	{
		action = ClickHandler.GameEndHandle(Buttons, mousePosition);
	}
	else
	{
		if (BoolKingInDanger)
		{
			action = HandleKingEscape(mousePosition, PlayerB);
		}
		else if (CampB.ClickedOnCapturedPiece(mousePosition) && PlayerB)
		{
			if (PieceChosen)
			{
				Utility->ClearPossibleMoves(ChosenPiecePosition, NewPossibleMoves, Tiles, TextureMap);
			}
			game::IPiece* pieceFromCamp = CampB.HandleButtonClick(mousePosition);
			if (pieceFromCamp)
			{
				CapturedPieceMapKey = CampB.GetKeyFromMap(mousePosition);
				PieceChosen = true;
				NewPossibleMoves = Utility->CalculateDropPossibleMoves(pieceFromCamp, Tiles);
				Utility->DisplayPossibleMoves(NewPossibleMoves, pieceFromCamp->IsFacingUp(), Tiles, TextureMap);
			}
		}
		else if (CapturedPieceMapKey != 0 && PlayerB)
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (Tiles[i][j].ContainsPossibleMoveInBounds(mousePosition))
					{
						game::IPiece* piece = CampB.GetCapturedPiece(CapturedPieceMapKey);
						DropPiece(piece, ut::Position(i, j));
						CampB.FixCampAfterCapture(TextureMap);
						PieceChosen = false;
						CapturedPieceMapKey = 0;
						PlayerB = !PlayerB;
						break;
					}
				}
			}
			if (CapturedPieceMapKey != 0)
			{
				for (int i = 0; i < NewPossibleMoves.size(); i++)
				{
					ut::Position p = NewPossibleMoves[i].GetPosition();
					if (p.Row != -999 || p.Col != -999)
					{
						Tiles[p.Row][p.Col].ClearTexture();
					}
				}
				CapturedPieceMapKey = 0;
				HandleTile(mousePosition, PlayerB);
			}
		}
		else if (CampA.ClickedOnCapturedPiece(mousePosition) && !PlayerB)
		{
			if (PieceChosen)
			{
				Utility->ClearPossibleMoves(ChosenPiecePosition, NewPossibleMoves, Tiles, TextureMap);
			}
			game::IPiece* pieceFromCamp = CampA.HandleButtonClick(mousePosition);
			if (pieceFromCamp)
			{
				CapturedPieceMapKey = CampA.GetKeyFromMap(mousePosition);
				PieceChosen = true;
				NewPossibleMoves = Utility->CalculateDropPossibleMoves(pieceFromCamp, Tiles);
				Utility->DisplayPossibleMoves(NewPossibleMoves, pieceFromCamp->IsFacingUp(), Tiles, TextureMap);
			}
		}
		else if (CapturedPieceMapKey != 0 && !PlayerB)
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (Tiles[i][j].ContainsPossibleMoveInBounds(mousePosition))
					{
						game::IPiece* piece = CampA.GetCapturedPiece(CapturedPieceMapKey);
						DropPiece(piece, ut::Position(i, j));
						CampA.FixCampAfterCapture(TextureMap);
						PieceChosen = false;
						CapturedPieceMapKey = 0;
						PlayerB = !PlayerB;
						break;
					}
				}
			}
			if (CapturedPieceMapKey != 0)
			{
				for (int i = 0; i < NewPossibleMoves.size(); i++)
				{
					ut::Position p = NewPossibleMoves[i].GetPosition();
					if (p.Row != -999 || p.Col != -999)
					{
						Tiles[p.Row][p.Col].ClearTexture();
					}
				}
				CapturedPieceMapKey = 0;
				HandleTile(mousePosition, PlayerB);
			}
		}
		else
		{
			HandleTile(mousePosition, PlayerB);
		}
		if (action.second == "")
		{
			action.first = 100;
			action.second = "NotBound";
		}
		for (auto& b : Buttons)
		{
			if (b.IsMouseInBounds(mousePosition))
			{
				action = b.GetActionString();
				if (action.second != "")
				{
					return action;
					break;
				}
			}
		}
	}
	return action;
}

void Board::HandleButtonHover(sf::Vector2i mousePosition)
{
	//CampA.HandleHover(mousePosition);
	//CampB.HandleHover(mousePosition);
	for (auto& b : Buttons)
	{
		b.Action(b.IsMouseInBounds(mousePosition));
	}
}

void scenes::Board::KingInDanger(game::IPiece* piece, const ut::Position& srcPos)
{
	int id = piece->GetPieceId();
	std::vector<ut::Position> moves;
	if (id <= 62)
	{
		moves = Utility->CalculatePossibleMoves(piece, srcPos, Tiles);
	}
	else if (piece->GetPieceName() == "GoldGeneral" || piece->GetPieceName() == "DarkGoldGeneral")
	{
		moves = Utility->CalculatePossibleMoves(piece, srcPos, Tiles);
	}
	else
	{
		moves = Utility->CalculateSpecialPossibleMoves(piece, srcPos, Tiles);
	}
	for (auto& m : moves)
	{
		if (Tiles[m.Row][m.Col].IsPieceInside())
		{
			if (Tiles[m.Row][m.Col].GetPieceName() == "King" || Tiles[m.Row][m.Col].GetPieceName() == "DarkKing")
			{
				Tiles[m.Row][m.Col].AssignTexture(TextureMap["Attacked" + Tiles[m.Row][m.Col].GetPieceName()]);
				KingHandling.SetKingPosition(m);
				KingHandling.SetAttackerPosition(srcPos);
				BoolKingInDanger = true;
				break;
			}
		}
	}
}

std::pair<int, std::string> scenes::Board::HandleKingEscape(const sf::Vector2i& mousePos, bool playerB)
{
	std::vector<ut::Position> endangeredMoves;
	std::map<int, ut::BoardTile> dropSacrifices;
	std::vector<int> dropSacrificesIds;
	KingHandling.CheckPossibleMoves(Tiles);
	auto bodyguardsPositions = KingHandling.FindBodyguards(Tiles);
	auto sacrifices = KingHandling.FindSacrifices(Tiles, endangeredMoves);

	if (PlayerB && CampB.GetCapturedPieces().size() != 0)
	{
		dropSacrifices = CampB.GetCapturedPieces();
		dropSacrificesIds = KingHandling.CheckDropSacrifices(CampB.GetCapturedPieces(), endangeredMoves, Tiles);
	}
	else if (!PlayerB && CampA.GetCapturedPieces().size() != 0)
	{
		dropSacrifices = CampB.GetCapturedPieces();
		dropSacrificesIds = KingHandling.CheckDropSacrifices(CampA.GetCapturedPieces(), endangeredMoves, Tiles);
	}
	if (KingHandling.GetKingMoves().size() == 0 && bodyguardsPositions.size() == 0 && sacrifices.size() == 0 && dropSacrificesIds.size() == 0)
	{
		std::string winner = (!PlayerB) ? "Bialy" : "Czarny";
		Info.UpdateText("Wygral: " + winner);
		GameEnd = true;
	}
	else
	{
		ProtectTheKing(bodyguardsPositions, sacrifices, dropSacrifices, endangeredMoves, mousePos);
	}
	return std::pair<int, std::string>(100, "NotBound");
}

void scenes::Board::ProtectTheKing(std::vector<ut::Position> bodyguardsPos, std::vector<ut::Position> sacrifices, std::map<int, ut::BoardTile> drops, std::vector<ut::Position> endangeredMoves, sf::Vector2i mousePos)
{
	auto kingPos = KingHandling.GetKingPosition();
	bodyguardsPos.push_back(kingPos);
	for (int i = 0; i < bodyguardsPos.size(); i++)
	{
		if (Tiles[bodyguardsPos[i].Row][bodyguardsPos[i].Col].ContainsPieceInBounds(mousePos))
		{
			if (PieceChosen)
			{
				Utility->ClearPossibleMoves(ChosenPiecePosition, NewPossibleMoves, Tiles, TextureMap);
				BoolDropPiece = false;
				PieceChosen = false;
			}
			game::IPiece* piece = Tiles[bodyguardsPos[i].Row][bodyguardsPos[i].Col].GetPiece();
			if (piece->GetPieceId() == 62)
			{
				auto attackerPos = KingHandling.GetAttackerPosition();
				Tiles[attackerPos.Row][attackerPos.Col].AssignTexture(TextureMap[Tiles[attackerPos.Row][attackerPos.Col].GetPieceName()]);
				Tiles[attackerPos.Row][attackerPos.Col].SetPossibleMove(false);
				NewPossibleMoves = KingHandling.GetKingMoves();
				PieceChosen = true;
				ChosenPiecePosition = ut::Position(bodyguardsPos[i].Row, bodyguardsPos[i].Col);
				for (auto& m : NewPossibleMoves)
				{
					if (m.Equals(attackerPos))
					{
						if (!KingHandling.IsAttackingPieceProtected(attackerPos, Tiles))
						{
							Tiles[m.Row][m.Col].AssignTexture(TextureMap["Attacked" + Tiles[attackerPos.Row][attackerPos.Col].GetPieceName()]);
							Tiles[m.Row][m.Col].SetPossibleMove(true);
						}
					}
					else if (!m.Equals(attackerPos))
					{
						if (!Tiles[m.Row][m.Col].IsPieceInside())
						{
							Tiles[m.Row][m.Col].AssignTexture(TextureMap["PossibleMove"]);
							Tiles[m.Row][m.Col].SetPossibleMove(true);
						}
					}
				}
			}
			else
			{
				if (piece->GetPieceId() > 62)
				{
					NewPossibleMoves = Utility->CalculateSpecialPossibleMoves(piece, bodyguardsPos[i], Tiles);
				}
				else
				{
					NewPossibleMoves = Utility->CalculatePossibleMoves(piece, bodyguardsPos[i], Tiles);
				}
				auto attackerPos = KingHandling.GetAttackerPosition();
				for (auto& m : NewPossibleMoves)
				{
					if (m.Equals(attackerPos))
					{
						Tiles[m.Row][m.Col].AssignTexture(TextureMap["Attacked" + Tiles[attackerPos.Row][attackerPos.Col].GetPieceName()]);
						Tiles[m.Row][m.Col].SetPossibleMove(true);
						ChosenPiecePosition = ut::Position(bodyguardsPos[i].Row, bodyguardsPos[i].Col);
						PieceChosen = true;
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i < sacrifices.size(); i++)
	{
		if (Tiles[sacrifices[i].Row][sacrifices[i].Col].ContainsPieceInBounds(mousePos))
		{
			if (PieceChosen)
			{
				Utility->ClearPossibleMoves(ChosenPiecePosition, NewPossibleMoves, Tiles, TextureMap);
				BoolDropPiece = false;
				PieceChosen = false;
			}
			game::IPiece* piece = Tiles[sacrifices[i].Row][sacrifices[i].Col].GetPiece();
			if (piece->GetPieceId() > 62)
			{
				NewPossibleMoves = Utility->CalculateSpecialPossibleMoves(piece, sacrifices[i], Tiles);
			}
			else
			{
				NewPossibleMoves = Utility->CalculatePossibleMoves(piece, sacrifices[i], Tiles);
			}
			std::vector<ut::Position> res;
			for (int i = 0; i < NewPossibleMoves.size(); i++)
			{
				for (int j = 0; j < endangeredMoves.size(); j++)
				{
					if (NewPossibleMoves[i].Equals(endangeredMoves[j]))
					{
						res.push_back(NewPossibleMoves[i]);
						break;
					}
				}
			}
			NewPossibleMoves.clear();
			for (auto& r : res)
			{
				NewPossibleMoves.push_back(r);
			}
			ChosenPiecePosition = ut::Position(sacrifices[i].Row, sacrifices[i].Col);
			PieceChosen = true;
			Utility->DisplayPossibleMoves(NewPossibleMoves, piece->IsFacingUp(), Tiles, TextureMap);
		}
	}
	for (auto& d : drops)
	{
		if (d.second.ContainsPieceInBounds(mousePos))
		{
			CapturedPieceMapKey = d.first;
			if (PieceChosen)
			{
				Utility->ClearPossibleMoves(ChosenPiecePosition, NewPossibleMoves, Tiles, TextureMap);
				BoolDropPiece = false;
				PieceChosen = false;
			}
			ut::BoardTile tile = d.second;
			game::IPiece* piece = tile.GetPiece();
			PieceChosen = true;
			bool dropPieceFacesUp = tile.GetIsPieceFacingUp();
			std::vector<ut::Position> dropMoves = Utility->CalculateDropPossibleMoves(piece, Tiles);
			std::vector<ut::Position> goodDropMoves;
			for (auto& e : endangeredMoves)
			{
				for (int i = 0; i < dropMoves.size(); i++)
				{
					if (e.Equals(dropMoves[i]))
					{
						goodDropMoves.push_back(e);
						break;
					}
				}
			}
			NewPossibleMoves.clear();
			NewPossibleMoves = goodDropMoves;
			Utility->DisplayPossibleMoves(NewPossibleMoves, tile.GetIsPieceFacingUp(), Tiles, TextureMap);
			BoolDropPiece = true;
			DroppedPiece = tile.GetPiece();
		}
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (Tiles[i][j].ContainsPossibleMoveInBounds(mousePos) && PieceChosen)
			{
				game::IPiece* chosenPiece;
				ut::Position destPosition = ut::Position(i, j);
				if (!BoolDropPiece)
				{
					chosenPiece = Tiles[ChosenPiecePosition.Row][ChosenPiecePosition.Col].GetPiece();

					if (Tiles[i][j].IsPieceInside() && Utility->IsEnemy(PlayerB, i, j, Tiles))
					{
						if (PlayerB)
						{
							CapturePiece(CampB, Tiles[i][j].GetPiece());
						}
						else
						{
							CapturePiece(CampA, Tiles[i][j].GetPiece());
						}
					}
				}
				else
				{
					chosenPiece = DroppedPiece;
					if (PlayerB)
					{
						CampB.GetCapturedPiece(CapturedPieceMapKey);
						CampB.FixCampAfterCapture(TextureMap);
					}
					else
					{
						CampA.GetCapturedPiece(CapturedPieceMapKey);
						CampA.FixCampAfterCapture(TextureMap);
					}
				}
				PieceChosen = false;
				BoolKingInDanger = false;
				PlayerB = !PlayerB;
				if (!BoolDropPiece)
				{
					MovePiece(ChosenPiecePosition, destPosition);
				}
				else
				{
					DropPiece(chosenPiece, destPosition);
					BoolDropPiece = false;
				}
				if (chosenPiece->GetPieceId() == 62)
				{
					chosenPiece->RestorePiece();
					kingPos.UpdatePosition(destPosition);
					Tiles[destPosition.Row][destPosition.Col].AssignTexture(TextureMap[chosenPiece->GetPieceName()]);
				}
				else
				{
					Tiles[kingPos.Row][kingPos.Col].AssignTexture(TextureMap[Tiles[kingPos.Row][kingPos.Col].GetPieceName()]);
				}
				break;
			}
		}
	}
}

Board::~Board()
{
	delete Utility;
}