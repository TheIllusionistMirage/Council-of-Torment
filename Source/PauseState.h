#pragma once

#include "State.h"
#include "GUI/Container.h"
#include <SFML/Graphics/Text.hpp>

/*	The pause state always runs, when the user
	presses "P" or "Escape" ingame. You can either
	return to the game or return to the menu.
*/
class PauseState : public State
{
	public:
		PauseState(StateStack& stack, Context context);

		virtual bool update(sf::Time elapsedTime);
		virtual void render();
		virtual bool handleEvent(const sf::Event& windowEvent);

	private:
		sf::Text pauseText;
		GUI::Container container;
};