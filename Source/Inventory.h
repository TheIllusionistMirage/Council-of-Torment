#pragma once

#include "Item.h"

class Inventory
{
	public:
		Inventory(State::Context context);

		void render();
		void updateInventoryPosition();
		void update(sf::Time elapsedTime);
		const bool isMouseInside() const;
		const bool& isOpen() const {return open;}
		void handleEvent(const sf::Event& windowEvent);
		void addItem(ItemID id, unsigned int number = 1);
		void craft(std::vector<Item> ingredients, ItemID id, unsigned int number = 1);

		bool checkIfIngredientAvailable(std::pair<int, int> ingredient);
		bool checkIfRecipeComplete(const std::vector<int>& ingredients);

	private:
		void changeOrder(float yPos);
		void moveInventory(sf::Time elapsedTime);
		void equipItem(std::vector<std::unique_ptr<Item>>& itemList);
		void updateOrder(std::vector<std::unique_ptr<Item>>& itemList, int order);

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
		float maxWeight;
		float weight;
		int describedIndex;
		bool mousePressed;
		bool itemDescribed;
		bool rightMouseButton;
		bool showScroll;
		bool itemMoved;
		bool itemDrag;
		bool moving;
		bool open;
};