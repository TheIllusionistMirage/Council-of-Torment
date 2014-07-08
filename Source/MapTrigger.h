#pragma once

#include "State.h"
#include <functional>
#include <string>

enum class TriggerType
{
	CALLBACK,
	LEVEL_CHANGE,
	DOOR_ENTER,
	DOOR_EXIT,
	CONSOLE_PRINT,
	ANIMATION_PLAY,
	ANIMATION_LOOP,
	PLAYER_MOVE,
	TILE_CHANGE,
	CONSOLE_COMMAND,
	LIGHT_SOURCE
};

/*	This enumeration represents the types through
	which a trigger can be activated. When you
	step on it, or when you click activate, etc.
*/
enum class ExecType
{
	STEP_ON, ACTIVATING, TOGGLED, STEP_ON_DELAYED, STEP_ON_CENTER
};

/*	A map trigger is a certain event that happens in
	special circumstances. For example if a player walks
	over a special tile, it will activate a trigger.
*/
class MapTrigger
{
	public:
		typedef std::function<void()> Callback;

	public:
		MapTrigger(State::Context context, TriggerType type, bool npcOnly);

		void stopTimer();
		void startTimer();
		void setCallback(Callback callback);
		void setTriggered(bool triggered);
		void setName(std::string name);
		void setType(TriggerType type);
		void setExecType(ExecType type);
		void setPosition(sf::Vector2f position);
		void setPlayerStandingOn(bool playerStandingOn);
		void setProperties(std::map<std::string, std::string> properties);
		void setNpcOnly(bool state);
		std::map<std::string, std::string> getProperties();

		bool isPlayerStandingOn() const;
		bool isTriggered() const;
		bool isNpcOnly() const;
		float getTimeSinceActivation() const;

		sf::Vector2f getPosition() const;
		std::string getName() const;
		TriggerType getType() const;
		ExecType getExecType() const;

		void executeTrigger();
		void updateTimer(float elapsedTime);
		bool isTimerStarted();

	private:
		std::map<std::string, std::string> properties;
		State::Context context;
		Callback callback;
		sf::Vector2f position;
		std::string	name;
		TriggerType type;
		ExecType exType;


		bool playerStandingOn;
		bool startedTimer;
		bool triggered;
		bool npcOnly;
		float timeSinceActivation;

		void playMapMusic(std::string mapName);
}; 