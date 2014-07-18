#pragma once

#include "State.h"
#include <SFML/Graphics.hpp>

class Item;

struct Recipe
{
	Recipe(const std::string& name, const sf::Font& font) : recipe {name, font, 12}, complete {false} {}
	std::vector<Item> craftedItem;
	std::vector<int> incredients;
	sf::Text recipe;
	bool complete;
};

class Crafting
{
	private:
		State::Context context;
		std::vector<Recipe> recipes;
		std::vector<Item> ingredients;
		Recipe* selectedRecipe;
		sf::Sprite craftingWindow;
		sf::RectangleShape recipeHighlight;
		sf::RectangleShape selectedHighlight;
		bool leftMouseButton;
		bool open;

	private:
		void createIngredients(Recipe* recipe);

	public:
		Crafting(State::Context context);
		void update(const sf::Time& elapsedTime);
		void render();
		
		void openCrafting(bool newOpen);
		void checkIfComplete(Recipe& recipe);
		void addRecipe(const std::string& itemName, int id);
};