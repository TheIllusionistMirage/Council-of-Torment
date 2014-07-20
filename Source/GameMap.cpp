#include "LightManager.h"
#include "LuaScript.h"
#include "GameMap.h"
#include "Console.h"
#include "Player.h"

#include <time.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the game map
 * ----------------------------------------------------------------------
 */
	GameMap::GameMap(State::Context context)
	: context(context)
	, currentMapName("")
	, spawnLocation(0, 0)
	, mapHeight(0)
	, mapWidth(0)
	, naturalLight(false)
	{
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Parses a Tmx-map
 * ----------------------------------------------------------------------
 */
	void GameMap::parseFile()
	{
		// Create a new map
		map = std::unique_ptr<Tmx::Map>(new Tmx::Map());

		// Parse the map
		map->ParseFile("Content/Maps/" + currentMapName);
		if(map->HasError())
			context.console->logError("Failed to load map: Content/Maps/" + currentMapName);
		else
		{
			// Clear the data 
			NPCs.clear();
			layerList.clear();
			objectList.clear();
			mapTriggers.clear();
			collisionMap.clear();
			animationMap.clear();
			imageLayer.clear();
			context.lightManager->clearLights();

			// Check if the lighting is natural
			if(map -> GetProperties().GetList()["natural_light"] == "True")
				naturalLight = true;
			else
				naturalLight = false;

			// Iterate through the layers.
			for(auto&& layer : map->GetLayers())
			{
				// Add the layers
				std::vector<Tmx::MapTile> mapTile;
				MapLayer mapLayer(context, layer->GetName(), currentMapName, layer->GetWidth(), layer->GetHeight());
				mapHeight = layer->GetHeight();
				mapWidth = layer->GetWidth();

				for(int y = 0; y != layer->GetHeight(); ++y)
				{
					mapTile.clear();

					// Add the tile to the layer
					for(int x = 0; x != layer->GetWidth(); ++x)
						mapTile.push_back(layer->GetTile(x, y));

					mapLayer.pushTile(mapTile);
				}

				// Push a layer
				layerList.push_back(mapLayer);
			}

			// Iterate through the object groups
			for(auto&& objectGroup : map->GetObjectGroups())
			{
				// Parse the image layer
				if (objectGroup->GetName() == "images")
				{
					for (auto&& object : objectGroup->GetObjects())
					{
						sf::Vector2f imagePos = {(float)object->GetX(), (float)object->GetY()-32};
						sf::Sprite image;
						unsigned int gid = object->GetGid();
						
						image.setTexture(context.contentManager->getTileset(currentMapName.substr(0, currentMapName.size() - 4)));
						image.setTextureRect(sf::IntRect(((gid % TILES_PER_ROW) * TILE_SIZE)-32, (gid / TILES_PER_ROW) * TILE_SIZE, 32, 32));
						image.setPosition(imagePos);

						imageLayer.push_back(image);
					}
				}

				// Parse the collision layer
				else if (objectGroup->GetName() == "collision")
				{
					for (auto&& object : objectGroup->GetObjects())
					{
						const Tmx::Polygon* poly = object->GetPolygon();
						if (poly != 0)
						{
							sf::ConvexShape shape(object->GetPolygon()->GetNumPoints());

							for (int i = 0; i != object->GetPolygon()->GetNumPoints(); ++i)
								shape.setPoint(i, sf::Vector2f(float(object->GetPolygon()->GetPoint(i).x), float(object->GetPolygon()->GetPoint(i).y)));

							shape.setPosition(float(object->GetX()), float(object->GetY()));

							if (shape.getPointCount() == 3)
							{
								if (shape.getPoint(1).x < 0 || shape.getPoint(2).x < 0)
									shape.setPosition(shape.getPosition().x - shape.getLocalBounds().width, shape.getPosition().y);

								if (shape.getPoint(1).y < 0 || shape.getPoint(2).y < 0)
									shape.setPosition(shape.getPosition().x, shape.getPosition().y - shape.getLocalBounds().height);

								if (!(shape.getPoint(1).x > 0 && shape.getPoint(2).x > 0 && shape.getPoint(2).y > 0) &&
									!(shape.getPoint(1).y > 0 && shape.getPoint(2).x < 0 && shape.getPoint(2).y > 0) &&
									!(shape.getPoint(1).x < 0 && shape.getPoint(2).x < 0 && shape.getPoint(2).y < 0) &&
									!(shape.getPoint(1).y < 0 && shape.getPoint(2).x > 0 && shape.getPoint(2).y < 0))
								{
									context.console->logError("Invalid collision shape!");
									shape.setFillColor(sf::Color::Red);
								}
							}
							else if (shape.getPointCount() == 4)
							{
								if (!(shape.getPoint(1).x > 0 && shape.getPoint(2).x > 0 && shape.getPoint(2).y > 0))
								{
									context.console->logError("Invalid collision shape!");
									shape.setFillColor(sf::Color::Red);
								}
							}
							else
							{
								shape.setFillColor(sf::Color::Red);
								context.console->logError("Polygon has too many vertizes!");
							}

							int lastXCoord = static_cast<int>((shape.getPosition().x + shape.getLocalBounds().width - 0.5f) / TILE_SIZE);
							int lastYCoord = static_cast<int>((shape.getPosition().y + shape.getLocalBounds().height - 0.5f) / TILE_SIZE);
							bool alreadyIn = false;

							for (float x = shape.getPosition().x; x < shape.getPosition().x + shape.getLocalBounds().width; x += TILE_SIZE)
							{
								for (float y = shape.getPosition().y; y < shape.getPosition().y + shape.getLocalBounds().height; y += TILE_SIZE)
								{
									int xCoord = static_cast<int>(x / TILE_SIZE);
									int yCoord = static_cast<int>(y / TILE_SIZE);
									collisionMap[xCoord][yCoord].push_back(shape);
									if (xCoord == lastXCoord && yCoord == lastYCoord)
										alreadyIn = true;
								}
							}

							if (!alreadyIn)
								collisionMap[lastXCoord][lastYCoord].push_back(shape);
						}
						else
							context.console->logError("Invalid polygon!");
					}
				}

				// Parse the trigger layer.
				else if (objectGroup -> GetName() == "triggers")
					for(int i = 0; i != objectGroup->GetNumObjects(); ++i)
					{
						auto object = objectGroup->GetObject(i);
						auto properties = object->GetProperties();

						if(object->GetType() == "player_spawn")
						{
							// Spawn location of the player
							spawnLocation.x = float(object->GetX() / TILE_SIZE);
							spawnLocation.y = float(object->GetY() / TILE_SIZE);

							// Set the player to the spawn location
							context.player->setPosition(spawnLocation.x, spawnLocation.y - 1.0f);
						}
						else if(object -> GetType() == "level_change" && 
								object -> GetName() == context.player->getDestinationLevel())
								{
									// Spawn location of the player
									spawnLocation.x = float(object->GetX() / TILE_SIZE);
									spawnLocation.y = float(object->GetY() / TILE_SIZE);

									// Set the player to the spawn location
									context.player->setPosition(spawnLocation.x, spawnLocation.y - 1.0f);

									std::unique_ptr<MapTrigger> trigger(new MapTrigger(context, TriggerType::CALLBACK, false));

									trigger->setType (TriggerType::LEVEL_CHANGE);
									trigger->setPosition(sf::Vector2f(float(object->GetX()), object->GetY() - TILE_SIZE * 2.0f));
									trigger->setProperties(object->GetProperties().GetList());
									trigger->setName(object->GetName());

									mapTriggers.insert(std::pair<std::string, std::unique_ptr<MapTrigger>>(object->GetName(), std::move(trigger)));
								}
						else if(object -> GetType() == "animation")
						{
							std::map<std::string, std::string> p = object -> GetProperties().GetList();

							std::unique_ptr<AnimationTile> tile(new AnimationTile(context,p["layer"], p["file"], std::stoi(p["frame_size"]), std::stof(p["frame_time"])));
							tile->setPosition(sf::Vector2f(float(object->GetX()), float(object->GetY() - 32)));
							animationMap.push_back(std::move(tile));
						}
						else if(object->GetType() == "light_source") 
						{
							sf::Uint8 red = std::stoi(object->GetProperties().GetList()["red"]);
							sf::Uint8 green = std::stoi(object->GetProperties().GetList()["green"]);
							sf::Uint8 blue = std::stoi(object->GetProperties().GetList()["blue"]);
							sf::Uint8 alpha = std::stoi(object->GetProperties().GetList()["alpha"]);
							bool enableFlickering = false;
							if(object->GetProperties().GetList()["flickering"] == "True")
								enableFlickering = true;
							float flickerRadius = 0.0f;
							float flickerSpeed = 0.0f;

							context.lightManager->addLight(float(object->GetX() + TILE_SIZE / 2), float(object->GetY() + TILE_SIZE / 2 - TILE_SIZE), std::stof(object->GetProperties().GetList()["radius"]), sf::Color(red, green, blue, alpha));

							if(enableFlickering)
							{
								flickerRadius = std::stof(object->GetProperties().GetList()["flicker_radius"]);
								flickerSpeed = std::stof(object->GetProperties().GetList()["flicker_speed"]);
								context.lightManager->getLightList().back().enableFlickering(flickerRadius, flickerSpeed);
							}
						}
						else if (object->GetType() == "npc")
						{
							LuaScript script{ context, "Content/Scripts/npcs.lua" };
							std::string luaKey = "npc_" + object->GetProperties().GetList()["id"];

							std::unique_ptr<Humanoid> h(new Humanoid(context, script.get<std::string>(luaKey+".file")));

							h -> setPosition(sf::Vector2f((float)object->GetX(), (float)object->GetY() - 32));
							h -> setDefaultSpeed(script.get<int>(luaKey+".velocity"));
							h -> setName(script.get<std::string>(luaKey+".name"));
							h -> setHealth(script.get<int>(luaKey+".health"));
							h -> setMana(script.get<int>(luaKey + ".mana"));
							h -> setIdleAnimation(script.get<int>(luaKey + ".idle_animation"));
							h -> setCanMove(script.get<bool>(luaKey+".can_move"));
							h -> setDialogueFile(object->GetProperties().GetList()["dialogue"]);
							h -> key = object->GetName();

							NPCs.insert(std::pair<std::string, std::unique_ptr<Humanoid>>(object->GetName(), std::move(h)));
							context.console->logWarning("Inserted NPC '" + script.get<std::string>(luaKey + ".name") + "' at " + std::to_string(object->GetX() / TILE_SIZE) + ":" + std::to_string((object->GetY() - 32) / TILE_SIZE));
						}
						else if(!object->GetType().empty())
						{
							// Create a new map trigger
							std::unique_ptr<MapTrigger> trigger(new MapTrigger(context, TriggerType::CALLBACK, false));

							// Set it's type
							if(object->GetType() == "console_print")		trigger->setType (TriggerType::CONSOLE_PRINT);
							else if(object->GetType() == "level_change")	trigger->setType (TriggerType::LEVEL_CHANGE);
							else if(object->GetType() == "animation_play")	trigger->setType(TriggerType::ANIMATION_PLAY);
							else if(object->GetType() == "animation_loop")	trigger->setType(TriggerType::ANIMATION_LOOP);
							else if(object->GetType() == "player_move")		trigger->setType(TriggerType::PLAYER_MOVE);
							else if(object->GetType() == "tile_change")		trigger->setType(TriggerType::TILE_CHANGE);
							else if(object->GetType() == "console_command")	trigger->setType(TriggerType::CONSOLE_COMMAND);

							// Set some properties
							trigger->setPosition(sf::Vector2f(float(object->GetX()), object->GetY() - TILE_SIZE * 2.0f));
							trigger->setProperties(object->GetProperties().GetList());
							trigger->setName(object->GetName());

							if (object->GetProperties().GetList()["NpcOnly"] == "True")
								trigger->setNpcOnly(true);

							if (object->GetProperties().GetList()["execution_type"] == "STEP_ON_CENTER")
								trigger->setExecType(ExecType::STEP_ON_CENTER);
							else if (object->GetProperties().GetList()["execution_type"] == "STEP_ON_DELAYED")
								trigger->setExecType(ExecType::STEP_ON_DELAYED);
							else
								trigger->setExecType(ExecType::STEP_ON);

							// Add the trigger to the map
							if(trigger->isNpcOnly())
								npcTriggers.insert(std::pair<std::string, std::unique_ptr<MapTrigger>>(object->GetName(), std::move(trigger)));
							else
								mapTriggers.insert(std::pair<std::string, std::unique_ptr<MapTrigger>>(object->GetName(), std::move(trigger)));
						}
					}
			}

			context.console->logInfo("Loaded map " + currentMapName + ".tmx into memory.");
			context.console->logInfo("Current map contains " + std::to_string(mapTriggers.size()) + " triggers.");

			if(naturalLight == true)
				context.console -> logInfo("Map has natural lighting.");
			else if(naturalLight == false)
				context.console -> logInfo("Map does not have natural lighting.");
		}
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Loads a map
 * ----------------------------------------------------------------------
 */
	void GameMap::loadMap(const std::string& filename, Direction direction)
	{
		// Load a map and reset the map name
		currentMapName = filename;
		parseFile();

		// Move the player
		if(direction != NO_DIRECTION)
			context.player->move(direction, 1);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Reloads the map
 * ----------------------------------------------------------------------
 */
	void GameMap::reload()
	{
		// Reload the map
		loadMap(currentMapName);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Triggers a trigger if the player stands over it
 * ----------------------------------------------------------------------
 */
	void GameMap::update(sf::Time elapsedTime)
	{
		for(auto&& script : mapTriggers)
		{
			sf::FloatRect bounds = context.player->getCollisionBounds();

			switch (script.second->getExecType())
			{
				case ExecType::STEP_ON:
					// Check if the player is standing over a trigger and trigger it
					if (!context.player->cantTrigger() && bounds.left + bounds.width > script.second->getPosition().x && bounds.left < script.second->getPosition().x + TILE_SIZE &&
						bounds.top + bounds.height > script.second->getPosition().y + TILE_SIZE && bounds.top < script.second->getPosition().y + 2 * TILE_SIZE)
						{
							// Execute the trigger
							if (!script.second->isTriggered())
								script.second->executeTrigger();

							// Player is standing on the trigger
							script.second->setPlayerStandingOn(true);
						}
					else
						script.second->setPlayerStandingOn(false);
					break;


				case ExecType::STEP_ON_CENTER:
					{
						int offset = 15;
						bounds.left += 7;
						bounds.width = bounds.width / 2;
						bounds.height = bounds.height / 2;

						if (!context.player->cantTrigger() && bounds.left + bounds.width > script.second->getPosition().x + offset && bounds.left < script.second->getPosition().x - offset + TILE_SIZE &&
							bounds.top + bounds.height > script.second->getPosition().y + TILE_SIZE - offset && bounds.top < script.second->getPosition().y + offset + TILE_SIZE)
							{
								// Execute the trigger
								if (!script.second->isTriggered())
									script.second->executeTrigger();

								// Player is standing on the trigger
								script.second->setPlayerStandingOn(true);
							}
						else
							script.second->setPlayerStandingOn(false);
					}
				break;

				case ExecType::STEP_ON_DELAYED:
					if (script.second->isTimerStarted() == false)
					{
						if (!context.player->cantTrigger() && bounds.left + bounds.width > script.second->getPosition().x && bounds.left < script.second->getPosition().x + TILE_SIZE &&
							bounds.top + bounds.height > script.second->getPosition().y + TILE_SIZE && bounds.top < script.second->getPosition().y + 2 * TILE_SIZE)
						{
							// Execute the trigger
							if (!script.second->isTriggered())
								script.second->startTimer();

							// Player is standing on the trigger
							script.second->setPlayerStandingOn(true);
						}
						else
							script.second->setPlayerStandingOn(false);
					}else{
						script.second->updateTimer(elapsedTime.asSeconds());

						if (script.second->getTimeSinceActivation() >= stof(script.second->getProperties()["time_to_execute"]))
						{
							if (!script.second->isTriggered())
								script.second->executeTrigger();

							script.second->setPlayerStandingOn(true);
							script.second->stopTimer();
						}
						else
							script.second->setPlayerStandingOn(false);
					}
				break;
			}
		}

		for(auto&& anim : animationMap)
			anim->update(elapsedTime);

		for (auto&& i : NPCs)
			i.second->updateNoPlayer(elapsedTime);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders all the layers
 * ----------------------------------------------------------------------
 */
	void GameMap::render(std::string layerName)
	{
		// Render the correct layer
		for(auto&& layer : layerList) 
			if(layer.getName() == layerName)
				layer.render();
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: Renders the animations
 * ----------------------------------------------------------------------
 */
	void GameMap::renderAnimations(std::string layerName)
	{
		for(auto&& anim : animationMap)
			if(anim->getLayer() == layerName)
				context.window->draw(anim->getSprite());
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 28 January 2014
 * Description: Returns the height of the map
 * ----------------------------------------------------------------------
 */
	int GameMap::getMapHeight() const
	{
		// Return the height of the map
		return mapHeight;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 28 January 2014
 * Description: Returns the width of the map
 * ----------------------------------------------------------------------
 */
	int GameMap::getMapWidth() const
	{
		// Return the width of the map
		return mapWidth;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 14 February 2014
 * Description: Gets a specific layer
 * ----------------------------------------------------------------------
 */
	MapLayer& GameMap::getLayer(std::string name)
	{
		for(auto&& layer : layerList)
		{
			if(layer.getName() == name)
				return layer;
		}

		return *(std::unique_ptr<MapLayer>(new MapLayer(context, "", "", 0, 0)));
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 14 February 2014
 * Description: Gets the map variable
 * ----------------------------------------------------------------------
 */
	std::unique_ptr<Tmx::Map>& GameMap::getMapObject()
	{
		return map;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 15 February 2014
 * Description: Gets the collision map
 * ----------------------------------------------------------------------
 */
	std::map<int, std::map<int, std::vector<sf::ConvexShape>>>& GameMap::getCollisionMap()
	{
		return collisionMap;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 3 March 2014
 * Description: Executes a specific trigger by the name
 * ----------------------------------------------------------------------
 */
	void GameMap::executeTrigger(std::string name)
	{
		mapTriggers[name]->executeTrigger();
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Returns if the light is natural
 * ----------------------------------------------------------------------
 */
	bool GameMap::isNaturalLighting() const
	{
		return naturalLight;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 26th May 2014
* Description: Returns the layer of NPCs in the map
* ----------------------------------------------------------------------
*/
	std::map<std::string, std::unique_ptr<Humanoid>>& GameMap::getNPCs()
	{
		return NPCs;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 26th May 2014
* Description: Renders every NPC in the map
* ----------------------------------------------------------------------
*/
	void GameMap::renderNPCs()
	{
		for (auto&& i : NPCs)
		{
			i.second->render();
		}
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 26th May 2014
* Description: Updates every NPC in the map
* ----------------------------------------------------------------------
*/
	void GameMap::updateNPCs(sf::Time elapsedTime)
	{
		for (auto&& i : NPCs)
		{
			i.second->update(elapsedTime);
		}
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 26th May 2014
* Description: Updates every NPC in the map
* ----------------------------------------------------------------------
*/
	std::vector<sf::Sprite>& GameMap::getImageLayer()
	{
		return imageLayer;
	}