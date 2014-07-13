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
		Item(State::Context context, sf::Texture& texture, sf::IntRect rect, int order, std::map<std::string, std::string> properties);

		void render();
		void setPosition(sf::Vector2f pos);
		void setOrder(int newOrder) {order = newOrder;}
		void equip();

		const int& getOrder() const {return order;}
		const std::string getName() const {return name.getString();}
		const sf::Vector2f& getPosition() const {return item.getPosition();}
		std::map<std::string, std::string>& getProperties() {return properties;}

	private:
		State::Context context;
		sf::VertexArray line;
		sf::Sprite item;
		sf::Text name;
		sf::RectangleShape equipShape;
		std::map<std::string, std::string> properties;
		int order;
		bool equipped;
		bool described;
};