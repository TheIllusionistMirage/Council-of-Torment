#pragma once

#include "GUI/Container.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

/*	The menu state only runs in the menu.
	It handles everything of the menu and
	uses a powerful GUI container, which makes
	the code so small.
*/
class MenuState : public State
{
	public:
		MenuState(StateStack& stack, Context context);

		virtual bool update(sf::Time elapsedTime);
		virtual void render();
		virtual bool handleEvent(const sf::Event& windowEvent);

	private:
		void addComponents(Context context);

	private:
		sf::Shader& shader;
		sf::Sprite textLogo;
		sf::RectangleShape shaderShape;
		GUI::Container container;
};