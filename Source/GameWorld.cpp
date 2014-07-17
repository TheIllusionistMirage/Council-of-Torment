#include "GameWorld.h"
#include "Game.h"
#include "Stdafx.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the game world
 * ----------------------------------------------------------------------
 */
GameWorld::GameWorld(State::Context context)
: context(context)
{
	// Create the render texture
	if(!roofTexture.create(context.window->getSize().x, context.window->getSize().y))
		context.window->close();

	// Load the default map
	context.gameMap->loadMap(DEFAULT_LOADED_MAP, SOUTH);
	//context.contentManager->playMusic(Music::GIDEON_TOWER);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the game world
 * ----------------------------------------------------------------------
 */
void GameWorld::update(sf::Time elapsedTime)
{
	// Update the global time
	context.globalTime->update(elapsedTime);

	// Update the player
	if(Game::currentState == States::GAME)
		context.player->update(elapsedTime);

	// Update the camera
	context.player->updateCamera();

	// Update the game map
	context.gameMap->update(elapsedTime);
	context.gameMap->updateNPCs(elapsedTime);

	// Update the light manager
	if(Game::currentState == States::GAME)
		context.lightManager->update(context.gameMap, elapsedTime);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the game world
 * ----------------------------------------------------------------------
 */
void GameWorld::render()
{
	// Update the camera
	context.player->updateCamera();
	sf::Vector2f cameraPos(context.player->getCamera().getCenter() - sf::Vector2f(context.window->getSize() / 2u));

	// Render some layers of the map
	context.gameMap->render("background");
	context.gameMap -> renderAnimations("background");
	context.gameMap->render("background2");
	context.gameMap->render("object");
	context.gameMap -> renderAnimations("object");
	context.gameMap->render("object2");
	context.gameMap->render("shadow");
	context.gameMap->render("object3");
	context.gameMap -> renderAnimations("shadow");

	// Render the image layer of the map
	std::vector<sf::Sprite>& imageLayerReference = context.gameMap -> getImageLayer();

	for (auto&& x : imageLayerReference)
		if (x.getPosition().y <= context.player->getPosition().y)
			if (isOnScreen(x, context.player->getCamera()))
				context.window->draw(x);

	for (auto&& npc : context.gameMap->getNPCs())
		if (npc.second->getSprite().getPosition().y <= context.player->getPosition().y)
			if (isOnScreen(npc.second->getSprite(), context.player->getCamera()))
				npc.second->render();

	// Render the player ---------------------------------------
	if(Game::currentState == States::GAME)
		context.player->render();
	// Render the player ---------------------------------------

	for (auto&& npc : context.gameMap->getNPCs())
		if (npc.second->getSprite().getPosition().y > context.player->getPosition().y)
			if (isOnScreen(npc.second->getSprite(), context.player->getCamera()))
				npc.second->render();

	for (auto&& x : imageLayerReference)
		if (x.getPosition().y > context.player->getPosition().y)
			if (isOnScreen(x, context.player->getCamera()))
				context.window->draw(x);

	context.gameMap->render("roof");
	context.gameMap->render("roof2");
	context.gameMap->render("roof-shadow");
	context.gameMap -> renderAnimations("roof");

	// Render the light manager
	if(Game::currentState == States::GAME)
		context.lightManager->render(context.player->getCamera());

	if(context.player->showCollision())
		context.player->renderCollisionShapes();

	if (Game::currentState == States::GAME)
		context.player->renderGUI();

	// Render the inventory
	if(Game::currentState == States::GAME)
		context.player->getInventory().render();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events of the world
 * ----------------------------------------------------------------------
 */
void GameWorld::handleEvent(const sf::Event& windowEvent)
{
	// Pass the other events to the player
	if(Game::currentState == States::GAME)
		context.player->handleEvent(windowEvent);
}