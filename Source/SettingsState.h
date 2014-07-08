#pragma once

#include "State.h"
#include "Player.h"
#include "GUI/Container.h"
#include "GUI/Button.h"
#include "GUI/Label.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <array>

/*	The setting state is very powerful, because the user
	can change various settings like the resolution,
	fullscreen or window mode, vSync, volume...
	All rendered on a wonderful book, it has an awesome
	flare. The state saves and loads everything so the user
	doesn't have to set this all over again.
*/
class SettingsState : public State
{
	public:
		SettingsState(StateStack& stack, Context context);

		virtual bool update(sf::Time elapsedTime);
		virtual void render();
		virtual bool handleEvent(const sf::Event& windowEvent);

	private:
		void applyChanges();
		void updateLabels();
		void saveResolution();
		void setCurrentMode();
		void addComponents(Context context);
		void addButtonLabel(Player::Action action, float yPos, const std::string& text, Context context);

	private:
		sf::Uint32 currentStyle;
		unsigned int currentMode;
		sf::Shader& shader;
		sf::Sprite textLogo;
		sf::Sprite bookBackground;
		sf::Text resolutionText;
		sf::RectangleShape shaderShape;
		GUI::Container container;
		std::vector<sf::VideoMode> modeList;
		std::array<GUI::Button::Ptr, Player::ACTION_COUNT> bindingButtons;
		std::array<GUI::Label::Ptr, Player::ACTION_COUNT> bindingLabels;
};