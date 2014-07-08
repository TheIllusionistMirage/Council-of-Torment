#pragma once

#include "State.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

/*	The title state is the state which is shown
	on the startup of the game. It will show some
	sort of information or maybe a loading screen.
	Not so sure atm^^
*/
class TitleState : public State
{
	public:
		TitleState(StateStack& stack, Context context);

		virtual bool update(sf::Time elapsedTime);
		virtual void render();
		virtual bool handleEvent(const sf::Event& windowEvent);

	private:
		sf::Sprite backgroundSprite;
		sf::Time textTime;
		sf::Text text;
		bool showText;
};