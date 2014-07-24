#include "GameState.h"
#include "Game.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the game state
 * ----------------------------------------------------------------------
 */
GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, gameWorld(context)
{
	// Change the state
	Game::currentState = States::GAME;

	// Set the camera to the player
	sf::View camera = context.window->getView();
	camera.zoom(0.5f);
	context.player->setCamera(camera);

	// Update the position of the inventory
	context.player->getInventory().updateInventoryPosition();

	for(auto i : playerMovement)
		i = false;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the state
 * ----------------------------------------------------------------------
 */
bool GameState::update(sf::Time elapsedTime)
{
	// Move the player
	if(getContext().player->isAbleToMove())
	{
		// Get the player
		Player& player = *getContext().player;

		// Move around
		if(!getContext().console->isVisible() && getContext().effectManager->getEffectList().empty())
		{
			if (sf::Keyboard::isKeyPressed(player.getAssignedKey(player.SNEAK)))
				player.setSpeed(40);
			else
				player.setSpeed(player.getDefaultSpeed());

			if(sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_UP)) && !sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_DOWN)))
			{
				player.move(NORTH, elapsedTime);
				playerMovement[NORTH - 1] = true;
			}
			else
				playerMovement[NORTH - 1] = false;
			if(sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_DOWN)) && !sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_UP)))
			{
				player.move(SOUTH, elapsedTime);
				playerMovement[SOUTH - 1] = true;
			}
			else
				playerMovement[SOUTH - 1] = false;
			if(sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_LEFT)) && !sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_RIGHT)))
			{
				player.move(WEST, elapsedTime);
				playerMovement[WEST - 1] = true;
			}
			else
				playerMovement[WEST - 1] = false;
			if(sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_RIGHT)) && !sf::Keyboard::isKeyPressed(player.getAssignedKey(player.MOVE_LEFT)))
			{
				player.move(EAST, elapsedTime);
				playerMovement[EAST - 1] = true;
			}
			else
				playerMovement[EAST - 1] = false;

			bool unMove = true;
			for(auto i : playerMovement)
				if(i)
					unMove = false;
			if(unMove)
				player.unMove();
		}
	}

	// Update the game world
	gameWorld.update(elapsedTime);

	return true;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the state
 * ----------------------------------------------------------------------
 */
void GameState::render()
{
	// Render the game world
	gameWorld.render();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events of the state
 * ----------------------------------------------------------------------
 */
bool GameState::handleEvent(const sf::Event& windowEvent)
{
	if(windowEvent.type == sf::Event::KeyPressed)
	{
		// Push the pause state if P or Escape was pressed
		if(windowEvent.key.code == sf::Keyboard::P || windowEvent.key.code == sf::Keyboard::Escape)
			requestStackPush(States::PAUSE);
	}

	// Pass the other events to the game world
	gameWorld.handleEvent(windowEvent);

	return true;
}