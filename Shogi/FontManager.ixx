export module FontManager;
import <SFML/Graphics.hpp>;
import <SFML/System.hpp>;
import <filesystem>;

namespace ut
{
	export class FontManager
	{
	public:
		FontManager() {};

		void LoadFontFromFile()
		{
			std::filesystem::path path(".\\Fonts\\arial.ttf");

			Font.loadFromFile(path.generic_string());
		}

		sf::Font GetFont() const noexcept
		{
			return Font;
		}

	private:

		sf::Font Font;
	};
}