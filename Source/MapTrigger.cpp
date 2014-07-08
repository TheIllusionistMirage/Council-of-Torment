#include "MapTrigger.h"
#include "Stdafx.h"
#include "Game.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Constructor of the map trigger class
 * ----------------------------------------------------------------------
 */
	MapTrigger::MapTrigger(State::Context context, TriggerType type, bool npcOnly)
	: callback()
	, context(context)
	, npcOnly(npcOnly)
	, type(type)
	, playerStandingOn(false)
	, triggered(false)
	, startedTimer(false)
	, position(0, 0)
	, properties()
	, name("")
	, timeSinceActivation(0.0)
	{
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Executes the trigger based on the type
 * ----------------------------------------------------------------------
 */
	void MapTrigger::executeTrigger() 
	{
		// Set the bool to true
		triggered = true;
		startedTimer = true;

		// Switch between the trigger types
		switch(type)
		{
			// Call the callback function if possible
			case(TriggerType::CALLBACK):
				if(callback)
					callback();
				break;

			case(TriggerType::CONSOLE_PRINT):
				context.console->logInfo(properties["message"]);
				break;

			case(TriggerType::LEVEL_CHANGE):
				context.player->setDestinationLevelExit(properties["coresponding"]);

				// Play the music for the map
				playMapMusic(properties["map"]);

				context.effectManager->add(Effect(EffectType::FADE));
				context.effectManager->getEffectList().back().setCallback([this]()
				{
					if(properties["direction"] == "north")
						context.gameMap->loadMap(properties["map"] + ".tmx", NORTH);
					else if(properties["direction"] == "south")
						context.gameMap->loadMap(properties["map"] + ".tmx", SOUTH);
					else if(properties["direction"] == "east")
						context.gameMap->loadMap(properties["map"] + ".tmx", EAST);
					else if(properties["direction"] == "west")
						context.gameMap->loadMap(properties["map"] + ".tmx", WEST);
				});
				break;

			case(TriggerType::ANIMATION_PLAY):
				context.player->playAnimationOnce(std::stoi(properties["id"]));
				break;

			case(TriggerType::ANIMATION_LOOP):
				//context.player->setIdleAnimation(std::stoi(properties["id"]));
				break;

			case(TriggerType::PLAYER_MOVE):
				if(properties["direction"] == "north")
					context.player->move(NORTH, std::stoi(properties["squares"]));
				else if(properties["direction"] == "south")
					context.player->move(SOUTH, std::stoi(properties["squares"]));
				else if(properties["direction"] == "east")
					context.player->move(EAST, std::stoi(properties["squares"]));
				else if(properties["direction"] == "west")
					context.player->move(WEST, std::stoi(properties["squares"]));
				break;

			case(TriggerType::TILE_CHANGE):
				context.gameMap->getLayer(properties["layer"]).getTileList()[unsigned int(position.y/32+1)][unsigned int(position.x/32)].id = std::stoi(properties["id"]);
				break;

			case(TriggerType::CONSOLE_COMMAND):
				if(properties["hasParam"] == "True")
				{
					context.console -> setEntryText(properties["cmd"] + " " + properties["param"]);
					context.console -> submitCommand();
				}
				else if(properties["hasParam"] == "False")
				{
					context.console -> setEntryText(properties["cmd"]);
					context.console -> submitCommand();
				}
				break;
		}

		if (properties.find("execute") != properties.end()) 
		{
			context.gameMap->executeTrigger(properties["execute"]); 
		}
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 30 January 2014
 * Description: Plays music for the map
 * ----------------------------------------------------------------------
 */
	void MapTrigger::playMapMusic(std::string mapName)
	{
		// Play a music depending on the map
		if(mapName == "gideon_tower")
			context.contentManager->playMusic(Music::GIDEON_TOWER);
		else if(mapName == "gideon_tower_outside")
			context.contentManager->playMusic(Music::TOWN_SQUARE);
		else
			context.contentManager->playMusic(Music::NO_MUSIC);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Sets another callback function for the map trigger
 * ----------------------------------------------------------------------
 */
	void MapTrigger::setCallback(Callback callback)
	{
		// Set the new callback function
		this->callback = std::move(callback);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 29 January 2014
 * Description: Sets another value for the playerStandingOn bool
 * ----------------------------------------------------------------------
 */
	void MapTrigger::setPlayerStandingOn(bool playerStandingOn)
	{
		// Set another value for the bool
		this->playerStandingOn = playerStandingOn;

		// Let the trigger be triggerable again
		if(!playerStandingOn && triggered)
			triggered = !triggered;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Sets the properties
 * ----------------------------------------------------------------------
 */
	void MapTrigger::setProperties(std::map<std::string, std::string> properties)
	{
		// Set the properties
		this->properties = properties;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Sets the triggered flag
 * ----------------------------------------------------------------------
 */
	void MapTrigger::setTriggered(bool triggered)
	{
		// Set the new flag
		this->triggered = triggered;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Sets another name
 * ----------------------------------------------------------------------
 */
	void MapTrigger::setName(std::string name)
	{
		// Set another name
		this->name = name;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Sets another type
 * ----------------------------------------------------------------------
 */
	void MapTrigger::setType(TriggerType type)
	{
		// Set another type
		this->type = type;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Sets another position
 * ----------------------------------------------------------------------
 */
	void MapTrigger::setPosition(sf::Vector2f position)
	{
		// Set another position
		this->position = position;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Returns true if it is triggered
 * ----------------------------------------------------------------------
 */
	bool MapTrigger::isTriggered() const
	{
		// Return true if it is triggered
		return triggered;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 29 January 2014
 * Description: Returns true if the player stands on the trigger
 * ----------------------------------------------------------------------
 */
	bool MapTrigger::isPlayerStandingOn() const
	{
		// Return true if the player stands on the trigger
		return playerStandingOn;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Returns true if it is for npc only
 * ----------------------------------------------------------------------
 */
	bool MapTrigger::isNpcOnly() const
	{
		// Return true if it is for npc only
		return npcOnly;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Returns the position
 * ----------------------------------------------------------------------
 */
	sf::Vector2f MapTrigger::getPosition() const
	{
		// Return the position
		return position;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Returns the name
 * ----------------------------------------------------------------------
 */
	std::string MapTrigger::getName() const
	{
		// Return the name
		return name;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 22 January 2014
 * Description: Returns the type
 * ----------------------------------------------------------------------
 */
	TriggerType MapTrigger::getType() const
	{
		// Return the type
		return type;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 19 May 2014
* Description: Sets the execution type
* ----------------------------------------------------------------------
*/
	void MapTrigger::setExecType(ExecType type)
	{
		exType = type;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 19 May 2014
* Description: Returns the execution type
* ----------------------------------------------------------------------
*/
	ExecType MapTrigger::getExecType() const
	{
		return exType;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 19 May 2014
* Description: Sets if the trigger can be activated by NPCs only
* ----------------------------------------------------------------------
*/
	void MapTrigger::setNpcOnly(bool state)
	{
		npcOnly = state;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 20 May 2014
* Description: Updates the trigger's timer.
* ----------------------------------------------------------------------
*/
	void MapTrigger::updateTimer(float elapsedTime)
	{
		if (startedTimer)
			timeSinceActivation += elapsedTime;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 20 May 2014
* Description: returns the time since activation
* ----------------------------------------------------------------------
*/
	float MapTrigger::getTimeSinceActivation() const
	{
		return timeSinceActivation;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 20 May 2014
* Description: returns the map of properties.
* ----------------------------------------------------------------------
*/
	std::map<std::string, std::string> MapTrigger::getProperties()
	{
		return properties;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 20 May 2014
* Description: Stops the timer completely
* ----------------------------------------------------------------------
*/
	void MapTrigger::stopTimer()
	{
		timeSinceActivation = 0.0;
		startedTimer = false;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 21 May 2014
* Description: Stops the timer completely
* ----------------------------------------------------------------------
*/
	void MapTrigger::startTimer()
	{
		startedTimer = true;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 21 May 2014
* Description: returns true if the timer is started
* ----------------------------------------------------------------------
*/
	bool MapTrigger::isTimerStarted()
	{
		return startedTimer;
	}
