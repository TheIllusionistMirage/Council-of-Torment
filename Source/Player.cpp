#include "Player.h"
#include "GameMap.h"
#include <string>
#include <map>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the player
 * ----------------------------------------------------------------------
 */
	Player::Player(State::Context context, std::string entName, float xPos, float yPos)
	: Humanoid(context, entName, xPos, yPos)
	, armorManager(context)
	, inventory(context)
	, camera()
	, keyBindings()
	, hunger(0)
	, thirst(0)
	, collision(false)
	{
		// Set initial key bindings
		keyBindings[sf::Keyboard::A] = MOVE_LEFT;
		keyBindings[sf::Keyboard::D] = MOVE_RIGHT;
		keyBindings[sf::Keyboard::W] = MOVE_UP;
		keyBindings[sf::Keyboard::S] = MOVE_DOWN;

		properties["hideGUI"] = false;

		std::unique_ptr<GUI::Label> l(new GUI::Label("test", context.contentManager->getFont(Fonts::SEGOEUI), sf::Vector2f(25,25)));
		l->setPosition(25, 25);
		l->setText("Health: null");
		l->setColor(sf::Color::White);
		l->setCharacterSize(14);

		statsLabels.insert(std::make_pair("hp", std::move(l)));
		
		std::unique_ptr<GUI::Label> l2(new GUI::Label("test", context.contentManager->getFont(Fonts::SEGOEUI), sf::Vector2f(25, 25)));
		l2->setPosition(25, 40);
		l2->setText("Mana: null");
		l2->setColor(sf::Color::White);
		l2->setCharacterSize(14);

		statsLabels.insert(std::make_pair("mana", std::move(l2)));
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 29 January 2014
 * Description: Updates the player
 * ----------------------------------------------------------------------
 */
	void Player::update(sf::Time elapsedTime)
	{
		// Update the humanoid part
		Humanoid::update(elapsedTime);

		// Update the armor manager 
		armorManager.update(*this);

		// Update the inventory
		inventory.update(elapsedTime);


		if(armorManager.containsPart(ArmorType::WEAPON_1H))
		{
			idleAnimation = IDLE_HOLDINGITEM;
			walkAnimation = WALK_HOLDINGITEM;
		}
		else if(armorManager.containsPart(ArmorType::WEAPON_2H))
		{
			idleAnimation = IDLE_HOLDINGSPEAR;
			walkAnimation = WALK_HOLDINGSPEAR;
		}

		// Update the user interface
		statsLabels["hp"]->setText("Health: " + std::to_string(health));
		statsLabels["mana"]->setText("Mana: " + std::to_string(mana));
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date:29 January 2014
 * Description: Renders the player
 * ----------------------------------------------------------------------
 */
	void Player::render()
	{
		// Render the humanoid part
		Humanoid::render();

		// Render the armor manager
		armorManager.render();
	}

	void Player::renderCollisionShapes()
	{
		auto map = context.gameMap->getCollisionMap();
		int xCoord = int((camera.getCenter().x - camera.getSize().x / 2) / TILE_SIZE);
		int yCoord = int((camera.getCenter().y - camera.getSize().y / 2) / TILE_SIZE);
		int width = int(context.window->getDefaultView().getSize().x / TILE_SIZE);
		int height = int(context.window->getDefaultView().getSize().y / TILE_SIZE);

		for(int x = xCoord; x != xCoord + width; ++x)
		{
			for(int y = yCoord; y != yCoord + height; ++y)
			{
				for(auto& shape : map[x][y])
				{
					if(shape.getPointCount() == 3)
					{
						if(shape.getPoint(1).x < 0 || shape.getPoint(2).x < 0)
							shape.setPosition(shape.getPosition().x + shape.getLocalBounds().width, shape.getPosition().y);

						if(shape.getPoint(1).y < 0 || shape.getPoint(2).y < 0)
							shape.setPosition(shape.getPosition().x, shape.getPosition().y + shape.getLocalBounds().height);
					}

					if(shape.getFillColor() != sf::Color::Red)
						shape.setFillColor(collision_debug_color);
					context.window->draw(shape);
				}
			}
		}
	}
/* ----------------------------------------------------------------------
* Author: Octav
* Date: 1 May 2014
* Description: Renders the player's GUI
* ----------------------------------------------------------------------
*/
	void Player::renderGUI()
	{
		context.window->setView(context.window->getDefaultView());

		if (properties["hideGUI"] == false)
		{
			for (auto&& i : statsLabels)
				context.window->draw(*i.second);
		}

		context.window->setView(context.player->getCamera());
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the camera
 * ----------------------------------------------------------------------
 */
	void Player::updateCamera()
	{
		// Set the center of the camera
		camera.setCenter(getPosition().x + bodySprite.getGlobalBounds().width / 2, getPosition().y + bodySprite.getGlobalBounds().height / 2);

		// Adjust the camera size
		if(camera.getSize().x > context.gameMap->getMapWidth() * TILE_SIZE || camera.getSize().y > context.gameMap->getMapHeight() * TILE_SIZE)
		{
			// Calculate the zoom factor
			float zoomFactor = 0.0f;
			if(camera.getSize().x > context.gameMap->getMapWidth() * TILE_SIZE)
				zoomFactor = float(context.gameMap->getMapWidth() * TILE_SIZE) / context.window->getSize().x;
			else
				zoomFactor = float(context.gameMap->getMapHeight() * TILE_SIZE) / context.window->getSize().y;

			// Set the new size
			camera.setSize(context.window->getSize().x * zoomFactor, context.window->getSize().y * zoomFactor);
		}

		// Adjust the camera to the map
		if(camera.getCenter().x - camera.getSize().x / 2.0f < 0)
			camera.setCenter(camera.getSize().x / 2.0f, camera.getCenter().y);
		if(camera.getCenter().y - camera.getSize().y / 2.0f < 0)
			camera.setCenter(camera.getCenter().x, camera.getSize().y / 2.0f);
		if(camera.getCenter().x + camera.getSize().x / 2.0f > context.gameMap->getMapWidth() * TILE_SIZE)
			camera.setCenter(context.gameMap->getMapWidth() * TILE_SIZE - camera.getSize().x / 2.0f, camera.getCenter().y);
		if(camera.getCenter().y + camera.getSize().y / 2.0f > context.gameMap->getMapHeight() * TILE_SIZE)
			camera.setCenter(camera.getCenter().x, context.gameMap->getMapHeight() * TILE_SIZE - camera.getSize().y / 2.0f);

		// Set the camera to the window
		context.window->setView(camera);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Sets the camera
 * ----------------------------------------------------------------------
 */
	void Player::setCamera(sf::View camera)
	{
		// Set the camera
		this->camera = camera;
		updateCamera();
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Changes the sprite of the player
 * ----------------------------------------------------------------------
 */
	void Player::changeSprite(std::string fileName)
	{
		// Load a new texture and set it to the sprite
		bodyTexture = context.contentManager->loadTexture(fileName);
		bodySprite.setTexture(bodyTexture);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events of the player
 * ----------------------------------------------------------------------
 */
	void Player::handleEvent(const sf::Event& windowEvent)
	{
		if(windowEvent.type == sf::Event::KeyPressed)
		{
			// Zoom in our out
			if(windowEvent.key.code == sf::Keyboard::F5) 
				zoomIn();
			else if(windowEvent.key.code == sf::Keyboard::F6)
				zoomOut();
			else if(windowEvent.key.code == sf::Keyboard::F7)
			{
				// Reset the camera size
				camera.setSize(context.window->getSize().x / 2.0f, context.window->getSize().y / 2.0f);
			}
		}

		else if(windowEvent.type == sf::Event::MouseWheelMoved && !inventory.isOpen())
		{
			// Zoom in or out
			if(windowEvent.mouseWheel.delta > 0)
				zoomIn();
			else if(windowEvent.mouseWheel.delta < 0)
				zoomOut();
		}

		else if(windowEvent.type == sf::Event::MouseButtonPressed)
		{
			// Reset the camera size
			if(windowEvent.mouseButton.button == sf::Mouse::Middle)
				camera.setSize(context.window->getSize().x / 2.0f, context.window->getSize().y / 2.0f);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			setSpeed(defaultVelocity*2);
		else
			setSpeed(defaultVelocity);

		// Pass the other events to the inventory
		inventory.handleEvent(windowEvent);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 29 January 2014
 * Description: Zooms the camera out
 * ----------------------------------------------------------------------
 */
	void Player::zoomOut()
	{
		// Zooming out
		camera.zoom(1.1f);

		// Prevent the view to be too zoomed out
		if(camera.getSize().x > context.window->getSize().x)
			camera.setSize(float(context.window->getSize().x), float(context.window->getSize().y));
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 29 January 2014
 * Description: Zooms the camera in
 * ----------------------------------------------------------------------
 */
	void Player::zoomIn()
	{
		// Zooming in
		camera.zoom(0.9f);

		// Prevent the view to be too zoomed in
		if(camera.getSize().x < context.window->getSize().x / 10)
			camera.setSize(float(context.window->getSize().x / 10), float(context.window->getSize().y / 10));
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Assigns a new key to the key bindings
 * ----------------------------------------------------------------------
 */
	void Player::assignKey(Action action, sf::Keyboard::Key key)
	{
		// Remove all keys that already map to action
		for(auto i = keyBindings.begin(); i != keyBindings.end();)
		{
			if(i->second == action)
				keyBindings.erase(i++);
			else
				++i;
		}

		// Insert new binding
		keyBindings[key] = action;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns an assigned key
 * ----------------------------------------------------------------------
 */
	sf::Keyboard::Key Player::getAssignedKey(Action action) const
	{
		for(auto key : keyBindings)
		{
			if(key.second == action)
				return key.first;
		}

		return sf::Keyboard::Unknown;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns true if the player can move
 * ----------------------------------------------------------------------
 */
	bool Player::isAbleToMove()
	{
		// Return true if the player can move
		return properties["canMove"];
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 29 January 2014
 * Description: Returns the inventory
 * ----------------------------------------------------------------------
 */
	Inventory& Player::getInventory()
	{
		// Return the inventory
		return inventory;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 31 January 2014
 * Description: Returns the camera
 * ----------------------------------------------------------------------
 */
	sf::View& Player::getCamera()
	{
		// Return the camera
		return camera;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 04 April 2014
 * Description: Sets the trigger which the player is moving to.
 * Parameter: s will be the name of the level_change trigger
 * ----------------------------------------------------------------------
 */
	void Player::setDestinationLevelExit(std::string s)
	{
		destinationLevel = s;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 04 April 2014
 * Description: returns the destination trigger name
 * ----------------------------------------------------------------------
 */
	std::string Player::getDstinationLevel()
	{
		return destinationLevel;
	}