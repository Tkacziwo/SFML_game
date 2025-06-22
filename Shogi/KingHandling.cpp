#include "KingHandling.h"

void ut::KingHandling::SetKing(game::SimplePiece king)
{
	King = king;
}

void ut::KingHandling::SetKingPosition(const ut::Position& pos)
{
	KingPosition = pos;
}

ut::Position ut::KingHandling::GetKingPosition() const noexcept
{
	return KingPosition;
}

void ut::KingHandling::SetAttackerPosition(const ut::Position& p)
{
	AttackerPos = p;
}

ut::Position ut::KingHandling::GetAttackerPosition() const noexcept
{
	return AttackerPos;
}

void ut::KingHandling::CheckPossibleMoves(std::vector<std::vector<ut::BoardTile>>& tiles)
{
	King.SetIsBottom(tiles[KingPosition.Row][KingPosition.Col].GetIsPieceFacingUp());
	game::IPiece* temp = tiles[KingPosition.Row][KingPosition.Col].GetPiece();
	std::vector<std::vector<int>> moveset = temp->GetPieceMoveset();
	KingPossibleMoves.clear();
	for (int col = 0; col < 3; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (moveset[row][col] == 1)
			{
				int destRow = KingPosition.Row - 1 + row;
				int destCol = KingPosition.Col - 1 + col;

				ut::Position destPosition(destRow, destCol);

				if (Utility->IsInBoard(destRow, destCol))
				{
					if (!tiles[destRow][destCol].IsPieceInside())
					{
						KingPossibleMoves.push_back(destPosition);
					}
					else
					{
						if (Utility->IsEnemy(King.IsFacingUp(), destRow, destCol, tiles))
						{
							KingPossibleMoves.push_back(destPosition);
						}
					}
				}
			}
		}
	}
	CheckSafeMoves(tiles);
}

void ut::KingHandling::CheckSafeMoves(std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<ut::Position> resMoves;
	std::vector<ut::Position> kingMovesCopy;
	std::vector<ut::Position> maxExtension;
	for (int col = 0; col < 9; col++)
	{
		for (int row = 0; row < 9; row++)
		{
			if (tiles[row][col].IsPieceInside())
			{
				if (Utility->IsEnemy(King.IsFacingUp(), row, col, tiles))
				{
					game::IPiece* piece = tiles[row][col].GetPiece();
					std::vector<std::vector<int>> moveset = piece->GetPieceMoveset();
					std::vector<ut::Position> otherPosition;
					if (piece->GetPieceId() > 62)
					{
						std::vector<ut::Position> merged;
						otherPosition = Utility->CalculateSpecialPossibleMoves(piece, ut::Position(row, col), tiles);
						maxExtension = Utility->MaximumExtension(piece->GetPieceId(), ut::Position(row, col), tiles);
						for (auto& m : maxExtension)
						{
							otherPosition.push_back(m);
						}
					}
					else
					{
						otherPosition = Utility->CalculatePossibleMoves(piece, ut::Position(row, col), tiles);
					}
					for (auto& k : KingPossibleMoves)
					{
						if (k.Equals(AttackerPos) && IsAttackingPieceProtected(AttackerPos, tiles))
						{
							k.Row = -999;
						}
					}
					for (int i = 0; i < KingPossibleMoves.size(); i++)
					{
						for (int k = 0; k < otherPosition.size(); k++)
						{
							if (KingPossibleMoves[i].Row == otherPosition[k].Row
								&& KingPossibleMoves[i].Col == otherPosition[k].Col)
							{
								if (KingPossibleMoves[i].Equals(AttackerPos) && !IsAttackingPieceProtected(AttackerPos, tiles))
								{

								}
								else
								{
									KingPossibleMoves[i].Row = -999;
								}
							}
						}
					}

				}
			}
		}
	}
	for (int i = 0; i < KingPossibleMoves.size(); i++)
	{
		if (KingPossibleMoves[i].Row != -999)
		{
			resMoves.push_back(KingPossibleMoves[i]);
		}
	}
	KingPossibleMoves.clear();
	for (auto& m : resMoves)
	{
		KingPossibleMoves.push_back(m);
	}
}

