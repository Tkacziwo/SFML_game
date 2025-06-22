#include "BoardUtilities.h"

using namespace ut;

bool BoardUtilities::IsInBoard(int row, int col)
{
	if (row > -1 && row < 9 && col > -1 && col < 9)
		return true;
	else
		return false;
}

bool ut::BoardUtilities::IsInBoard(const ut::Position& pos)
{
	if (pos.Row > -1 && pos.Row < 9 && pos.Col > -1 && pos.Col < 9)
		return true;
	else
		return false;
}

bool BoardUtilities::IsEnemy(bool sourcePieceFacesUp, int row, int col, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	bool destPieceFacesUp = tiles[row][col].GetIsPieceFacingUp();
	if (destPieceFacesUp == sourcePieceFacesUp)
		return false;
	else
		return true;
}

bool ut::BoardUtilities::IsPieceInsideEnemy(const bool srcPieceFacesUp, ut::Position& checkedPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	if (tiles[checkedPos.Row][checkedPos.Col].IsPieceInside())
	{
		const bool checkedPieceFacesUp = tiles[checkedPos.Row][checkedPos.Col].GetIsPieceFacingUp();
		if (srcPieceFacesUp == checkedPieceFacesUp)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void BoardUtilities::LoadPieceMovesetFromFile(std::map<std::string, std::vector<std::vector<int>>>& simpleMovesetMap, std::map<std::string, std::vector<std::vector<int>>>& specialMovesetMap, std::map<std::string, std::vector<std::vector<int>>>& specialPiecePromotion)
{
	std::fstream file("moves.txt");
	std::string handler;
	size_t wordEndPosition{};

	while (std::getline(file, handler))
	{
		wordEndPosition = handler.find(' ');
		auto word = handler.substr(0, wordEndPosition);
		std::vector<std::vector<int>> moves;
		handler = handler.substr(wordEndPosition + 1);
		int k{ 0 };
		int iterations{ 0 };
		if (handler.size() / 3 == 3)
		{
			iterations = 3;
		}
		else
		{
			iterations = 4;
		}
		for (int i = 0; i < iterations; i++)
		{
			std::vector<int> temp;
			for (int j = 0; j < 3; j++)
			{
				auto chr = new char(handler[k]);
				temp.push_back(static_cast<int>(std::atoi(chr)));

				delete chr;
				k++;
			}
			moves.push_back(temp);
			temp.clear();
		}

		if (handler.size() / 3 == 3)
		{
			simpleMovesetMap.insert(std::make_pair(word, moves));
		}
		else
		{
			specialMovesetMap.insert(std::make_pair(word, moves));
		}
	}
	file.close();

	std::fstream specialMoves("SpecialMoves.txt");

	while (std::getline(specialMoves, handler))
	{
		wordEndPosition = handler.find(' ');
		auto word = handler.substr(0, wordEndPosition);
		handler = handler.substr(wordEndPosition + 1);
		std::vector<std::vector<int>> moves;
		int k{ 0 };
		for (int i = 0; i < 4; i++)
		{
			std::vector<int> temp;
			for (int j = 0; j < 3; j++)
			{
				auto chr = new char(handler[k]);
				temp.push_back(static_cast<int>(std::atoi(chr)));

				delete chr;
				k++;
			}
			moves.push_back(temp);
			temp.clear();
		}
		specialPiecePromotion.insert(std::make_pair(word, moves));
	}
	specialMoves.close();
}

void BoardUtilities::InitBackground(sf::Vector2f& size, sf::RectangleShape& background)
{
	background.setSize(size);
	background.setFillColor(sf::Color(221, 212, 163));
}

void ut::BoardUtilities::ClearPossibleMoves(const ut::Position& pos, std::vector<ut::Position>& moves, std::vector<std::vector<ut::BoardTile>>& tiles, std::map<std::string, sf::Texture>& textureMap)
{
	for (int i = 0; i < moves.size(); i++)
	{
		ut::Position p = moves[i].GetPosition();

		if (tiles[p.Row][p.Col].IsPieceInside())
		{
			tiles[p.Row][p.Col].SetPossibleMove(false);
			tiles[p.Row][p.Col].AssignTexture(textureMap[tiles[p.Row][p.Col].GetPieceName()]);
		}
		else if (p.Row != pos.Row || p.Col != pos.Col)
		{
			tiles[p.Row][p.Col].ClearTexture();
		}
	}
	moves.clear();
}

std::vector<int> ut::BoardUtilities::CheckGoodColumnsForDrop(const std::string& name, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<int> goodColumns;
	for (int col = 0; col < 9; col++)
	{
		bool found = false;
		for (int row = 0; row < 9; row++)
		{
			if (tiles[row][col].IsPieceInside())
			{
				auto checkedPieceName = tiles[row][col].GetPieceName();
				if (name == checkedPieceName)
				{
					found = true;
				}
			}
		}
		if (!found)
		{
			goodColumns.push_back(col);
		}
	}
	return goodColumns;
}

std::vector<ut::Position> ut::BoardUtilities::CalculateDropPossibleMoves(game::IPiece* piece, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<ut::Position> moves;
	auto pieceName = piece->GetPieceName();
	if (pieceName == "Pawn" || pieceName == "DarkPawn")
	{
		std::vector<int> goodColumns = CheckGoodColumnsForDrop(pieceName, tiles);
		for (int i = 0; i < goodColumns.size(); i++)
		{
			for (int row = 0; row < 9; row++)
			{
				if (!tiles[row][goodColumns[i]].IsPieceInside())
				{
					moves.push_back(ut::Position(row, goodColumns[i]));
				}
			}
		}
	}
	else
	{
		for (int col = 0; col < 9; col++)
		{
			for (int row = 0; row < 9; row++)
			{
				if (!tiles[row][col].IsPieceInside())
				{
					moves.push_back(ut::Position(row, col));
				}
			}
		}
	}
	return moves;
}

std::vector<ut::Position> ut::BoardUtilities::CalculatePossibleMoves(game::IPiece* piece, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<ut::Position> moves;
	std::vector<std::vector<int>> moveset = piece->GetPieceMoveset();
	std::vector<std::thread> threads;
	std::mutex movesMutex;
	for (int col = 0; col < 3; col++)
	{
		threads.push_back(std::thread([this, &movesMutex, &moveset, col, srcPos, &moves, &tiles, &piece]()
			{
				movesMutex.lock();
				for (int row = 0; row < 3; row++)
				{
					if (moveset[row][col] == 1)
					{
						int destRow = srcPos.Row - 1 + row;
						int destCol = srcPos.Col - 1 + col;

						ut::Position destPosition(destRow, destCol);

						if (IsInBoard(destRow, destCol))
						{
							if (!tiles[destRow][destCol].IsPieceInside())
							{
								moves.push_back(destPosition);
							}
							else if (tiles[destRow][destCol].IsPieceInside()
								&& IsEnemy(piece->IsFacingUp(), destRow, destCol, tiles))
							{
								moves.push_back(destPosition);
							}
						}
					}
				}
				movesMutex.unlock();
			}));
	}
	for (auto& t : threads)
	{
		t.join();
	}
	return moves;
}

std::vector<ut::Position> ut::BoardUtilities::CalculateProtectionMoves(game::IPiece* piece, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<ut::Position> moves;
	std::vector<std::vector<int>> moveset = piece->GetPieceMoveset();
	std::vector<std::thread> threads;
	std::mutex movesMutex;
	for (int col = 0; col < 3; col++)
	{
		threads.push_back(std::thread([this, &movesMutex, &moveset, col, srcPos, &moves]()
			{
				movesMutex.lock();
				for (int row = 0; row < 3; row++)
				{
					if (moveset[row][col] == 1)
					{
						int destRow = srcPos.Row - 1 + row;
						int destCol = srcPos.Col - 1 + col;

						ut::Position destPosition(destRow, destCol);

						if (IsInBoard(destRow, destCol))
						{
							moves.push_back(destPosition);
						}
					}
				}
				movesMutex.unlock();
			}));
	}
	for (auto& t : threads)
	{
		t.join();
	}
	return moves;
}

std::vector<ut::Position> ut::BoardUtilities::CalculateSpecialPossibleMoves(game::IPiece* piece, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<ut::Position> moves;
	std::vector<std::vector<int>> moveset = piece->GetPieceMoveset();
	std::string pieceName = piece->GetPieceName();
	if (pieceName == "GoldGeneral" || pieceName == "DarkGoldGeneral")
	{
		moves = CalculatePossibleMoves(piece, srcPos, tiles);
	}
	else
	{
		int subtractor = (piece->GetPieceName() == "DarkHorse") ? 1 : 2;
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				int destCol{ srcPos.Col - 1 + col };
				int destRow{ srcPos.Row - subtractor + row };
				ut::Position destPos(destRow, destCol);

				if (IsInBoard(destPos))
				{
					if (moveset[row][col] == 1 && piece->GetPieceName() != "Horse")
					{
						auto temp = ExtendSpecialPieceMoves(srcPos, destPos, tiles);
						for (auto& t : temp)
						{
							moves.push_back(t);
						}
					}
					else if (moveset[row][col] == 2)
					{
						if (!tiles[destRow][destCol].IsPieceInside())
						{
							moves.push_back(destPos);
						}
						else
						{
							if (IsEnemy(piece->IsFacingUp(), destRow, destCol, tiles))
							{
								moves.push_back(destPos);
							}
						}
					}
				}
			}
		}
	}
	return moves;
}

