export module Position;
import <map>;

namespace ut
{
	export class Position
	{
	public:
		Position() {};

		Position(int row, int col) : Row(row), Col(col) {};

		void UpdatePosition(int row, int col)
		{
			Row = row;
			Col = col;
		}

		void UpdatePosition(Position p)
		{
			Row = p.Row;
			Col = p.Col;
		}

		std::pair<int, int> GetPosition() const noexcept
		{
			return std::make_pair(Row, Col);
		}

		ut::Position GetPosition()
		{
			return ut::Position(Row, Col);
		}

		bool Equals(const ut::Position& other) const
		{
			if (Row == other.Row && Col == other.Col)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		int Row;
		int Col;
	};
}