std::vector<ut::Position> ut::KingHandling::GetKingMoves() const noexcept
{
	return KingPossibleMoves;
}

ut::Position ut::KingHandling::FindKing(bool playerB, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (tiles[row][col].IsPieceInside() && tiles[row][col].GetPieceId() == 62)
			{
				game::IPiece* piece = tiles[row][col].GetPiece();
				if (playerB && piece->IsFacingUp())
				{
					return ut::Position(row, col);
				}
				else if (!playerB && !piece->IsFacingUp())
				{
					return ut::Position(row, col);
				}
			}
		}
	}
}

std::vector<ut::Position> ut::KingHandling::FindBodyguards(std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<ut::Position> guardPositions;
	for (int col = 0; col < 9; col++)
	{
		for (int row = 0; row < 9; row++)
		{
			if (tiles[row][col].IsPieceInside())
			{
				game::IPiece* piece = tiles[row][col].GetPiece();
				if (!Utility->IsEnemy(tiles[KingPosition.Row][KingPosition.Col].GetIsPieceFacingUp(), row, col, tiles) && piece->GetPieceId() != 62)
				{
					if (!ut::Position(row, col).Equals(KingPosition)
						&& CanPieceKillAttacker(tiles, ut::Position(row, col), AttackerPos))
					{
						guardPositions.push_back(ut::Position(row, col));
					}
				}
			}
		}
	}
	return guardPositions;
}

std::vector<ut::Position> ut::KingHandling::FindSacrifices(std::vector<std::vector<ut::BoardTile>>& tiles, std::vector<ut::Position>& endangeredMoves)
{
	std::vector<ut::Position> sacrifices;
	ut::Position rookPos;
	ut::Position bishopPos;
	game::IPiece* attacker = tiles[AttackerPos.Row][AttackerPos.Col].GetPiece();
	game::SimplePiece king = tiles[KingPosition.Row][KingPosition.Col].GetPiece();
	std::vector<ut::Position> rookMoves, bishopMoves;
	if (attacker->GetPieceId() == 64)
	{
		rookMoves = Utility->MaximumAbsoluteExtensionRook(KingPosition, tiles, rookPos);
	}
	else if (attacker->GetPieceId() == 63)
	{
		bishopMoves = Utility->MaximumAbsoluteExtensionBishop(KingPosition, tiles, bishopPos);
	}
	for (auto& r : rookMoves)
	{
		endangeredMoves.push_back(r);
	}
	for (auto& b : bishopMoves)
	{
		endangeredMoves.push_back(b);
	}
	for (int col = 0; col < 9; col++)
	{
		for (int row = 0; row < 9; row++)
		{
			if (tiles[row][col].IsPieceInside() && !Utility->IsEnemy(king.IsFacingUp(), row, col, tiles)
				&& !KingPosition.Equals(ut::Position(row, col)))
			{
				game::IPiece* chosenPiece = tiles[row][col].GetPiece();
				ut::Position chosenPiecePosition(row, col);
				std::vector<ut::Position> chosenPiecePossibleMoves;
				if (chosenPiece->GetPieceId() > 62)
				{
					chosenPiecePossibleMoves = Utility->CalculateSpecialPossibleMoves(chosenPiece, chosenPiecePosition, tiles);
				}
				else
				{
					chosenPiecePossibleMoves = Utility->CalculatePossibleMoves(chosenPiece, chosenPiecePosition, tiles);
				}
				for (int i = 0; i < chosenPiecePossibleMoves.size(); i++)
				{
					for (int j = 0; j < endangeredMoves.size(); j++)
					{
						if (chosenPiecePossibleMoves[i].Equals(endangeredMoves[j]))
						{
							sacrifices.push_back(chosenPiecePosition);
							break;
						}
					}
				}
			}
		}
	}
	return sacrifices;
}

