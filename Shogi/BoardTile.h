import Hitbox;

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "SimplePiece.h"
#include "SpecialPiece.h"

namespace ut
{
	class BoardTile
	{
	public:

		BoardTile() 
		{
			ContainsPiece = false;
			ContainsPossibleMove = false;
			Piece = nullptr;
		}

		BoardTile(sf::Vector2f& offset)
			: ContainsPiece(false)
		{
			ContainsPossibleMove = false;
			Piece = nullptr;
			Tile.setSize(sf::Vector2f(50, 50));
			Tile.setFillColor(sf::Color::White);
			Tile.setPosition(offset);
			Hitbox = ut::Hitbox(Tile);
			sf::Texture t;
			t.loadFromFile(".\\Sprite\\Piece\\BlankTile.png");
			BackupTexture = t;
		}

		void DrawTile(sf::RenderTarget& target) const
		{
			target.draw(Tile);
		}

		void SetBackground(const sf::Color& color)
		{
			Tile.setFillColor(color);
		}

		bool IsPieceInside() const noexcept
		{
			if (ContainsPiece == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool ContainsPieceInBounds(sf::Vector2i mousePos)
		{
			if (Hitbox.CheckHitbox(mousePos) && ContainsPiece)
				return true;
			else
				return false;
		}

		bool ContainsPossibleMoveInBounds(sf::Vector2i mousePos)
		{
			if (Hitbox.CheckHitbox(mousePos) && ContainsPossibleMove == true)
				return true;
			else
				return false;
		}

		game::IPiece* GetPiece()
		{
			if (Piece->GetPieceId() != 0)
				return Piece;
			else
				return nullptr;
		}

		bool GetIsPieceFacingUp()
		{
			if (Piece->GetPieceId() != 0)
				return Piece->IsFacingUp();
			else
				return false;
		}

		const sf::Texture* GetTexture()
		{
			const sf::Texture* t(Tile.getTexture());

			return t;
		}

		void AssignPiece(game::IPiece* piece, sf::Texture& texture)
		{
			Piece = piece;
			ContainsPiece = true;
			ContainsPossibleMove = false;
			AssignTexture(texture);
		}

		void AssignSimplePiece(game::SimplePiece& piece, sf::Texture& texture)
		{
			Piece = new game::SimplePiece(piece);
			ContainsPiece = true;
			ContainsPossibleMove = false;
			AssignTexture(texture);
		}

		void AssignSpecialPiece(game::SpecialPiece& piece, sf::Texture& texture)
		{
			Piece = new game::SpecialPiece(piece);
			ContainsPiece = true;
			ContainsPossibleMove = false;
			AssignTexture(texture);
		}

		void AssignTexture(sf::Texture& t)
		{
			Tile.setTexture(&t);
		}

		void ReleasePiece()
		{
			ContainsPiece = false;
			ContainsPossibleMove = false;
			Tile.setTexture(&BackupTexture);
		}

		void SetTilePosition(const sf::Vector2f& offset)
		{
			Tile.setPosition(offset);
			Hitbox = ut::Hitbox(Tile);
		}

		sf::Rect<float> GetTilePosition()
		{
			return Tile.getGlobalBounds();
		}

		std::string GetPieceName()
		{
			return Piece->GetPieceName();
		}

		int GetPieceId()
		{
			return Piece->GetPieceId();
		}

		void ClearTexture()
		{
			Tile.setTexture(&BackupTexture);
			ContainsPossibleMove = false;
		}

		bool IsMouseInBounds(sf::Vector2i mousePos)
		{
			return Hitbox.CheckHitbox(mousePos);
		}

		void SetPossibleMove(bool contains)
		{
			ContainsPossibleMove = contains;
		}

		bool IsPossibleMove() const noexcept
		{
			return ContainsPossibleMove;
		}

		void DeallocateMemory()
		{
			delete Piece;
		}

	private:
		sf::RectangleShape Tile;
		ut::Hitbox Hitbox;
		sf::Texture PieceTexture;
		game::IPiece* Piece;
		bool ContainsPiece;
		bool ContainsPossibleMove;
		sf::Texture BackupTexture;
	};
}