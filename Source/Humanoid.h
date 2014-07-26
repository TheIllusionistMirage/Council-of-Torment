#pragma once

#include "DialogueBox.h"
#include "NpcEntity.h"
#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <map>

enum HumanAnimationID
{
	IDLE_NORMAL,
	IDLE_HOLDINGITEM,
	IDLE_HOLDINGSPEAR,
	IDLE_HOLDINGCRATE,
	SITTING,
	WALK_NORMAL,
	WALK_HOLDINGITEM,
	WALK_HOLDINGSPEAR,
	WALK_HOLDINGCRATE,
	DEATH,
	CRAFTING,
	IDLE_BACKWARD,
	BOW_IDLE,
	TOTAL_ANIMATIONS
};

/*	The humanoid is the base class for the player and
	the class for all NPC's. It contains various data
	and functions about what a humanoid needs.
	It is very powerful in a container full of NPC's.
*/
	class Humanoid
	{
		public:
			Humanoid(State::Context context, std::string entName, float xPos = 0.0f, float yPos = 0.0f);

			virtual void move(int direction, sf::Time elapsedTime);
			virtual void updateNoPlayer(sf::Time elapsedTime);
			virtual void playAnimationOnce(int HumanAnimationID);
			virtual void move(int direction, int squares);
			virtual void update(sf::Time elapsedTime);
			virtual void drawOverheadText();
			virtual void unMove();
			virtual void render();

			/*---------- Setters ------------------*/

			virtual void setProperty(std::string key, bool value);
			virtual void setPosition(float tileX, float tileY);
			virtual void setDialogueFile(std::string filename);
			virtual void setPosition(sf::Vector2f position);
			virtual void sayLine(std::string lineID);
			virtual void setDialogueMode(bool state);
			virtual void setDefaultSpeed(int speed);
			virtual void attack(Humanoid& target);
			virtual void setIdleAnimation(int id);
			virtual void setName(std::string n);
			virtual void setCanMove(bool flag);
			virtual void setHealth(int health);
			virtual void setSpeed(int speed);
			virtual void toggleDefendMode();
			virtual void setMana(int mana);

			/*---------- Getters ------------------*/

			virtual sf::Vector2f getRelativePosition();
			virtual sf::FloatRect getCollisionBounds();
			virtual bool getProperty(std::string key);
			virtual std::string getDialogueFile();
			virtual sf::IntRect getTextureRext();
			virtual sf::Vector2f getPosition();
			virtual int getDefaultVelocity();
			virtual sf::Sprite& getSprite();
			DialogueBox& getOverheadText();
			virtual std::string getName();
			virtual bool cantTrigger();
			virtual int getVelocity();

			/*---------- Public Variables ---------*/
			// the key that points to this humanoid in the gameMap
			std::string key; 

		protected:
			DialogueBox overheadText;
			State::Context context;
			sf::Sprite bodySprite;
			sf::Texture bodyTexture;

			std::map<std::string, bool> properties;
			sf::FloatRect collisionBounds;
			sf::Vector2f targetPos;
			std::string name;
			std::string dialogueFile;
			std::string currentLine;

			float defaultFrameTime;
			float timer;
			float poisonTimer;
			bool isMoving;
			bool playOnce;
			bool noTrigger;
			bool dialogueMode;
			bool defendMode;
			int velocity;
			int health;
			int mana;
			int defaultVelocity;
			unsigned int frameX;
			unsigned int frameY;
			unsigned int maximumHealth;
			unsigned int maximumMana;
			unsigned int directionFacing;
			unsigned int currentAnimation;
			unsigned int walkAnimation;
			unsigned int idleAnimation;
			unsigned int id;
	};