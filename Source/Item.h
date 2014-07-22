#pragma once

#include "State.h"
#include <SFML/Graphics/RectangleShape.hpp>

enum Category
{
	ALL,
	FOOD,
	EQUIPMENT,
	BOOKS,
	MATERIALS,
	CATEGORY_COUNT
};

enum ItemID
{
	HEALTH_POTION,
	MANA_POTION,
	HEALTH_FLASK,
	MANA_FLASK,
	RUSTY_BLADE,
	DUSTY_TOME,
	SCROLL_OF_FIREBALL,
	RAW_SALMON,
	HANDFUL_MUSHROOMS,
	IRON_INGOT,
	STEEL_INGOT,
	SILVER_INGOT,
	GOLD_INGOT,
	HARDIUM_INGOT,
	IRON_ORE,
	COAL,
	SILVER_ORE,
	GOLD_ORE,
	HARDIUM_ORE,
	ITEM_ID_COUNT
};

class Item
{
	public:
		Item(State::Context context, sf::IntRect rect, sf::IntRect iconRect, int order, std::map<std::string, std::string> properties, bool ingredient = false);

		void equip();
		void render();
		void update(sf::Time elapsedTime);
		void setPosition(sf::Vector2f pos);
		void setDescribed(bool newDescribed) { described = newDescribed; }
		void showLine(bool show) { lines = show; }
		void setOrder(int newOrder) {order = newOrder;}
		void setColor(const sf::Color& newColor);
		void setBlocked(bool newBlocked) { blocked = newBlocked; }
		void setInsertedNumber(int number) { insertedNumber = number; }
		void updateDescriptionText();

		const int& getInsertedNumber() const { return insertedNumber; }
		const int& getOrder() const {return order;}
		const std::string getName() const {return name.getString();}
		const sf::Vector2f& getPosition() const {return item.getPosition();}
		std::map<std::string, std::string>& getProperties() {return properties;}

		const bool& isBlocked() const { return blocked; }
		const sf::Text& getDescriptionText() { return description; }

	private:
		State::Context context;
		sf::VertexArray line;
		sf::Sprite itemIcon;
		sf::Sprite item;
		sf::Text name;
		sf::Color color;
		sf::Text description;
		sf::RectangleShape equipShape;
		sf::RectangleShape itemIconShape;
		sf::RectangleShape blockedShape;
		sf::RectangleShape descriptionBackground;
		std::map<std::string, std::string> properties;
		float scaleFactor;
		int insertedNumber;
		int order;
		bool blocked;
		bool lines;
		bool increaseScale;
		bool equipped;
		bool ingredient;
		bool described;
};