bool ut::BoardUtilities::RookProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	int checkUp = srcPos.Row - 1, checkRight = srcPos.Col + 1,
		checkDown = srcPos.Row + 1, checkLeft = srcPos.Col - 1;
	if (IsInBoard(checkUp, srcPos.Col))
	{
		for (int i = checkUp; i > 0; i--)
		{
			if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles)
				&& tiles[i][srcPos.Col].GetPieceId() == 64)
			{
				return true;
			}
		}
	}
	if (IsInBoard(checkDown, srcPos.Col))
	{
		for (int i = checkDown; i < 9; i++)
		{
			if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles)
				&& tiles[i][srcPos.Col].GetPieceId() == 64)
			{
				return true;
			}
		}
	}
	if (IsInBoard(srcPos.Row, checkRight))
	{
		for (int i = checkRight; i < 9; i++)
		{
			if (tiles[srcPos.Row][i].IsPieceInside() && !IsEnemy(srcFacesUp, srcPos.Row, i, tiles)
				&& tiles[srcPos.Row][i].GetPieceId() == 64)
			{
				return true;
			}
		}
	}
	if (IsInBoard(srcPos.Row, checkLeft))
	{
		for (int i = checkLeft; i > 0; i--)
		{
			if (tiles[srcPos.Row][i].IsPieceInside() && !IsEnemy(srcFacesUp, srcPos.Row, i, tiles)
				&& tiles[srcPos.Row][i].GetPieceId() == 64)
			{
				return true;
			}
		}
	}
	return false;
}

