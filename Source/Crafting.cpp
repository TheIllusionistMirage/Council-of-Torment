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
, buttonHighlight {{106.0f, 34.0f}}
, leftMouseButton {false}
, cPressed {false}
, isMoving {false}
, open {false}
{
	craftingWindow.setPosition(100.0f, 50.0f);
	selectedHighlight.setFillColor({150, 150, 150, 80});
	recipeHighlight.setFillColor({200, 200, 200, 100});
	buttonHighlight.setFillColor({200, 200, 200, 100});
}

void Crafting::update(const sf::Time& elapsedTime)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !cPressed)
		openCrafting(!open);

	if(open)
	{
		recipeHighlight.setPosition(-100.0f, -100.0f);
		buttonHighlight.setPosition(-100.0f, -100.0f);

		sf::Vector2f mousePos {sf::Mouse::getPosition(*context.window)};
		sf::Vector2f craftingPos {craftingWindow.getPosition()};

		// If the mouse is inside the dragable part
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.x > craftingPos.x && mousePos.x < craftingPos.x + craftingWindow.getLocalBounds().width &&
		   mousePos.y > craftingPos.y && mousePos.y < craftingPos.y + 40.0f && !leftMouseButton && !isMoving && !context.player->getInventory().isMouseInside())
		{
			isMoving = true;
			dragCoordinates.x = mousePos.x - craftingPos.x;
			dragCoordinates.y = mousePos.y - craftingPos.y;
		}

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMoving)
		{
			// Sets the position of the console
			craftingWindow.setPosition(mousePos.x - dragCoordinates.x, mousePos.y - dragCoordinates.y);

			// Adjust the position, if it goes out of screen
			if(craftingWindow.getPosition().x < 0)
				craftingWindow.setPosition(0, craftingWindow.getPosition().y);
			if(craftingWindow.getPosition().y < 0)
				craftingWindow.setPosition(craftingWindow.getPosition().x, 0);
			if(craftingWindow.getPosition().x + craftingWindow.getLocalBounds().width > context.window->getSize().x)
				craftingWindow.setPosition(context.window->getSize().x - craftingWindow.getLocalBounds().width, craftingWindow.getPosition().y);
			if(craftingWindow.getPosition().y + craftingWindow.getLocalBounds().height > context.window->getSize().y)
				craftingWindow.setPosition(craftingWindow.getPosition().x, context.window->getSize().y - craftingWindow.getLocalBounds().height);
		}

		craftingPos = {craftingWindow.getPosition()};

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
					createIngredients(selectedRecipe);
				}
			}
		}

		// If the mouse is inside the buttons
		if(mousePos.x > craftingPos.x + 424.0f && mousePos.x < craftingPos.x + 636.0f &&
		   mousePos.y > craftingPos.y + 351.0f && mousePos.y < craftingPos.y + 384.0f && !context.player->getInventory().isMouseInside())
		{
			int index {static_cast<int>((mousePos.x - craftingPos.x - 424.0f) / 106.0f)};

			buttonHighlight.setPosition(craftingPos.x + 424.0f + index * 106.0f, craftingPos.y + 351.0f);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !leftMouseButton)
			{
				if(index == 0)
					openCrafting(false);
				else if(index == 1 && selectedRecipe && selectedRecipe->complete)
				{
					context.player->getInventory().craft(ingredients, static_cast<ItemID>(std::stoi(selectedRecipe->craftedItem.back().getProperties()["id"])),
														 std::stoi(selectedRecipe->craftedItem.back().getProperties()["number"]));
				}
			}
		}

		for(size_t i {0}; i != recipes.size(); ++i)
			recipes[i].recipe.setPosition(craftingPos.x + 98.0f - static_cast<int>(recipes[i].recipe.getLocalBounds().width / 2), craftingPos.y + 75.0f + i * 20.0f);

		if(selectedRecipe)
		{
			selectedHighlight.setPosition(craftingPos.x + 10.0f, craftingPos.y + 75.0f + selectedRecipe->order * 20.0f);

			for(auto& ingredient : ingredients)
				ingredient.setPosition({craftingPos.x + 199.0f, craftingPos.y + 75.0f + ingredient.getOrder() * 20.0f});

			selectedRecipe->craftedItem.back().setPosition({craftingPos.x + 425.0f, craftingPos.y + 115.0f});
		}
	}

	leftMouseButton = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	cPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::C);

	if(!leftMouseButton)
		isMoving = false;
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
		context.window->draw(buttonHighlight);

		// Restore the camera
		context.window->setView(camera);
	}
}

void Crafting::openCrafting(bool newOpen)
{
	isMoving = false;
	open = newOpen;
	if(open)
	{
		// Check for complete recipes
		for(auto& recipe : recipes)
			checkIfComplete(recipe);
	}
}

void Crafting::addRecipe(const std::string& itemName, int id)
{
	LuaScript script {context, "Content/Scripts/items.lua"};

	std::string name {script.get<std::string>(itemName + ".name")};
	std::vector<int> ingredients {script.getIntVector(itemName + ".recipe")};
	std::string resultCount {script.get<std::string>(itemName + ".result_count")};

	recipes.push_back({name, context.contentManager->getFont(Fonts::SEGOEUI), recipes.size()});
	recipes.back().ingredients = ingredients;

	std::map<std::string, std::string> properties;
	std::stringstream stream; stream << id;

	properties["id"] = stream.str();
	properties["number"] = properties["max_number"] = resultCount;
	properties["name"] = script.get<std::string>(itemName + ".name");

	int iconID = std::stoi(script.get<std::string>(itemName + ".iconID"));
	sf::IntRect rect {(iconID % 8) * 16, int(iconID / 8) * 16, 16, 16};

	sf::Vector2f craftingPos {craftingWindow.getPosition()};
	recipes.back().craftedItem.push_back(Item(context, rect, sf::IntRect(), 0, properties));

	if(open)
		checkIfComplete(recipes.back());
}

void Crafting::createIngredients(Recipe* recipe)
{
	ingredients.clear();

	for(auto& id : recipe->ingredients)
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
			ingredient.setColor(sf::Color::White, false);
		else
			ingredient.setColor({200, 200, 200, 180}, false);
	}
}

void Crafting::checkIfComplete(Recipe& recipe)
{
	if(context.player->getInventory().checkIfRecipeComplete(recipe.ingredients))
	{
		recipe.recipe.setColor(sf::Color::White);
		recipe.craftedItem.back().setColor(sf::Color::White, true);
		recipe.complete = true;
	}
	else
	{
		recipe.recipe.setColor({200, 200, 200, 180});
		recipe.craftedItem.back().setColor({200, 200, 200, 180}, true);
		recipe.complete = false;
	}
}

void Crafting::updateRecipes()
{
	if(open)
	{
		for(auto& recipe : recipes)
			checkIfComplete(recipe);

		if(selectedRecipe)
		{
			sf::Vector2f craftingPos {craftingWindow.getPosition()};
			for(auto& ingredient : ingredients)
			{
				int id {std::stoi(ingredient.getProperties()["id"])};
				int number {std::stoi(ingredient.getProperties()["number"])};

				if(context.player->getInventory().checkIfIngredientAvailable(std::make_pair(id, number)))
					ingredient.setColor(sf::Color::White, false);
				else
					ingredient.setColor({200, 200, 200, 180}, false);
			}
		}
	}
}

bool Crafting::hasRecipe(int id)
{
	bool has {false};

	for(auto& recipe : recipes)
	{
		if(std::stoi(recipe.craftedItem.back().getProperties()["id"]) == id)
		{
			has = true;
			break;
		}
	}

	return has;
}