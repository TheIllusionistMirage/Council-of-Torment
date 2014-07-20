#include <SFML/Graphics/RenderWindow.hpp>

#include "Player.h"
#include "DialogueBox.h"
#include "LuaScript.h"
#include "GameMap.h"
#include "Console.h"
#include "Stdafx.h"

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Constructor for a Dialogue Box
* ----------------------------------------------------------------------
*/
	DialogueBox::DialogueBox(State::Context context, const std::string t)
	: context(context)
	, fontSize(11)
	, sidePadding(3)
	, topPadding(2)
	, rectColor(0,0,0,150)
	, textColor(sf::Color::White)
	{
		font.loadFromFile("Content/Fonts/arial.ttf");

		text.setPosition(sf::Vector2f(0, 0));
		text.setCharacterSize(fontSize);
		text.setColor(textColor);
		text.setFont(context.contentManager->getFont(Fonts::SEGOEUI));
		text.setString(t);
	

		int width = text.getGlobalBounds().width + sidePadding * 2;
		int height = text.getGlobalBounds().height + topPadding * 2;

		rect.setFillColor(rectColor);
		rect.setSize(sf::Vector2f(width, height));

		centerOrigin(text);
		centerOrigin(rect);
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Sets the position of the whole element
* ----------------------------------------------------------------------
*/
	void DialogueBox::setPosition(sf::Vector2f p)
	{
		pos = p;

		rect.setPosition(pos);
		text.setPosition(pos.x, pos.y-1);
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Sets the text
* ----------------------------------------------------------------------
*/
	void DialogueBox::setText(std::string t)
	{
		text.setString(t);

		int width = text.getGlobalBounds().width + sidePadding*2;
		int height = text.getGlobalBounds().height + topPadding*2;

		rect.setSize(sf::Vector2f(width, height));

		centerOrigin(text);
		centerOrigin(rect);
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Sets the text
* ----------------------------------------------------------------------
*/
	void DialogueBox::setText(std::string t, int size)
	{
		text.setString(t);

		int width = size + sidePadding * 2;
		int height = fontSize + topPadding * 2;

		rect.setSize(sf::Vector2f(width, height));

		centerOrigin(text);
		centerOrigin(rect);
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Renders the dialogueBox
* ----------------------------------------------------------------------
*/
	void DialogueBox::render()
	{
		context.window -> draw(rect);
		context.window -> draw(text);
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Handles the input
* ----------------------------------------------------------------------
*/
	void DialogueBox::handleEvents(const sf::Event& e)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*context.window);
		sf::FloatRect bounds = rect.getGlobalBounds();

		if (e.type == sf::Event::MouseMoved)
		{
			if (bounds.contains(static_cast<sf::Vector2f>(mousePos)))
			{
				rect.setFillColor(sf::Color::White);
				text.setColor(sf::Color::Black);
			}

			else
			{
				rect.setFillColor(rectColor);
				text.setColor(textColor);
			}
		}

		if (e.type == sf::Event::MouseButtonPressed &&
			sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
			bounds.contains(static_cast<sf::Vector2f>(mousePos)))
			{
					if (execute == "none")
					{
						context.player->setProperty("isInDialogue", false);
						context.gameMap->getNPCs()[context.player->getTargetNPC()]->setDialogueMode(false);
					}
					else
						context.gameMap -> getNPCs()[context.player->getTargetNPC()]->sayLine(execute);
			}
	}