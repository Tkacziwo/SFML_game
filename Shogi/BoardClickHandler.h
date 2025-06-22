#pragma once
#include "Imports.h"
#include "Button.h"

namespace ut
{
	class BoardClickHandler
	{
	private:
		bool GameEnd;
		ut::BoardUtilities* Utility;
	public:
		BoardClickHandler();

		std::pair<int, std::string> HandleButtonClick(std::vector<core::Button>& btns, const sf::Vector2i& mousePosition);

		std::pair<int, std::string> GameEndHandle(std::vector<core::Button>& btns,
			const sf::Vector2i& mousePosition);

		~BoardClickHandler();
	};
}