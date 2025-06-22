#include "BoardClickHandler.h"

ut::BoardClickHandler::BoardClickHandler()
{
	Utility = new BoardUtilities();
}

std::pair<int, std::string> ut::BoardClickHandler::HandleButtonClick(std::vector<core::Button>& btns, 
	const sf::Vector2i& mousePosition)
{
	if (GameEnd)
	{
		return GameEndHandle(btns, mousePosition);
	}
	else
	{

	}
	return std::pair<int, std::string>();
}

std::pair<int, std::string> ut::BoardClickHandler::GameEndHandle(std::vector<core::Button>& btns, 
	const sf::Vector2i& mousePosition)
{
	std::pair<int, std::string> action;
	for (auto& b : btns)
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
	if (action.second == "")
	{
		action.second = "NotBound";
		action.first = 100;
	}
	return action;
}

ut::BoardClickHandler::~BoardClickHandler()
{
	delete Utility;
}