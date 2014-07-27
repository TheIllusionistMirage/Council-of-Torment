#pragma once

#include "Player.h"
#include "GameMap.h"
#include "Console.h"
#include "Crafting.h"
#include "GlobalTime.h"
#include "StateStack.h"
#include "LightManager.h"
#include "EffectManager.h"
#include "GUI/Container.h"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

/*	The game class is the first thing the main
	function creates. It creates all the classes,
	the window, the states...
	It is like the main point of the project.
*/
class Game
{
	public:
		Game();
		void run();
		static sf::Uint32 windowStyle;
		static States::ID currentState;
		
	private:
		void handleEvent();
		void update(sf::Time elapsedTime);
		void render();
		void renderMenuMap();
		void loadResolution();
		void registerStates();

	private:
		static const sf::Time FrameTime;
		float cameraSpeed;
		float startPos;
		float endPos;
		bool loadMap;
		bool right;

		sf::RenderWindow window;
		sf::RenderTexture windowTexture;
		State::Context context;
		StateStack stateStack;

		sf::View menuCamera;
		EffectManager effectManager;
		ContentManager contentManager;
		GlobalTime globalTime;
		Crafting crafting;
		Console console;
		GameMap gameMap;
		GameMap menuMap;
		Player player;
		sf::Text fpsText;

		LightManager lightManager;
};