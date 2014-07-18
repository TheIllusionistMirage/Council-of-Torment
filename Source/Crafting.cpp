#include "Crafting.h"
#include "Item.h"
#include "LuaScript.h"
#include "Player.h"

Crafting::Crafting(State::Context context)
: context {context}
, selectedRecipe {nullptr}
, craftingWindow {context.contentManager->getTexture(Textures::CRAFTING_WINDOW)}
, selectedHighlight {{178.0f, 20.0f}}
, recipeHighlight {{178.0f, 20.0f}}
, leftMouseButton {false}
, open {false}
{
	craftingWindow.setPosition(100.0f, 50.0f);
	selectedHighlight.setFillColor({150, 150, 150, 80});
	recipeHighlight.setFillColor({200, 200, 200, 100});
}

void Crafting::update(const sf::Time& elapsedTime)
{
	if(open)
	{
		recipeHighlight.setPosition(-100.0f, -100.0f);

		sf::Vector2f mousePos {sf::Mouse::getPosition(*context.window)};
		sf::Vector2f craftingPos {craftingWindow.getPosition()};

		// If the mouse is inside the recipe table
		if(mousePos.x > craftingPos.x + 10.0f && mousePos.x < craftingPos.x + 188.0f &&
		   mousePos.y > craftingPos.y + 75.0f && mousePos.y < craftingPos.y + 75.0f + recipes.size() * 20.0f)
		{
			int index {static_cast<int>((mousePos.y - craftingPos.y - 75.0f) / 20.0f)};
			
			recipeHighlight.setPosition(craftingPos.x + 10.0f, craftingPos.y + 75.0f + index * 20.0f);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !leftMouseButton)
			{
				if(selectedRecipe && recipeHighlight.getPosition() == selectedHighlight.getPosition())
					selectedRecipe = nullptr;
				else
				{
					selectedRecipe = &recipes[index];
					selectedHighlight.setPosition(recipeHighlight.getPosition());
					createIngredients(selectedRecipe);
				}
			}
		}
	}

	leftMouseButton = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void Crafting::render()
{
	if(open)
	{
		// Save the camera and set the default view
		sf::View camera = context.window->getView();
		context.window->setView(context.window->getDefaultView());

		context.window->draw(craftingWindow);

		for(auto& recipe : recipes)
			context.window->draw(recipe.recipe);

		if(selectedRecipe)
		{
			for(auto& item : ingredients)
				item.render();

			context.window->draw(selectedHighlight);
			selectedRecipe->craftedItem.back().render();
		}

		context.window->draw(recipeHighlight);

		// Restore the camera
		context.window->setView(camera);
	}
}

void Crafting::openCrafting(bool newOpen)
{
	open = newOpen;
	if(open)
	{
		// Check for complete recipes
		for(auto& recipe : recipes)
			checkIfComplete(recipe);

		// Update the position
		sf::Vector2f craftingPos {craftingWindow.getPosition()};
		for(size_t i {0}; i != recipes.size(); ++i)
			recipes[i].recipe.setPosition(craftingPos.x + 98.0f - static_cast<int>(recipes[i].recipe.getLocalBounds().width / 2), craftingPos.y + 75.0f + i * 20.0f);
	}
}

void Crafting::addRecipe(const std::string& itemName, int id)
{
	LuaScript script {context, "Content/Scripts/items.lua"};

	std::string name {script.get<std::string>(itemName + ".name")};
	std::vector<int> ingredients {script.getIntVector(itemName + ".recipe")};
	std::string resultCount {script.get<std::string>(itemName + ".result_count")};

	recipes.push_back({name, context.contentManager->getFont(Fonts::SEGOEUI)});
	recipes.back().incredients = ingredients;

	std::map<std::string, std::string> properties;
	std::stringstream stream; stream << id;

	properties["id"] = stream.str();
	properties["number"] = properties["max_number"] = resultCount;
	properties["name"] = script.get<std::string>(itemName + ".name");

	int iconID = std::stoi(script.get<std::string>(itemName + ".iconID"));
	sf::IntRect rect {(iconID % 8) * 16, int(iconID / 8) * 16, 16, 16};

	sf::Vector2f craftingPos {craftingWindow.getPosition()};
	recipes.back().craftedItem.push_back(Item(context, rect, sf::IntRect(), 0, properties));
	recipes.back().craftedItem.back().setPosition({craftingPos.x + 424.0f, craftingPos.y + 114.0f});

	if(open)
	{
		checkIfComplete(recipes.back());
		recipes.back().recipe.setPosition(craftingPos.x + 98.0f - static_cast<int>(recipes.back().recipe.getLocalBounds().width / 2), craftingPos.y + 75.0f + recipes.size() * 20.0f);
	}
}

void Crafting::createIngredients(Recipe* recipe)
{
	ingredients.clear();

	for(auto& id : recipe->incredients)
	{
		std::map<std::string, std::string> properties;
		std::stringstream stream; stream << id;
		bool existing {false};

		for(auto& ingredient : ingredients)
		{
			if(std::stoi(ingredient.getProperties()["id"]) == id)
			{
				existing = true;
				std::stringstream stream;
				stream << std::stoi(ingredient.getProperties()["number"]) + 1;
				ingredient.getProperties()["number"] = ingredient.getProperties()["max_number"] = stream.str();
			}
		}

		if(!existing)
		{
			properties["id"] = stream.str();
			properties["number"] = properties["max_number"] = "1";

			stream.str(""); stream << "item_" << id;
			LuaScript script {context, "Content/Scripts/items.lua"};

			properties["name"] = script.get<std::string>(stream.str() + ".name");

			int iconID = std::stoi(script.get<std::string>(stream.str() + ".iconID"));
			sf::IntRect rect {(iconID % 8) * 16, int(iconID / 8) * 16, 16, 16};

			ingredients.push_back(Item(context, rect, sf::IntRect(), ingredients.size(), properties));
		}
	}

	sf::Vector2f craftingPos {craftingWindow.getPosition()};
	for(auto& ingredient : ingredients)
	{
		int id {std::stoi(ingredient.getProperties()["id"])};
		int number {std::stoi(ingredient.getProperties()["number"])};

		if(context.player->getInventory().checkIfIngredientAvailable(std::make_pair(id, number)))
			ingredient.setColor(sf::Color::White);
		else
			ingredient.setColor({200, 200, 200, 180});

		ingredient.setPosition({craftingPos.x + 199.0f, craftingPos.y + 75.0f + ingredient.getOrder() * 20.0f});
	}
}

void Crafting::checkIfComplete(Recipe& recipe)
{
	if(context.player->getInventory().checkIfRecipeComplete(recipe.incredients))
	{
		recipe.recipe.setColor(sf::Color::White);
		recipe.craftedItem.back().setColor(sf::Color::White);
		recipe.complete = true;
	}
	else
	{
		recipe.recipe.setColor({200, 200, 200, 180});
		recipe.craftedItem.back().setColor({200, 200, 200, 180});
		recipe.complete = false;
	}
}