#pragma once

#include "State.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <map>

enum Direction
{
	NO_DIRECTION,
	NORTH,
	SOUTH,
	WEST,
	EAST,
};

enum AnimationID
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

		virtual void update(sf::Time elapsedTime);
		virtual void move(int direction, int squares);
		virtual void move(int direction, sf::Time elapsedTime);
		virtual void unMove() { if(!noTrigger) isMoving = false; }
		virtual void render();

		virtual void playAnimationOnce(int animationID);
		virtual void setIdleAnimation(int id);
		virtual void setPosition(float tileX, float tileY);
		virtual void setPosition(sf::Vector2f position);
		virtual void setHealth(int health);
		virtual void setDefaultSpeed(int speed);
		virtual void setName(std::string n);
		virtual void setMana(int mana);
		virtual void setSpeed(int speed);
		virtual void setCanMove(bool flag);
		virtual void setProperty(std::string key, bool value);

		virtual bool cantTrigger() const { return noTrigger; }
		virtual std::string getName();
		virtual sf::Vector2f getPosition() const;
		virtual sf::Vector2f getRelativePosition() const;
		virtual sf::IntRect getTextureRext() const;
		virtual sf::FloatRect getCollisionBounds() const { return collisionBounds; }
		virtual bool getProperty(std::string key);

	protected:
		State::Context context;
		sf::Sprite bodySprite;
		sf::Texture bodyTexture;

		std::map<std::string, bool> properties;
		sf::FloatRect collisionBounds;
		sf::Vector2f targetPos;
		std::string name;

		float defaultFrameTime;
		float timer;
		float poisonTimer;
		bool isMoving;
		bool playOnce;
		bool noTrigger;
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