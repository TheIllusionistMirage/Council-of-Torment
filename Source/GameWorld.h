#pragma once

#include "State.h"
#include "GameMap.h"
#include <SFML/Graphics/RenderTexture.hpp>

/*	The game world is the class which has the game map
	and all the LivingEntitys in it. It draws them in a
	specific order, to get our cool 2.5d look.
*/
class GameWorld
{
	public:
		GameWorld(State::Context context);

		void update(sf::Time elapsedTime);
		void render();
		void handleEvent(const sf::Event& windowEvent);

	private:
		State::Context context;
		sf::RenderTexture roofTexture;
};