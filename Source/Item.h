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
	HEALING_POTION,
	HEALTH_FLASK,
	RUSTY_BLADE,
	DUSTY_TOME,
	SCROLL_OF_FIREBALL,
	RAW_SALMON,
	ITEM_ID_COUNT
};

class Item
{
	public:
		Item(State::Context context, sf::IntRect rect, sf::IntRect iconRect, int order, std::map<std::string, std::string> properties);

		void equip();
		void render();
		void update(sf::Time elapsedTime);
		void setPosition(sf::Vector2f pos);
		void setDescribed(bool newDescribed) { described = newDescribed; }
		void setOrder(int newOrder) {order = newOrder;}
		void updateDescriptionText();

		const int& getOrder() const {return order;}
		const std::string getName() const {return name.getString();}
		const sf::Vector2f& getPosition() const {return item.getPosition();}
		std::map<std::string, std::string>& getProperties() {return properties;}

		const sf::Text& getDescriptionText() { return description; }

	private:
		State::Context context;
		sf::VertexArray line;
		sf::Sprite itemIcon;
		sf::Sprite item;
		sf::Text name;
		sf::Text description;
		sf::RectangleShape equipShape;
		sf::RectangleShape itemIconShape;
		sf::RectangleShape descriptionBackground;
		std::map<std::string, std::string> properties;
		float scaleFactor;
		int order;
		int descriptionLines;
		bool increaseScale;
		bool equipped;
		bool described;
};