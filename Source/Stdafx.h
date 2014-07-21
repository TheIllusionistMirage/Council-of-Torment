#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <sstream>
#include <map>

// A constant for the first loaded map in the game
#define DEFAULT_LOADED_MAP "gideon_tower.tmx"

// Just some constants to not get confused
#define BRIGHT_CHECKBOX		20
#define DARK_CHECKBOX		0
#define TILES_PER_ROW		8
#define TILE_SIZE			32

int toCategory(std::string string);

// Some colors for the logging
#define error_color		sf::Color(221, 61, 6)
#define success_color	sf::Color(110, 201, 28)
#define warning_color	sf::Color(225, 174, 0)

#define morning_color	sf::Color(220, 150, 120, 180)
#define dawn_color		sf::Color(220, 200, 180, 220)
#define noon_color		sf::Color(255, 255, 255, 255)
#define afternoon_color	sf::Color(190, 190, 200, 220)
#define dusk_color		sf::Color(150, 160, 210, 120)
#define night_color		sf::Color(255, 255, 255, 20)
#define collision_debug_color sf::Color::Green

namespace States
{
	enum ID
	{
		NONE,
		TITLE,
		MENU,
		SETTINGS,
		DEFOCUS,
		GAME,
		PAUSE
	};
}

namespace EffectTrigger
{
	enum ID
	{
		NO_TRIGGER,
		START_FADE
	};
}

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);

void centerOrigin(sf::CircleShape& circleShape);
void centerOrigin(sf::RectangleShape& rectangleShape);
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

bool isOnScreen(const sf::Sprite& sprite, const sf::View& camera);
float randomFloat(float min, float max);
int randomInteger(int min, int max);

float distance(const sf::Vector2f first, const sf::Vector2f second);
float dotProduct(const sf::Vector2f& vector1, const sf::Vector2f& vector2);
float magnitude(const sf::Vector2f& vector);
sf::Vector2f normalize(const sf::Vector2f&vector);
std::string wrap(std::string text, size_t line_length = 35);