bool ut::BoardUtilities::BishopProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	ut::Position checkUpRight(srcPos.Row - 1, srcPos.Col + 1), checkDownRight(srcPos.Row + 1, srcPos.Col + 1),
		checkDownLeft(srcPos.Row + 1, srcPos.Col - 1), checkUpLeft(srcPos.Row - 1, srcPos.Col - 1);
	if (IsInBoard(checkUpRight))
	{
		int shift = 0;
		for (int i = checkUpRight.Row; i > 0; i--)
		{
			if (IsInBoard(checkUpRight.Row - shift, checkUpRight.Col + shift))
			{
				if (tiles[checkUpRight.Row - shift][checkUpRight.Col + shift].IsPieceInside() &&
					!IsEnemy(srcFacesUp, checkUpRight.Row - shift, checkUpRight.Col + shift, tiles)
					&& tiles[checkUpRight.Row - shift][checkUpRight.Col + shift].GetPieceId() == 63)
				{
					return true;
				}
				shift++;
			}
		}
	}
	if (IsInBoard(checkDownRight))
	{
		int shift = 0;
		for (int i = checkDownRight.Row; i < 9; i++)
		{
			if (IsInBoard(checkDownRight.Row + shift, checkDownRight.Col + shift))
			{
				if (tiles[checkDownRight.Row + shift][checkDownRight.Col + shift].IsPieceInside() &&
					!IsEnemy(srcFacesUp, checkDownRight.Row + shift, checkDownRight.Col + shift, tiles)
					&& tiles[checkDownRight.Row + shift][checkDownRight.Col + shift].GetPieceId() == 63)
				{
					return true;
				}
				shift++;
			}
		}
	}
	if (IsInBoard(checkDownLeft))
	{
		int shift = 0;
		for (int i = checkDownLeft.Row; i < 9; i++)
		{
			if (IsInBoard(checkDownLeft.Row + shift, checkDownLeft.Col - shift))
			{
				if (tiles[checkDownLeft.Row + shift][checkDownLeft.Col - shift].IsPieceInside() &&
					!IsEnemy(srcFacesUp, checkDownLeft.Row + shift, checkDownLeft.Col - shift, tiles)
					&& tiles[checkDownLeft.Row + shift][checkDownLeft.Col - shift].GetPieceId() == 63)
				{
					return true;
				}
				shift++;
			}
		}
	}
	if (IsInBoard(checkUpLeft))
	{
		int shift = 0;
		for (int i = checkUpLeft.Row; i > 0; i--)
		{
			if (IsInBoard(checkUpLeft.Row - shift, checkUpLeft.Col - shift))
			{
				if (tiles[checkUpLeft.Row - shift][checkUpLeft.Col - shift].IsPieceInside() &&
					!IsEnemy(srcFacesUp, checkUpLeft.Row - shift, checkUpLeft.Col - shift, tiles)
					&& tiles[checkUpLeft.Row - shift][checkUpLeft.Col - shift].GetPieceId() == 63)
				{
					return true;
				}
				shift++;
			}
		}
	}
	return false;
}

