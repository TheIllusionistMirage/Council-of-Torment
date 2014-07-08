#pragma once

#include "State.h"
#include <SFML/Graphics/Text.hpp>

/*	The defocus state is the state which get's
	called, whenever the game is out of focus.
	It simply displays a text and freezes the
	game update with a darken texture.
*/
class DefocusState : public State
{
	public:
		DefocusState(StateStack& stack, Context context);

		virtual bool update(sf::Time elapsedTime);
		virtual void render();
		virtual bool handleEvent(const sf::Event& windowEvent);

	private:
		sf::Text defocusText;
};