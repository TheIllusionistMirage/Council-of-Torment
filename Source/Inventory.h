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
		void setMoving(bool newMoving) { if(open != newMoving) moving = newMoving; }
		void craft(std::vector<Item>& ingredients, ItemID id, unsigned int number = 1);

		bool checkIfRecipeComplete(const std::vector<int>& ingredients);
		void blockItems(const std::vector<int>& ingredients);
		void cancelCrafting();

	private:
		void restoreBackup();
		void changeOrder(float yPos);
		void moveInventory(sf::Time elapsedTime);
		void equipItem(std::vector<std::unique_ptr<Item>>& itemList);
		void updateOrder(std::vector<std::unique_ptr<Item>>& itemList, int order, bool add = false);

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
		std::vector<std::unique_ptr<Item>> craftingBackup;
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
		bool recipeMode;
		bool showScroll;
		bool itemMoved;
		bool itemDrag;
		bool moving;
		bool open;
};