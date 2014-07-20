#include "Humanoid.h"
#include "LuaScript.h"
#include "MapLayer.h"
#include "GameMap.h"
#include "Console.h"
#include "Player.h"

#include <string>
#include <math.h>
#include <map>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the humanoid class
 * ----------------------------------------------------------------------
 */
	Humanoid::Humanoid(State::Context context, std::string entName, float xPos, float yPos)
	: context(context)
	, name(entName)
	, health(100)
	, noTrigger(false)
	, defaultVelocity(100)
	, velocity(100)
	, maximumHealth(100)
	, maximumMana(100)
	, mana(100)
	, frameX(0)
	, idleAnimation(IDLE_NORMAL)
	, walkAnimation(WALK_NORMAL)
	, currentAnimation(IDLE_NORMAL)
	, directionFacing(SOUTH)
	, defaultFrameTime(0.1f)
	, timer(defaultFrameTime)
	, frameY(currentAnimation)
	, isMoving(false)
	, playOnce(false)
	, dialogueMode(false)
	, poisonTimer(3.0f)
	, collisionBounds()
	, currentLine("line_1")
	, overheadText(context, "")
	{
		// Load the texture and set it to the sprite
		bodyTexture = context.contentManager->loadTexture("Content/Textures/Humanoids/" + name);
		bodySprite.setTexture(bodyTexture);
		bodySprite.setPosition(xPos * TILE_SIZE, yPos * TILE_SIZE / 2);
		bodySprite.setTextureRect(sf::IntRect(frameX * TILE_SIZE, frameY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		targetPos = bodySprite.getPosition();

		overheadText.setFontSize(5);
		overheadText.setSidePadding(1);
		overheadText.setTopPadding(1);

		// Set some default properties
		properties["hasOneHandedWeapon"] = false;
		properties["hasTwoHandedWeapon"] = false;
		properties["ignoresCollision"] = false;
		properties["aggressive"] = false;
		properties["isPoisoned"] = false;
		properties["invincible"] = false;
		properties["hasShield"] = false;
		properties["canMove"] = true;
		properties["visible"] = true;
		properties["isPlayer"] = false;
		properties["hasDialogue"] = false;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 19 July 2014
* Description: Updates the humanoid but this code doesn't apply to the player
* ----------------------------------------------------------------------
*/
	void Humanoid::updateNoPlayer(sf::Time elapsedTime)
	{
		float xDistance = 5;
		float yDistance = 25;
		collisionBounds = sf::FloatRect(targetPos.x + xDistance, targetPos.y + yDistance, TILE_SIZE - 2 * xDistance, TILE_SIZE - yDistance);

		// Update Dialogues
		overheadText.setPosition(sf::Vector2f(bodySprite.getPosition().x+16, bodySprite.getPosition().y - 5));

		if (context.player->getCollisionBounds().intersects(collisionBounds) && 
			dialogueMode == false && 
			context.player->isMoving &&
			properties["hasDialogue"] == true)
			{
				context.player->setProperty("isInDialogue", true);
				context.player->setTargetNPC(key);

				dialogueMode = true;


				sayLine("line_1");
			}

		// Check if player is out of range
		float x = bodySprite.getPosition().x - context.player->getPosition().x;
		float y = bodySprite.getPosition().y - context.player->getPosition().y;
		int maximumDistance = 100;

		if(dialogueMode && sqrt(x*x + y*y) > maximumDistance)
		{
			context.player->setProperty("isInDialogue", false);
			dialogueMode = false;
		}
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the humanoid
 * ----------------------------------------------------------------------
 */
	void Humanoid::update(sf::Time elapsedTime)
	{
		if(isMoving && properties["canMove"] && noTrigger)
		{
			// Move the humanoid depending on the direction
			switch(directionFacing)
			{
				// To the north
				case(NORTH):
					if(bodySprite.getPosition().y - velocity * elapsedTime.asSeconds() <= targetPos.y)
					{
						isMoving = false;
						noTrigger = false;
						bodySprite.setPosition(targetPos);
						break;
					}

					bodySprite.move(0, -velocity * elapsedTime.asSeconds());
					break;

				// To the south
				case(SOUTH):
					if(bodySprite.getPosition().y + velocity * elapsedTime.asSeconds() >= targetPos.y)
					{
						isMoving = false;
						noTrigger = false;
						bodySprite.setPosition(targetPos);
						break;
					}

					bodySprite.move(0, velocity * elapsedTime.asSeconds());
					break;

				// To the east
				case(EAST):
					if(bodySprite.getPosition().x + velocity * elapsedTime.asSeconds() >= targetPos.x)
					{
						isMoving = false;
						noTrigger = false;
						bodySprite.setPosition(targetPos);
						break;
					}

					bodySprite.move(velocity * elapsedTime.asSeconds(), 0);
					break;

				// To the west
				case(WEST):
					if(bodySprite.getPosition().x - velocity * elapsedTime.asSeconds() <= targetPos.x)
					{
						isMoving = false;
						noTrigger = false;
						bodySprite.setPosition(targetPos);
						break;
					}

					bodySprite.move(-velocity * elapsedTime.asSeconds(), 0);
					break;
			}
		}

		// Set a different animation if you move
		if(isMoving)
			currentAnimation = walkAnimation;
		else
			currentAnimation = idleAnimation;

		// Update some values
		frameY = currentAnimation;
		timer -= elapsedTime.asSeconds();

		// Change the frame time of the animation
		if(isMoving)
			defaultFrameTime = 0.1f;
		else
			defaultFrameTime = 0.2f;

		// Change the animation
		if(timer < 0) 
		{
			// Set the next frame
			timer = defaultFrameTime;
			frameX += 1;

			if(frameX * TILE_SIZE >= bodySprite.getTexture()->getSize().x)
			{
				if(playOnce)
				{
					properties["canMove"] = true;
					playOnce = false;
					idleAnimation = IDLE_NORMAL;
				}

				frameX = 0;
			}
		}

		// Update the texture rect
		bodySprite.setTextureRect(sf::IntRect(frameX * TILE_SIZE, frameY * TILE_SIZE, TILE_SIZE, TILE_SIZE));

		// Lose some life if poisoned
		if(properties["isPoisoned"] == true && properties["invincible"] == false)
		{
			poisonTimer -= elapsedTime.asSeconds();
			if(poisonTimer <= 0.0f)
			{
				health -= 1;
				poisonTimer = 3.0f;
			}
		}
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Draw the humanoid
 * ----------------------------------------------------------------------
 */
	void Humanoid::render()
	{
		// Save the float position and cast the sprite position to int
		sf::Vector2f position = bodySprite.getPosition();
		bodySprite.setPosition(float(int(position.x + 0.5f)), float(int(position.y + 0.5f)));

		// Render the humanoid
		if (properties["visible"] == true)
			context.window->draw(bodySprite);

		// Restore the position
		bodySprite.setPosition(position);

		if (dialogueMode == true)
			overheadText.render();

		// ENABLE TO SEE THE COLLISION RECTANGLE
		/*sf::RectangleShape shape;
		shape.setFillColor(sf::Color::Green);
		shape.setPosition(collisionBounds.left, collisionBounds.top);
		shape.setSize(sf::Vector2f(collisionBounds.width, collisionBounds.height));

		context.window->draw(shape);*/
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Move the humanoid
 * ----------------------------------------------------------------------
 */
	void Humanoid::move(int direction, int squares)
	{
		// Move the humanoid
		if(!isMoving)
		{
			// Change direction
			isMoving = true;
			noTrigger = true;
			directionFacing = direction;

			// Depending on the direction change the target position
			if(direction == NORTH) targetPos.y -= squares * TILE_SIZE;
			else if(direction == SOUTH) targetPos.y += squares * TILE_SIZE;
			else if(direction == WEST) targetPos.x -= squares * TILE_SIZE;
			else if(direction == EAST) targetPos.x += squares * TILE_SIZE;
		}
	}

	void Humanoid::move(int direction, sf::Time elapsedTime)
	{
		if(!noTrigger)
		{
			isMoving = true;
			directionFacing = direction;
			sf::Vector2f oldPos = targetPos;

			// Depending on the direction change the target position
			if(direction == NORTH) targetPos.y -= velocity * elapsedTime.asSeconds();
			else if(direction == SOUTH) targetPos.y += velocity * elapsedTime.asSeconds();
			else if(direction == WEST) targetPos.x -= velocity * elapsedTime.asSeconds();
			else if(direction == EAST) targetPos.x += velocity * elapsedTime.asSeconds();

			// Collision
			int xCoord = int((targetPos.x + 0.5f) / TILE_SIZE);
			int yCoord = int((targetPos.y + 0.5f) / TILE_SIZE);
			float xDistance = 5;
			float yDistance = 25;
			collisionBounds = sf::FloatRect(targetPos.x + xDistance, targetPos.y + yDistance, TILE_SIZE - 2 * xDistance, TILE_SIZE - yDistance);

			auto map = context.gameMap->getCollisionMap();

			for(int x = xCoord; x != xCoord + 2; ++x)
			{
				for(int y = yCoord; y != yCoord + 2; ++y)
				{
					for(auto& shape : map[x][y])
					{
						sf::FloatRect shapeBounds(shape.getPosition().x, shape.getPosition().y, shape.getLocalBounds().width, shape.getLocalBounds().height);
						if(shapeBounds.intersects(collisionBounds))
						{
							if(shape.getPointCount() == 4)
							{
								// Rectangle collision
								if(shape.getPoint(1).x > 0 && shape.getPoint(2).x > 0 && shape.getPoint(2).y > 0)
								{
									if(direction == NORTH) targetPos.y = shape.getPosition().y + shape.getLocalBounds().height - yDistance;
									else if(direction == SOUTH) targetPos.y = shape.getPosition().y - bodySprite.getLocalBounds().height;
									else if(direction == WEST) targetPos.x = shape.getPosition().x + shape.getLocalBounds().width - xDistance;
									else if(direction == EAST) targetPos.x = shape.getPosition().x - bodySprite.getLocalBounds().width + xDistance;
								}
							}
							else if(shape.getPointCount() == 3)
							{
								// Triangle collision
								if(shape.getPoint(1).x > 0 && shape.getPoint(2).x > 0 && shape.getPoint(2).y > 0)
								{
									// Triangle with no left down corner
									if(direction == NORTH)
									{
										if(collisionBounds.left + collisionBounds.width > shapeBounds.left + shapeBounds.width)
											targetPos.y = shape.getPosition().y + shape.getLocalBounds().height - yDistance;
										else
										{
											float factor = 1 - (shapeBounds.top + shapeBounds.height - collisionBounds.top) / shapeBounds.height;
											float width = shapeBounds.width * factor;
											if(targetPos.x + collisionBounds.width + xDistance > shapeBounds.left + width)
												targetPos.x = shapeBounds.left + width - collisionBounds.width - xDistance;
										}
									}
									else if(direction == EAST)
									{
										if(collisionBounds.top < shapeBounds.top)
											targetPos.x = shape.getPosition().x - bodySprite.getLocalBounds().width + xDistance;
										else
										{
											float factor = 1 - (shapeBounds.left + shapeBounds.width - (collisionBounds.left + collisionBounds.width)) / shapeBounds.width;
											float height = shapeBounds.height * factor;
											if(targetPos.y + yDistance < shapeBounds.top + height)
												targetPos.y = shapeBounds.top + height - yDistance;
										}
									}
									else if(direction == SOUTH && collisionBounds.top < shapeBounds.top)
										targetPos.y = shape.getPosition().y - bodySprite.getLocalBounds().height;
									else if(direction == WEST && collisionBounds.left + collisionBounds.width > shapeBounds.left + shapeBounds.width)
										targetPos.x = shape.getPosition().x + shape.getLocalBounds().width - xDistance;
								}
								else if(shape.getPoint(1).y > 0 && shape.getPoint(2).x < 0 && shape.getPoint(2).y > 0)
								{
									// Triangle with no left up corner
									if(direction == SOUTH)
									{
										if(collisionBounds.left + collisionBounds.width > shapeBounds.left + shapeBounds.width)
											targetPos.y = shape.getPosition().y - bodySprite.getLocalBounds().height;
										else
										{
											float factor = (shapeBounds.top + shapeBounds.height - (collisionBounds.top + collisionBounds.height)) / shapeBounds.height;
											float width = shapeBounds.width * factor;
											if(targetPos.x + collisionBounds.width + xDistance > shapeBounds.left + width)
												targetPos.x = shapeBounds.left + width - collisionBounds.width - xDistance;
										}
									}
									else if(direction == EAST)
									{
										if(collisionBounds.top + collisionBounds.height > shapeBounds.top + shapeBounds.height)
											targetPos.x = shape.getPosition().x - bodySprite.getLocalBounds().width + xDistance;
										else
										{
											float factor = (shapeBounds.left + shapeBounds.width - (collisionBounds.left + collisionBounds.width)) / shapeBounds.width;
											float height = shapeBounds.height * factor;
											if(targetPos.y + yDistance + collisionBounds.height > shapeBounds.top + height)
												targetPos.y = shapeBounds.top + height - yDistance - collisionBounds.height;
										}
									}
									else if(direction == NORTH && collisionBounds.top + collisionBounds.height > shapeBounds.top + shapeBounds.height)
										targetPos.y = shape.getPosition().y + shape.getLocalBounds().height - yDistance;
									else if(direction == WEST && collisionBounds.left + collisionBounds.width > shapeBounds.left + shapeBounds.width)
										targetPos.x = shape.getPosition().x + shape.getLocalBounds().width - xDistance;
								}
								else if(shape.getPoint(1).x < 0 && shape.getPoint(2).x < 0 && shape.getPoint(2).y < 0)
								{
									// Triangle with no right up corner
									if(direction == SOUTH)
									{
										if(collisionBounds.left < shapeBounds.left)
											targetPos.y = shape.getPosition().y - bodySprite.getLocalBounds().height;
										else
										{
											float factor = 1 - (shapeBounds.top + shapeBounds.height - (collisionBounds.top + collisionBounds.height)) / shapeBounds.height;
											float width = shapeBounds.width * factor;
											if(targetPos.x + xDistance < shapeBounds.left + width)
												targetPos.x = shapeBounds.left + width - xDistance;
										}
									}
									else if(direction == WEST)
									{
										if(collisionBounds.top + collisionBounds.height > shapeBounds.top + shapeBounds.height)
											targetPos.x = shape.getPosition().x + shape.getLocalBounds().width - xDistance;
										else
										{
											float factor = 1 - (shapeBounds.left + shape.getLocalBounds().width - collisionBounds.left) / shapeBounds.width;
											float height = shapeBounds.height * factor;
											if(targetPos.y + yDistance + collisionBounds.height > shapeBounds.top + height)
												targetPos.y = shapeBounds.top + height - yDistance - collisionBounds.height;
										}
									}
									else if(direction == NORTH && collisionBounds.top + collisionBounds.height > shapeBounds.top + shapeBounds.height)
										targetPos.y = shape.getPosition().y + shape.getLocalBounds().height - yDistance;
									else if(direction == EAST && collisionBounds.left < shapeBounds.left)
										targetPos.x = shape.getPosition().x - bodySprite.getLocalBounds().width + xDistance;
								}
								else if(shape.getPoint(1).y < 0 && shape.getPoint(2).x > 0 && shape.getPoint(2).y < 0)
								{
									// Triangle with no right down corner
									if(direction == NORTH)
									{
										if(collisionBounds.left < shapeBounds.left)
											targetPos.y = shape.getPosition().y + shape.getLocalBounds().height - yDistance;
										else
										{
											float factor = (shapeBounds.top + shapeBounds.height - collisionBounds.top) / shapeBounds.height;
											float width = shapeBounds.width * factor;
											if(targetPos.x + xDistance < shapeBounds.left + width)
												targetPos.x = shapeBounds.left + width - xDistance;
										}
									}
									else if(direction == WEST)
									{
										if(collisionBounds.top < shapeBounds.top)
											targetPos.x = shape.getPosition().x + shape.getLocalBounds().width - xDistance;
										else
										{
											float factor = (shapeBounds.left + shape.getLocalBounds().width - collisionBounds.left) / shapeBounds.width;
											float height = shapeBounds.height * factor;
											if(targetPos.y + yDistance < shapeBounds.top + height)
												targetPos.y = shapeBounds.top + height - yDistance;
										}
									}
									else if(direction == SOUTH && collisionBounds.top < shapeBounds.top)
										targetPos.y = shape.getPosition().y - bodySprite.getLocalBounds().height;
									else if(direction == EAST && collisionBounds.left < shapeBounds.left)
										targetPos.x = shape.getPosition().x - bodySprite.getLocalBounds().width + xDistance;
								}
							}
						}
					}
				}
			}

			bodySprite.setPosition(targetPos);
		}
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Plays a certain animation
 * ----------------------------------------------------------------------
 */
	void Humanoid::playAnimationOnce(int animationID)
	{
		// Play a certain animation
		frameX = 0;
		properties["canMove"] = false;
		playOnce = true;
		idleAnimation = animationID;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Sets a new position (coordinates as parameter)
 * ----------------------------------------------------------------------
 */
	void Humanoid::setPosition(float tileX, float tileY)
	{
		// Set a new position and set isMoving to false
		bodySprite.setPosition(tileX * TILE_SIZE, tileY * TILE_SIZE - 16);
		targetPos = bodySprite.getPosition();
		isMoving = false;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Sets a new position (position as parameter)
 * ----------------------------------------------------------------------
 */
	void Humanoid::setPosition(sf::Vector2f position)
	{
		// Set a new position
		bodySprite.setPosition(position);
		targetPos = position;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Sets the new health
 * ----------------------------------------------------------------------
 */
	void Humanoid::setHealth(int health)
	{
		// Set the new health
		this->health = health;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Sets the new mana
 * ----------------------------------------------------------------------
 */
	void Humanoid::setMana(int mana)
	{
		// Set the new mana
		this->mana = mana;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the position casted to int
 * ----------------------------------------------------------------------
 */
	sf::Vector2f Humanoid::getPosition() const
	{
		// Cast the sprite position to int
		sf::Vector2f position(float(int(bodySprite.getPosition().x + 0.5f)), float(int(bodySprite.getPosition().y + 0.5f)));

		// Return the position
		return position;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the texture rect of the humanoid
 * ----------------------------------------------------------------------
 */
	sf::IntRect Humanoid::getTextureRext() const
	{
		// Return the texture rect
		return bodySprite.getTextureRect();
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 17 March 2014
 * Description: Returns the relative position of the player in squares
 * ----------------------------------------------------------------------
 */
	sf::Vector2f Humanoid::getRelativePosition() const
	{
		float x = bodySprite.getPosition().x / TILE_SIZE;
		float y = (bodySprite.getPosition().y + TILE_SIZE/2) / TILE_SIZE;

		return sf::Vector2f(x, y);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Sets the velocity
 * ----------------------------------------------------------------------
 */
	void Humanoid::setSpeed(int speed)
	{
		// Set the velocity
		velocity = speed;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Sets the default velocity
 * ----------------------------------------------------------------------
 */
	void Humanoid::setDefaultSpeed(int speed)
	{
		// Set the velocity
		defaultVelocity = speed;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 26th May 2014
* Description: Sets if the humanoid can move or not.
* ----------------------------------------------------------------------
*/
	void Humanoid::setCanMove(bool flag)
	{
		properties["canMove"] = flag;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 26th May 2014
* Description: Sets the idle animation of the humanoid
* ----------------------------------------------------------------------
*/
	void Humanoid::setIdleAnimation(int id)
	{
		idleAnimation = id;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 13th July 2014
* Description: Sets a certain key in the properties list
* ----------------------------------------------------------------------
*/
	void Humanoid::setProperty(std::string key, bool value)
	{
		if (properties.find(key) == properties.end()) {
			context.console->logError("Unable to find humanoid value [" + key + "] for " + name);
		}
		else {
			properties[key] = value;
		}
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 13th July 2014
* Description: Returns the value at a given key
* ----------------------------------------------------------------------
*/
	bool Humanoid::getProperty(std::string key)
	{
		if (properties.find(key) == properties.end()) {
			context.console->logError("Unable to find humanoid value [" + key + "] for " + name);
			return false;
		}
		else {
			return properties[key];
		}
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 17th July 2014
* Description: Sets the name of the humanoid entity
* ----------------------------------------------------------------------
*/
	void Humanoid::setName(std::string n)
	{
		name = n;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 17th July 2014
* Description: Gets the name of the humanoid entity
* ----------------------------------------------------------------------
*/
	std::string Humanoid::getName()
	{
		return name;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 17th July 2014
* Description: Gets the sprite of the humanoid entity
* ----------------------------------------------------------------------
*/
	sf::Sprite& Humanoid::getSprite()
	{
		return bodySprite;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18th July 2014
* Description: Starts the dialogue mode
* ----------------------------------------------------------------------
*/
	void Humanoid::setDialogueFile(std::string filename)
	{
		dialogueFile = filename;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18th July 2014
* Description: Says a certain line
* ----------------------------------------------------------------------
*/
	void Humanoid::sayLine(std::string lineID)
	{
		if (dialogueMode)
		{
			currentLine = lineID;

			LuaScript script(context, "Content/Dialogues/" + dialogueFile);
			std::string headText = script.get<std::string>(currentLine + ".words");

			for (size_t i = 0; i < headText.size(); i += 34)
			{
				for (unsigned int start = i; start > 0; start--)
				{
					if (headText[start] == ' ')
					{
						headText.insert(start, "\n");
						break;
					}
				}
			}

			overheadText.setText(headText);
			overheadText.setPosition(sf::Vector2f(bodySprite.getPosition().x, bodySprite.getPosition().y-overheadText.getText().getGlobalBounds().height));

			int lineCount = script.get<int>("number_of_lines");
			int optionCount = script.get<int>(currentLine + ".options.count");

			context.player->flushDialogues();

			for (int i = 1; i <= optionCount; ++i)
			{
				DialogueBox optionBox(context, "t");

				optionBox.id = i;
				optionBox.setFontSize(13);
				optionBox.setSidePadding(10);
				optionBox.setTopPadding(7);
				optionBox.setText(script.get<std::string>(currentLine + ".options.option" + std::to_string(i) + ".text"), 250);
				optionBox.setTextColor(sf::Color::White);
				optionBox.setExecution(script.get<std::string>(currentLine + ".options.option" + std::to_string(i) + ".next_line"));

				if (i == 1)
					optionBox.setPosition(sf::Vector2f(context.window->getSize().x / 2.0f, context.window->getSize().y / 1.5f));
				else
				{
					sf::Vector2f exPos = context.player->getDialogueReplyList()[i - 1].getPosition();
					optionBox.setPosition(sf::Vector2f(exPos.x, exPos.y + optionBox.getFontSize() + optionBox.getTopPadding() * 2 + 2));
				}

				if (optionBox.getText().getString() == "Good-Bye." || optionBox.getText().getString() == "Goodbye.")
					optionBox.setTextColor(sf::Color(199, 44, 5));

				context.player->getDialogueReplyList()[i] = optionBox;
			}
		}
	}