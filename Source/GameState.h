#pragma once

#include "State.h"
#include "GameWorld.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

/*	The game state is the state which runs, when
	starting the actual game. It has the game world
	and handles some important events like the
	player movement.
*/
class GameState : public State
{
	public:
		GameState(StateStack& stack, Context context);

		virtual bool update(sf::Time elapsedTime);
		virtual void render();
		virtual bool handleEvent(const sf::Event& windowEvent);

	private:
		GameWorld gameWorld;
		bool playerMovement[4];
};