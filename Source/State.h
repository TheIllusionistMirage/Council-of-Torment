#pragma once

#include "Stdafx.h"
#include "Contentmanager.h"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class EffectManager;
class LightManager;
class StateStack;
class GlobalTime;
class Crafting;
class GameMap;
class Console;
class Player;

/*	The state class is the basis of all other states.
	It also has the context, which holds the most
	important classes needed in nearly every function.
*/
class State
{
	public:
		typedef std::unique_ptr<State> Ptr;
		struct Context
		{
			Context(sf::RenderTexture& window, sf::RenderWindow& renderWindow, ContentManager& contentManager, EffectManager& effectManager, LightManager& lightManager,
					Player& player, GameMap& gameMap, Console& console, Crafting& crafting, GlobalTime& globalTime);
			sf::RenderTexture* window;
			sf::RenderWindow* renderWindow;
			LightManager* lightManager;
			EffectManager* effectManager;
			ContentManager* contentManager;
			GlobalTime* globalTime;
			Crafting* crafting;
			GameMap* gameMap;
			Console* console;
			Player* player;
		};

	public:
		State(StateStack& stack, Context context);
		virtual ~State();

		virtual bool update(sf::Time elapsedTime) = 0;
		virtual void render() = 0;
		virtual bool handleEvent(const sf::Event& windowEvent) = 0;

	protected:
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStateClear();

		Context getContext() const;

	private:
		StateStack* stack;
		Context context;
};