bool ut::BoardUtilities::LanceProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	int checkUp = srcPos.Row - 1, checkDown = srcPos.Row + 1;
	if (IsInBoard(checkUp, srcPos.Col))
	{
		for (int i = srcPos.Row; i >= 0; i--)
		{
			if (IsInBoard(i, srcPos.Col))
			{
				if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles)
					&& tiles[i][srcPos.Col].GetPieceId() == 65)
				{
					return true;
				}
			}
		}
	}
	if (IsInBoard(checkDown, srcPos.Col))
	{
		for (int i = srcPos.Row; i < 9; i++)
		{
			if (IsInBoard(i, srcPos.Col))
			{
				if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles)
					&& tiles[i][srcPos.Col].GetPieceId() == 65)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ut::BoardUtilities::HorseProtectionMoves(bool srcFacesUp, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	if (srcFacesUp)
	{
		ut::Position horsePos1(srcPos.Row + 2, srcPos.Col - 1);
		ut::Position horsePos2(srcPos.Row + 2, srcPos.Col + 1);
		if (IsInBoard(horsePos1)
			&& tiles[horsePos1.Row][horsePos1.Col].IsPieceInside()
			&& tiles[horsePos1.Row][horsePos1.Col].GetPieceId() == 66)
		{
			if (!IsEnemy(srcFacesUp, horsePos1.Row, horsePos1.Col, tiles))
			{
				return true;
			}
		}
		if (IsInBoard(horsePos2)
			&& tiles[horsePos2.Row][horsePos2.Col].IsPieceInside()
			&& tiles[horsePos2.Row][horsePos2.Col].GetPieceId() == 66)
		{
			if (!IsEnemy(srcFacesUp, horsePos2.Row, horsePos2.Col, tiles))
			{
				return true;
			}
		}
	}
	else
	{
		ut::Position horsePos1(srcPos.Row - 2, srcPos.Col - 1);
		ut::Position horsePos2(srcPos.Row - 2, srcPos.Col + 1);
		if (IsInBoard(horsePos1)
			&& tiles[horsePos1.Row][horsePos1.Col].IsPieceInside()
			&& tiles[horsePos1.Row][horsePos1.Col].GetPieceId() == 66)
		{
			if (!IsEnemy(srcFacesUp, horsePos1.Row, horsePos1.Col, tiles))
			{
				return true;
			}
		}
		if (IsInBoard(horsePos2)
			&& tiles[horsePos2.Row][horsePos2.Col].IsPieceInside()
			&& tiles[horsePos2.Row][horsePos2.Col].GetPieceId() == 66)
		{
			if (!IsEnemy(srcFacesUp, horsePos2.Row, horsePos2.Col, tiles))
			{
				return true;
			}
		}
	}
	return false;
}

std::vector<ut::Position> ut::BoardUtilities::ExtendSpecialPieceMoves(ut::Position srcPos, ut::Position destPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	int sourceRow = srcPos.Row, sourceCol = srcPos.Col,
		destRow = destPos.Row, destCol = destPos.Col;
	std::vector<ut::Position> moves;
	if (destRow < sourceRow && sourceCol == destCol)
	{
		int dummyShift{ 0 };
		for (int i = destRow; i >= 0; i--)
		{
			if (dummyShift != -999)
			{
				ut::Position temp = SpecialPieceAttackCheck(srcPos, ut::Position(i, sourceCol), dummyShift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (destRow > sourceRow && sourceCol == destCol)
	{
		int dummyShift{ 0 };
		for (int i = destRow; i <= 8; i++)
		{
			if (dummyShift != -999)
			{
				ut::Position temp = SpecialPieceAttackCheck(srcPos, ut::Position(i, sourceCol), dummyShift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (destRow == sourceRow && sourceCol > destCol)
	{
		int dummyShift{ 0 };
		for (int i = destCol; i >= 0; i--)
		{
			if (dummyShift != -999)
			{
				ut::Position temp = SpecialPieceAttackCheck(srcPos, ut::Position(sourceRow, i), dummyShift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (destRow == sourceRow && sourceCol < destCol)
	{
		int dummyShift{ 0 };
		for (int i = destCol; i <= 8; i++)
		{
			if (dummyShift != -999)
			{
				ut::Position temp = SpecialPieceAttackCheck(srcPos, ut::Position(sourceRow, i), dummyShift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (destRow < sourceRow && destCol < sourceCol)
	{
		int shift{ 0 };
		for (int i = destRow; i >= 0; i--)
		{
			if (shift != -999)
			{
				int checkedRow{ destRow - shift },
					checkedCol{ destCol - shift };
				ut::Position checkedPos(checkedRow, checkedCol);
				ut::Position temp = SpecialPieceAttackCheck(srcPos, checkedPos, shift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (destRow < sourceRow && destCol > sourceCol)
	{
		int shift{ 0 };
		for (int i = destRow; i >= 0; i--)
		{
			if (shift != -999)
			{
				int checkedRow{ destRow - shift },
					checkedCol{ destCol + shift };
				ut::Position checkedPos(checkedRow, checkedCol);
				ut::Position temp = SpecialPieceAttackCheck(srcPos, checkedPos, shift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
		}
	}
	else if (destRow > sourceRow && destCol < sourceCol)
	{
		int shift{ 0 };
		for (int i = destRow; i <= 8; i++)
		{
			if (shift != -999)
			{
				int checkedRow{ destRow + shift },
					checkedCol{ destCol - shift };
				ut::Position checkedPos(checkedRow, checkedCol);
				ut::Position temp = SpecialPieceAttackCheck(srcPos, checkedPos, shift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
		}
	}
	else if (destRow > sourceRow && destCol > sourceCol)
	{
		int shift{ 0 };
		for (int i = destRow; i <= 8; i++)
		{
			if (shift != -999)
			{
				int checkedRow{ destRow + shift },
					checkedCol{ destCol + shift };
				ut::Position checkedPos(checkedRow, checkedCol);
				ut::Position temp = SpecialPieceAttackCheck(srcPos, checkedPos, shift, tiles);
				if (IsInBoard(temp))
				{
					moves.push_back(temp);
				}
				else
				{
					break;
				}
			}
		}
	}
	return moves;
}

std::vector<ut::Position> ut::BoardUtilities::MaximumExtension(int pieceId, ut::Position srcPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<ut::Position> moves;
	bool srcFacesUp = tiles[srcPos.Row][srcPos.Col].GetIsPieceFacingUp();
	if (pieceId == 65)
	{
		if (srcFacesUp)
		{
			int checkUp = srcPos.Row - 1;
			if (IsInBoard(checkUp, srcPos.Col))
			{
				for (int i = srcPos.Row; i > 0; i--)
				{
					if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && !srcPos.Equals(ut::Position(i, srcPos.Col)))
					{
						break;
					}
					if (tiles[i][srcPos.Col].IsPieceInside() && IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && tiles[i][srcPos.Col].GetPieceId() != 62)
					{
						break;
					}
					ut::Position p(i, srcPos.Col);
					moves.push_back(p);
				}
			}
		}
		else
		{
			int checkDown = srcPos.Row + 1;
			if (IsInBoard(checkDown, srcPos.Col))
			{
				for (int i = srcPos.Row; i < 9; i++)
				{
					if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && !srcPos.Equals(ut::Position(i, srcPos.Col)))
					{
						break;
					}
					if (tiles[i][srcPos.Col].IsPieceInside() && IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && tiles[i][srcPos.Col].GetPieceId() != 62)
					{
						break;
					}
					ut::Position p(i, srcPos.Col);
					moves.push_back(p);
				}
			}
		}
	}
	else if (pieceId == 64)
	{
		int checkUp = srcPos.Row - 1;
		int checkRight = srcPos.Col + 1;
		int checkDown = srcPos.Row + 1;
		int checkLeft = srcPos.Col - 1;
		if (IsInBoard(checkUp, srcPos.Col))
		{
			for (int i = srcPos.Row; i > 0; i--)
			{
				if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && !srcPos.Equals(ut::Position(i, srcPos.Col)))
				{
					break;
				}
				if (tiles[i][srcPos.Col].IsPieceInside() && IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && tiles[i][srcPos.Col].GetPieceId() != 62)
				{
					break;
				}
				ut::Position p(i, srcPos.Col);
				moves.push_back(p);
			}
		}
		if (IsInBoard(checkDown, srcPos.Col))
		{
			for (int i = srcPos.Row; i < 9; i++)
			{
				if (tiles[i][srcPos.Col].IsPieceInside() && !IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && !srcPos.Equals(ut::Position(i, srcPos.Col)))
				{
					break;
				}
				if (tiles[i][srcPos.Col].IsPieceInside() && IsEnemy(srcFacesUp, i, srcPos.Col, tiles) && tiles[i][srcPos.Col].GetPieceId() != 62)
				{
					break;
				}
				ut::Position p(i, srcPos.Col);
				moves.push_back(p);
			}
		}
		if (IsInBoard(srcPos.Row, checkRight))
		{
			for (int i = srcPos.Col; i < 9; i++)
			{
				if (tiles[srcPos.Row][i].IsPieceInside() && !IsEnemy(srcFacesUp, srcPos.Row, i, tiles) && !srcPos.Equals(ut::Position(srcPos.Row, i)))
				{
					break;
				}
				if (tiles[srcPos.Row][i].IsPieceInside() && IsEnemy(srcFacesUp, srcPos.Row, i, tiles) && tiles[srcPos.Row][i].GetPieceId() != 62)
				{
					break;
				}
				ut::Position p(srcPos.Row, i);
				moves.push_back(p);
			}
		}
		if (IsInBoard(srcPos.Row, checkLeft))
		{
			for (int i = checkRight; i > 0; i--)
			{
				if (tiles[srcPos.Row][i].IsPieceInside() && !IsEnemy(srcFacesUp, srcPos.Row, i, tiles) && !srcPos.Equals(ut::Position(srcPos.Row, i)))
				{
					break;
				}
				if (tiles[srcPos.Row][i].IsPieceInside() && IsEnemy(srcFacesUp, srcPos.Row, i, tiles) && tiles[srcPos.Row][i].GetPieceId() != 62)
				{
					break;
				}
				ut::Position p(srcPos.Row, i);
				moves.push_back(p);
			}
		}
	}
	else if (pieceId == 63)
	{
		ut::Position checkUpRight(srcPos.Row - 1, srcPos.Col + 1);
		ut::Position checkDownRight(srcPos.Row + 1, srcPos.Col + 1);
		ut::Position checkDownLeft(srcPos.Row + 1, srcPos.Col - 1);
		ut::Position checkUpLeft(srcPos.Row - 1, srcPos.Col - 1);

		if (IsInBoard(checkUpRight))
		{
			int shift = 0;
			for (int i = checkUpRight.Row; i > 0; i--)
			{
				int row = checkUpRight.Row - shift;
				int col = checkUpRight.Col + shift;
				if (IsInBoard(row, col))
				{
					if (tiles[row][col].IsPieceInside() && !IsEnemy(srcFacesUp, row, col, tiles) && !srcPos.Equals(ut::Position(row, col)))
					{
						break;
					}
					if (tiles[row][col].IsPieceInside() && IsEnemy(srcFacesUp, row, col, tiles) && tiles[row][col].GetPieceId() != 62)
					{
						break;
					}
					moves.push_back(ut::Position(row, col));
					shift++;
				}
			}
		}
		if (IsInBoard(checkDownRight))
		{
			int shift = 0;
			for (int i = checkDownRight.Row; i < 9; i++)
			{
				int row = checkDownRight.Row + shift;
				int col = checkDownRight.Col + shift;
				if (IsInBoard(row, col))
				{
					if (tiles[row][col].IsPieceInside() && !IsEnemy(srcFacesUp, row, col, tiles) && !srcPos.Equals(ut::Position(row, col)))
					{
						break;
					}
					if (tiles[row][col].IsPieceInside() && IsEnemy(srcFacesUp, row, col, tiles) && tiles[row][col].GetPieceId() != 62)
					{
						break;
					}
					moves.push_back(ut::Position(row, col));
					shift++;
				}
			}
		}
		if (IsInBoard(checkDownLeft))
		{
			int shift = 0;
			for (int i = checkDownLeft.Row; i < 9; i++)
			{
				int row = checkDownLeft.Row + shift;
				int col = checkDownLeft.Col - shift;
				if (IsInBoard(row, col))
				{
					if (tiles[row][col].IsPieceInside() && !IsEnemy(srcFacesUp, row, col, tiles) && !srcPos.Equals(ut::Position(row, col)))
					{
						break;
					}
					if (tiles[row][col].IsPieceInside() && IsEnemy(srcFacesUp, row, col, tiles) && tiles[row][col].GetPieceId() != 62)
					{
						break;
					}
					moves.push_back(ut::Position(row, col));
					shift++;
				}
			}
		}
		if (IsInBoard(checkUpLeft))
		{
			int shift = 0;
			for (int i = checkUpLeft.Row; i > 0; i--)
			{
				int row = checkUpLeft.Row - shift;
				int col = checkUpLeft.Col - shift;
				if (IsInBoard(row, col))
				{
					if (tiles[row][col].IsPieceInside() && !IsEnemy(srcFacesUp, row, col, tiles) && !srcPos.Equals(ut::Position(row, col)))
					{
						break;
					}
					if (tiles[row][col].IsPieceInside() && IsEnemy(srcFacesUp, row, col, tiles) && tiles[row][col].GetPieceId() != 62)
					{
						break;
					}
					moves.push_back(ut::Position(row, col));
					shift++;
				}
			}
		}
	}

	return moves;
}

std::vector<ut::Position> ut::BoardUtilities::MaximumAbsoluteExtensionRook(ut::Position kingPos, std::vector<std::vector<ut::BoardTile>>& tiles, ut::Position& rookPos)
{
	std::vector<ut::Position> resMoves;
	int checkUp = kingPos.Row - 1, checkRight = kingPos.Col + 1,
		checkDown = kingPos.Row + 1, checkLeft = kingPos.Col - 1;
	game::IPiece* king = tiles[kingPos.Row][kingPos.Col].GetPiece();
	std::vector<std::thread> threads;
	std::mutex movesMutex;

	if (IsInBoard(checkUp, kingPos.Col))
	{
		threads.push_back(std::thread([this, &movesMutex, &rookPos, &resMoves, &kingPos, &tiles, &king]()
			{
				movesMutex.lock();
				std::vector<ut::Position> moves;
				for (int i = kingPos.Row; i >= 0; i--)
				{
					ut::Position p(i, kingPos.Col);
					if (IsInBoard(p))
					{
						moves.push_back(p);
						if (tiles[i][kingPos.Col].IsPieceInside() && tiles[i][kingPos.Col].GetPieceId() == 64
							&& IsEnemy(king->IsFacingUp(), i, kingPos.Col, tiles))
						{
							rookPos = p;
							for (auto& m : moves)
							{
								resMoves.push_back(m);
							}
							break;
						}
					}
				}
				movesMutex.unlock();
			}));
	}
	if (IsInBoard(checkDown, kingPos.Col))
	{
		threads.push_back(std::thread([this, &movesMutex, &rookPos, &resMoves, &kingPos, &tiles, &king]()
			{
				movesMutex.lock();
				std::vector<ut::Position> moves;
				for (int i = kingPos.Row; i < 9; i++)
				{
					ut::Position p(i, kingPos.Col);
					if (IsInBoard(p))
					{
						moves.push_back(p);
						if (tiles[i][kingPos.Col].IsPieceInside() && tiles[i][kingPos.Col].GetPieceId() == 64
							&& IsEnemy(king->IsFacingUp(), i, kingPos.Col, tiles))
						{
							rookPos = p;
							for (auto& m : moves)
							{
								resMoves.push_back(m);
							}
							break;
						}
					}
				}
				movesMutex.unlock();
			}));
	}
	if (IsInBoard(kingPos.Row, checkRight))
	{
		threads.push_back(std::thread([this, &movesMutex, &rookPos, &resMoves, &kingPos, &tiles, &king]()
			{
				movesMutex.lock();
				std::vector<ut::Position> moves;
				for (int i = kingPos.Col; i < 9; i++)
				{
					ut::Position p(kingPos.Row, i);
					if (IsInBoard(p))
					{
						moves.push_back(p);
						if (tiles[kingPos.Row][i].IsPieceInside() && tiles[kingPos.Row][i].GetPieceId() == 64
							&& IsEnemy(king->IsFacingUp(), kingPos.Row, i, tiles))
						{
							rookPos = p;
							for (auto& m : moves)
							{
								resMoves.push_back(m);
							}
							break;
						}
					}
				}
				movesMutex.unlock();
			}));
	}
	if (IsInBoard(kingPos.Row, checkLeft))
	{
		threads.push_back(std::thread([this, &movesMutex, &rookPos, &resMoves, &kingPos, &tiles, &king]()
			{
				movesMutex.lock();
				std::vector<ut::Position> moves;
				for (int i = kingPos.Col; i >= 0; i--)
				{
					ut::Position p(kingPos.Row, i);
					if (IsInBoard(p))
					{
						moves.push_back(p);
						if (tiles[kingPos.Row][i].IsPieceInside() && tiles[kingPos.Row][i].GetPieceId() == 64
							&& IsEnemy(king->IsFacingUp(), kingPos.Row, i, tiles))
						{
							rookPos = p;
							for (auto& m : moves)
							{
								resMoves.push_back(m);
							}
							break;
						}
					}
				}
				movesMutex.unlock();
			}));
	}
	for (auto& t : threads)
	{
		t.join();
	}
	return resMoves;
}

std::vector<ut::Position> ut::BoardUtilities::MaximumAbsoluteExtensionBishop(ut::Position kingPos, std::vector<std::vector<ut::BoardTile>>& tiles, ut::Position& bishopPos)
{
	std::vector<ut::Position> resMoves;
	ut::Position checkUpRight(kingPos.Row - 1, kingPos.Col + 1), checkDownRight(kingPos.Row + 1, kingPos.Col + 1),
		checkDownLeft(kingPos.Row + 1, kingPos.Col - 1), checkUpLeft(kingPos.Row - 1, kingPos.Col - 1);
	game::IPiece* king = tiles[kingPos.Row][kingPos.Col].GetPiece();
	std::vector<std::thread> threads;
	std::mutex movesMutex;
	if (IsInBoard(checkUpRight))
	{
		threads.push_back(std::thread([this, &movesMutex, &checkUpRight, &tiles, &king, &bishopPos, &resMoves]()
			{
				movesMutex.lock();
				int shift = 0;
				std::vector<ut::Position> moves;
				for (int i = checkUpRight.Row; i > 0; i--)
				{
					int row = checkUpRight.Row - shift;
					int col = checkUpRight.Col + shift;
					ut::Position p(row, col);
					if (IsInBoard(row, col))
					{
						moves.push_back(p);
						if (tiles[row][col].IsPieceInside() && tiles[row][col].GetPieceId() == 63
							&& IsEnemy(king->IsFacingUp(), row, col, tiles))
						{
							bishopPos = p;
							for (auto& m : moves)
							{
								resMoves.push_back(m);
							}
							break;
						}
					}
					shift++;
				}
				movesMutex.unlock();
			}));
	}
	if (IsInBoard(checkDownRight))
	{
		threads.push_back(std::thread([this, &movesMutex, &checkDownRight, &tiles, &king, &bishopPos, &resMoves]()
			{
				movesMutex.lock();
				int shift = 0;
				std::vector<ut::Position> moves;
				for (int i = checkDownRight.Row; i < 9; i++)
				{
					int row = checkDownRight.Row + shift;
					int col = checkDownRight.Col + shift;
					if (IsInBoard(row, col))
					{
						ut::Position p(row, col);
						if (IsInBoard(row, col))
						{
							moves.push_back(p);
							if (tiles[row][col].IsPieceInside() && tiles[row][col].GetPieceId() == 63
								&& IsEnemy(king->IsFacingUp(), row, col, tiles))
							{
								bishopPos = p;
								for (auto& m : moves)
								{
									resMoves.push_back(m);
								}
								break;
							}
						}
					}
					shift++;
				}
				movesMutex.unlock();
			}));
	}
	if (IsInBoard(checkDownLeft))
	{
		threads.push_back(std::thread([this, &movesMutex, &checkDownLeft, &tiles, &king, &bishopPos, &resMoves]()
			{
				int shift = 0;
				std::vector<ut::Position> moves;
				for (int i = checkDownLeft.Row; i < 9; i++)
				{
					int row = checkDownLeft.Row + shift;
					int col = checkDownLeft.Col - shift;
					if (IsInBoard(row, col))
					{
						ut::Position p(row, col);
						if (IsInBoard(row, col))
						{
							moves.push_back(p);
							if (tiles[row][col].IsPieceInside() && tiles[row][col].GetPieceId() == 63
								&& IsEnemy(king->IsFacingUp(), row, col, tiles))
							{
								bishopPos = p;
								for (auto& m : moves)
								{
									resMoves.push_back(m);
								}
								break;
							}
						}
					}
					shift++;
				}
			}));
	}
	if (IsInBoard(checkUpLeft))
	{
		threads.push_back(std::thread([this, &movesMutex, &checkUpLeft, &tiles, &king, &bishopPos, &resMoves]()
			{
				int shift = 0;
				std::vector<ut::Position> moves;
				for (int i = checkUpLeft.Row; i > 0; i--)
				{
					int row = checkUpLeft.Row - shift;
					int col = checkUpLeft.Col - shift;
					if (IsInBoard(row, col))
					{
						ut::Position p(row, col);
						if (IsInBoard(row, col))
						{
							moves.push_back(p);
							if (tiles[row][col].IsPieceInside() && tiles[row][col].GetPieceId() == 63
								&& IsEnemy(king->IsFacingUp(), row, col, tiles))
							{
								bishopPos = p;
								for (auto& m : moves)
								{
									resMoves.push_back(m);
								}
								break;
							}
						}
					}
					shift++;
				}
			}));
	}
	for (auto& t : threads)
	{
		t.join();
	}
	return resMoves;
}

ut::Position ut::BoardUtilities::SpecialPieceAttackCheck(ut::Position srcPos, ut::Position checkedPos, int& shift, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	ut::Position pos;
	if (IsInBoard(checkedPos))
	{
		if (!tiles[checkedPos.Row][checkedPos.Col].IsPieceInside())
		{
			pos = checkedPos;
			shift++;
		}
		else
		{
			if (IsEnemy(tiles[srcPos.Row][srcPos.Col].GetIsPieceFacingUp(), checkedPos.Row, checkedPos.Col, tiles))
			{
				pos = checkedPos;
			}
			shift = -999;
		}
	}
	return pos;
}

void ut::BoardUtilities::DisplayPossibleMoves(std::vector<ut::Position>& moves,
	bool isSrcPieceFacingUp,
	std::vector<std::vector<ut::BoardTile>>& tiles, std::map<std::string,
	sf::Texture>& textureMap)
{
	for (int i = 0; i < moves.size(); i++)
	{
		auto p = moves[i].GetPosition();
		if (tiles[p.Row][p.Col].IsPieceInside())
		{
			if (IsEnemy(isSrcPieceFacingUp, p.Row, p.Col, tiles))
			{
				tiles[p.Row][p.Col].AssignTexture(textureMap["Attacked" + tiles[p.Row][p.Col].GetPieceName()]);
				tiles[p.Row][p.Col].SetPossibleMove(true);
			}
		}
		else
		{
			tiles[p.Row][p.Col].AssignTexture(textureMap["PossibleMove"]);
			tiles[p.Row][p.Col].SetPossibleMove(true);
		}
	}
}