std::vector<int> ut::KingHandling::CheckDropSacrifices(std::map<int, ut::BoardTile> pieces, std::vector<ut::Position>& endangeredMoves, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	std::vector<int> goodDrops;
	for (auto& c : pieces)
	{
		std::vector<ut::Position> moves = Utility->CalculateDropPossibleMoves(c.second.GetPiece(), tiles);
		for (int i = 0; i < endangeredMoves.size(); i++)
		{
			for (int j = 0; j < moves.size(); j++)
			{
				if (moves[j].Equals(endangeredMoves[i]))
				{
					goodDrops.push_back(c.first);
					break;
				}
			}
		}
	}
	return goodDrops;
}

bool ut::KingHandling::CanPieceKillAttacker(std::vector<std::vector<ut::BoardTile>>& tiles, ut::Position guardPos, ut::Position attackersPos)
{
	game::IPiece* guard = tiles[guardPos.Row][guardPos.Col].GetPiece();
	auto guardMoveset = guard->GetPieceMoveset();
	std::vector<ut::Position> guardMoves;
	if (guard->GetPieceId() > 62)
	{
		guardMoves = Utility->CalculateSpecialPossibleMoves(guard, guardPos, tiles);
	}
	else
	{
		guardMoves = Utility->CalculatePossibleMoves(guard, guardPos, tiles);
	}

	for (auto& m : guardMoves)
	{
		if (attackersPos.Row == m.Row && attackersPos.Col == m.Col)
		{
			return true;
		}
	}
	return false;
}

bool ut::KingHandling::IsAttackingPieceProtected(ut::Position attackerPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	game::IPiece* attacker = tiles[attackerPos.Row][attackerPos.Col].GetPiece();
	bool protectedBySimplePiece = IsAttackingPieceProtectedInVicinity(attackerPos, tiles);
	bool protectedByRook = Utility->RookProtectionMoves(attacker->IsFacingUp(), attackerPos, tiles);
	bool protectedByBishop = Utility->BishopProtectionMoves(attacker->IsFacingUp(), attackerPos, tiles);
	bool protectedByLance = Utility->LanceProtectionMoves(attacker->IsFacingUp(), attackerPos, tiles);
	bool protectedByHorse = Utility->HorseProtectionMoves(attacker->IsFacingUp(), attackerPos, tiles);
	if (protectedBySimplePiece || protectedByRook || protectedByBishop || protectedByLance || protectedByHorse)
	{
		return true;
	}
	return false;
}

bool ut::KingHandling::IsAttackingPieceProtectedInVicinity(ut::Position attackerPos, std::vector<std::vector<ut::BoardTile>>& tiles)
{
	game::IPiece* attacker = tiles[attackerPos.Row][attackerPos.Col].GetPiece();
	for (int col = 0; col < 3; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			ut::Position p(attackerPos.Row - 1 + row, attackerPos.Col - 1 + col);
			if (Utility->IsInBoard(p) && !p.Equals(attackerPos))
			{
				bool srcFacesUp = attacker->IsFacingUp();
				if (tiles[p.Row][p.Col].IsPieceInside() && !Utility->IsEnemy(srcFacesUp, p.Row, p.Col, tiles))
				{
					game::IPiece* checkedPiece = tiles[p.Row][p.Col].GetPiece();
					auto checkedPieceMoveset = checkedPiece->GetPieceMoveset();
					std::vector<ut::Position> checkedPieceMoves
						= Utility->CalculateProtectionMoves(checkedPiece, ut::Position(p.Row, p.Col), tiles);
					for (auto& m : checkedPieceMoves)
					{
						if (m.Row == attackerPos.Row && m.Col == attackerPos.Col)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}