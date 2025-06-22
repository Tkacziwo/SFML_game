export module Hitbox;
import <SFML/Graphics.hpp>;

namespace ut
{
	export class Hitbox
	{
	public:
		Hitbox() {};

		Hitbox(const sf::RectangleShape shape)
		{
			Bounds = sf::FloatRect(shape.getGlobalBounds());
		}

		bool CheckHitbox(const sf::Vector2i mousePos)
		{
			sf::Vector2f convMousePos = static_cast<sf::Vector2f>(mousePos);

			if (Bounds.contains(convMousePos))
				return true;
			else
				return false;
		}

		sf::FloatRect GetHitboxBounds() const noexcept
		{
			return Bounds;
		}

		void operator = (sf::RectangleShape shape)
		{
			Bounds = sf::FloatRect(shape.getGlobalBounds());
		}

	private:
		sf::FloatRect Bounds;
	};

	static sf::FloatRect operator << (Hitbox& hitbox, sf::FloatRect& res)
	{
		return hitbox.GetHitboxBounds();
	}
}