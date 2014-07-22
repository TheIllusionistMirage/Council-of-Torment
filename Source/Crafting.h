#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

class Item;

struct Recipe
{
	Recipe(const std::string& name, const sf::Font& font, size_t order) : recipe {name, font, 12}, complete {false}, order { order } {}
	std::vector<Item> craftedItem;
	std::vector<int> ingredients;
	sf::Text recipe;
	bool complete;
	size_t order;
};

class Crafting
{
	private:
		State::Context context;
		std::vector<Recipe> recipes;
		std::vector<Item> ingredients;
		Recipe* selectedRecipe;
		sf::Sprite craftingWindow;
		sf::Vector2f dragCoordinates;
		sf::RectangleShape recipeHighlight;
		sf::RectangleShape selectedHighlight;
		sf::RectangleShape buttonHighlight;
		sf::RectangleShape darkButton;
		bool leftMouseButton;
		bool isMoving;
		bool cPressed;
		bool open;

	private:
		void createIngredients(Recipe* recipe);
		void checkIfIngredientsComplete();
		void craft();

	public:
		Crafting(State::Context context);
		void update(const sf::Time& elapsedTime);
		void updateRecipes();
		void render();
		
		void increaseIncredient(int id, int number);
		void openCrafting(bool newOpen);
		void checkIfComplete(Recipe& recipe);
		void addRecipe(const std::string& itemName, int id);

		bool hasRecipe(int id);
		const bool& isOpen() const { return open; }
};