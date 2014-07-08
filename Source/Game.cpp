#include "Game.h"
#include "TitleState.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "DefocusState.h"
#include "SettingsState.h"
#include <Windows.h>
#include <fstream>

// Static / const variables
const sf::Time Game::FrameTime = sf::seconds(1.0f/60.0f);
sf::Uint32 Game::windowStyle = sf::Style::Close;
States::ID Game::currentState = States::TITLE;

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the game class
 * ----------------------------------------------------------------------
 */
Game::Game()
: window()
, globalTime()
, contentManager()
, context(State::Context(window, contentManager, effectManager, lightManager, player, gameMap, console, globalTime))
, player(context, "player")
, effectManager(context)
, lightManager(context)
, stateStack(context)
, console(context)
, gameMap(context)
, menuMap(context)
, cameraSpeed(10.0f)
, startPos(1360.0f)
, endPos(1600.0f)
, loadMap(true)
, right(true)
{
	// Generate random numbers
	srand(unsigned int(time(0)));

	// Load the resolution
	loadResolution();

	// Set the framelimit and enable vSync
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(false);

	// Create the effect manager texture
	context.effectManager->createTexture();

	// Init the light manager
	lightManager.init();

	// Register all states and start with the title state
	registerStates();
	stateStack.pushState(States::MENU);

	// Clear the window and display it
	window.clear(sf::Color(50, 50, 50));
	window.display();

	// Create the menu camera
	menuCamera = sf::View(sf::Vector2f(startPos, 1460), sf::Vector2f(window.getSize()));
	menuCamera.zoom(0.5f);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Runs the game loop
 * ----------------------------------------------------------------------
 */
void Game::run()
{
	// Some variables to measure the time
	sf::Clock clock;
	sf::Time lastUpdateTime = sf::Time::Zero;

	// Loads an icon and sets it the window
	sf::Image icon;
	if(!icon.loadFromFile("Content/Textures/icon.png"))
		console.logError("Failed to load icon!");
	window.setIcon(50, 50, icon.getPixelsPtr());

	while(window.isOpen())
	{
		// Get the elapsed time and add it to the variable
		lastUpdateTime += clock.restart();

		// If the game receives any lag this saves the game logic
		while(lastUpdateTime > FrameTime)
		{
			// Reduce the time of the lag und update everything until the lag is over
			lastUpdateTime -= FrameTime;

			// Handle the events and update the game
			handleEvent();
			update(FrameTime);
			
			MSG msg;
			while(PeekMessage(&msg, NULL, 0xc042, 0xc042, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If no state is the stack, the game should close
			if(stateStack.isEmpty())
				window.close();
		}

		// Render the game
		render();
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the game
 * ----------------------------------------------------------------------
 */
void Game::update(sf::Time elapsedTime)
{
	// Update the stack
	stateStack.update(elapsedTime);

	// Update the content manager
	contentManager.update(elapsedTime);

	// Update the effect manager (2, because then it doesn't update if you are out if focus)
	if(stateStack.getSize() < 2)
		effectManager.update(elapsedTime);

	// Update the console
	console.update(elapsedTime);

	// Load the map once
	if((currentState == States::MENU || currentState == States::SETTINGS))
	{
		if(loadMap)
		{
			menuMap.loadMap("gideon_tower.tmx");
			loadMap = false;
		}
	}
	else
		loadMap = true;

	if((currentState == States::MENU || currentState == States::SETTINGS) && stateStack.getSize() < 2)
	{
		// Update the size
		menuCamera.setSize(sf::Vector2f(window.getSize()));
		menuCamera.zoom(0.5f);

		// Move the camera diagonally back and forth
		if(right)
		{
			// Move the camera until a certain position
			menuCamera.move(cameraSpeed * elapsedTime.asSeconds(), cameraSpeed * elapsedTime.asSeconds());
			if(menuCamera.getCenter().x > endPos)
			{
				// Move back and turn around
				menuCamera.move(-cameraSpeed * elapsedTime.asSeconds(), -cameraSpeed * elapsedTime.asSeconds());
				right = false;
			}
		}
		else
		{
			// Move the camera until a certain position
			menuCamera.move(-cameraSpeed * elapsedTime.asSeconds(), -cameraSpeed * elapsedTime.asSeconds());
			if(menuCamera.getCenter().x < startPos)
			{
				// Move back and turn around
				menuCamera.move(cameraSpeed * elapsedTime.asSeconds(), cameraSpeed * elapsedTime.asSeconds());
				right = true;
			}
		}

		// Update the light manager
		lightManager.update(&menuMap, elapsedTime);
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the game
 * ----------------------------------------------------------------------
 */
void Game::render()
{
	// Clear the window
	window.clear(sf::Color::White);

	// Render the menu map if the state is correct
	if(currentState == States::MENU || currentState == States::SETTINGS)
		renderMenuMap();

	// Render the stack
	stateStack.render();

	// Render the effect manager
	effectManager.render();

	// Render the console
	window.draw(console);

	// Display the window
	window.display();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the menu map
 * ----------------------------------------------------------------------
 */
void Game::renderMenuMap()
{
	// Set the menu camera
	window.setView(menuCamera);

	// Render the layers of the map
	menuMap.render("background");
	menuMap.render("background2");
	menuMap.render("object");
	menuMap.render("object2");
	menuMap.render("shadow");
	menuMap.render("roof");
	menuMap.render("roof2");
	menuMap.render("roof-shadow");

	// Render the light manager
	lightManager.render(menuCamera);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events
 * ----------------------------------------------------------------------
 */
void Game::handleEvent()
{
	// Handle all the incoming events
	sf::Event windowEvent;
	while(window.pollEvent(windowEvent))
	{
		// If the user closes with X the game should close
		if(windowEvent.type == sf::Event::Closed)
			window.close();
		else if(windowEvent.type == sf::Event::LostFocus)
		{
			// Add the defocus state
			stateStack.pushState(States::DEFOCUS);
		}
		else if(windowEvent.type == sf::Event::GainedFocus)
		{
			// Remove the defocus state
			stateStack.popState();
		}

		// Pass the other events to the stack
		if(effectManager.getEffectList().empty() && !console.isVisible())
			stateStack.handleEvent(windowEvent);

		// Pass the other events to the console
		console.handleEvent(windowEvent);

		if(windowEvent.type == sf::Event::MouseWheelMoved)
		{
			// Reset the mouse wheel
			if(windowEvent.mouseWheel.delta != 0)
				windowEvent.mouseWheel.delta = 0;
		}
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Loads the resolution
 * ----------------------------------------------------------------------
 */
void Game::loadResolution()
{
	// Open the resolution file
	std::ifstream resolutionFile("Content/Saves/resolution.cff", std::ios::binary);
	if(resolutionFile)
	{
		// Load the resolution
		unsigned int width = 0, height = 0;
		resolutionFile.read(reinterpret_cast<char*>(&width), sizeof(unsigned int));
		resolutionFile.read(reinterpret_cast<char*>(&height), sizeof(unsigned int));
		resolutionFile.read(reinterpret_cast<char*>(&Game::windowStyle), sizeof(sf::Uint32));

		// Check if the loaded resolution is valid
		bool valid = false;
		std::vector<sf::VideoMode> modeList = sf::VideoMode::getFullscreenModes();
		for(auto&& mode : modeList)
		{
			// Valid, if width AND height are supported
			if(mode.width == width && mode.height == height)
			{
				valid = true;
				break;
			}
		}

		// Recreate the window
		if(valid)
			window.create(sf::VideoMode(width, height), "Council of Torment", windowStyle);
		else
			window.create(sf::VideoMode(800, 600), "Council of Torment", windowStyle);
	}
	else
	{
		// Create the default window, if no resolution file is saved
		window.create(sf::VideoMode(800, 600), "Council of Torment", windowStyle);
	}

	// Don't forget to close the file
	resolutionFile.close();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Registers all the states
 * ----------------------------------------------------------------------
 */
void Game::registerStates()
{
	// Register all states used in the game
	stateStack.registerState<TitleState>(States::TITLE);
	stateStack.registerState<MenuState>(States::MENU);
	stateStack.registerState<GameState>(States::GAME);
	stateStack.registerState<PauseState>(States::PAUSE);
	stateStack.registerState<DefocusState>(States::DEFOCUS);
	stateStack.registerState<SettingsState>(States::SETTINGS);
}