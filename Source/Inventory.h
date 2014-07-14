#pragma once

#include "Item.h"

class Inventory
{
	public:
		Inventory(State::Context context);

		void render();
		void updateInventoryPosition();
		void update(sf::Time elapsedTime);
		const bool& isOpen() const {return open;}
		void handleEvent(const sf::Event& windowEvent);
		void addItem(ItemID id, unsigned int number = 1);

	private:
		void changeOrder(float yPos);
		void moveInventory(sf::Time elapsedTime);
		void equipItem(std::vector<std::unique_ptr<Item>>& itemList);

	private:
		State::Context context;
		sf::Texture itemTexture;
		sf::Sprite inventory;
		sf::Sprite scrollSprite;
		sf::CircleShape categoryHighlight;
		sf::CircleShape categorySelected;
		sf::Text weightText;
		Category category;
		sf::RectangleShape itemHighlight;
		sf::RectangleShape inventoryBackground;
		std::vector<std::unique_ptr<Item>> itemList[CATEGORY_COUNT];
		sf::Vector2f oldMousePos;
		Item* dragItem;
		float clickedPosition;
		float scrollValue[CATEGORY_COUNT];
		float itemScrollSpeed;
		float speed;
		int maxWeight;
		int weight;
		bool mousePressed;
		bool rightMouseButton;
		bool showScroll;
		bool itemMoved;
		bool itemDrag;
		bool moving;
		bool open;
};