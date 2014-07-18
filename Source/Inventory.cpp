#include "Inventory.h"
#include "Player.h"
#include "Console.h"
#include "LuaScript.h"
#include "Crafting.h"
#include <algorithm>

bool operator < (const std::unique_ptr<Item>& left, const std::unique_ptr<Item>& right)
{
	return left->getOrder() < right->getOrder();
}

Inventory::Inventory(State::Context context)
: context(context)
, inventory(context.contentManager->getTexture(Textures::INVENTORY))
, itemTexture(context.contentManager->getTexture(Textures::ITEMS))
, scrollSprite(context.contentManager->getTexture(Textures::SCROLL_UP))
, weightText("", context.contentManager->getFont(Fonts::SEGOEUI), 12)
, category(ALL)
, speed(2000.0f)
, maxWeight(200)
, weight(0)
, oldMousePos()
, describedIndex {0}
, itemScrollSpeed(200.0f)
, rightMouseButton {false}
, itemDescribed {false}
, showScroll(false)
, itemMoved(false)
, itemDrag(false)
, moving(false)
, open(false)
{
	for(auto&& scroll : scrollValue)
		scroll = 0.0f;

	categoryHighlight = sf::CircleShape(18);
	categoryHighlight.setFillColor(sf::Color(255, 255, 255, 100));
	categoryHighlight.setPosition(-100.0f, -100.0f);

	categorySelected = sf::CircleShape(18);
	categorySelected.setFillColor(sf::Color(150, 150, 150, 100));
	categorySelected.setPosition(-100.0f, -100.0f);

	itemHighlight = sf::RectangleShape(sf::Vector2f(214.0f, 20.0f));
	itemHighlight.setFillColor(sf::Color(200, 200, 200, 100));

	inventoryBackground = sf::RectangleShape(sf::Vector2f(214.0f, 463.0f));
	inventoryBackground.setFillColor(sf::Color(20, 20, 20));

	for(int i = 0; i != ITEM_ID_COUNT; ++i)
		addItem(static_cast<ItemID>(i));
}

void Inventory::update(sf::Time elapsedTime)
{
	// Update the crafting menu
	context.crafting->update(elapsedTime);

	sf::Vector2f mousePos(sf::Mouse::getPosition(*context.window));

	if(moving)
		moveInventory(elapsedTime);

	categorySelected.setPosition(inventory.getPosition().x + 25.0f + category * 41.0f, inventory.getPosition().y + 33.0f);
	inventoryBackground.setPosition(inventory.getPosition().x + 22.0f, inventory.getPosition().y + 85.0f);
	scrollSprite.setPosition(inventory.getPosition().x + 16.0f, inventory.getPosition().y);
	categoryHighlight.setPosition(-100.0f, -100.0f);
	itemHighlight.setPosition(-100.0f, -100.0f);

	std::stringstream stream;
	stream<<weight<<" / "<<maxWeight;
	weightText.setString(stream.str());
	weightText.setPosition(inventory.getPosition().x + 208.0f - float(int(weightText.getLocalBounds().width / 2.0f)), inventory.getPosition().y + 1.0f);

	for(auto&& item : itemList[category])
	{
		item->setPosition(sf::Vector2f(inventory.getPosition().x + 23.0f, inventory.getPosition().y + 86.0f + item->getOrder() * 20.0f - scrollValue[category]));
		if(itemDescribed && item->getOrder() > describedIndex)
			item->setPosition(sf::Vector2f(item->getPosition().x, item->getPosition().y + itemList[category][describedIndex]->getDescriptionText().getLocalBounds().height + 8.0f));
	}

	if(open && !moving)
	{
		for(int i = ALL; i != CATEGORY_COUNT; ++i)
		{
			float xPos =  inventory.getPosition().x + 25.0f + i * 41.0f;
			float yPos =  inventory.getPosition().y + 33.0f;

			if(mousePos.x > xPos && mousePos.x < xPos + 36.0f && mousePos.y > yPos && mousePos.y < yPos + 36.0f)
			{
				categoryHighlight.setPosition(xPos, yPos);

				if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mousePressed)
				{
					if(itemDescribed)
					{
						itemList[category][describedIndex]->setDescribed(false);
						itemDescribed = false;
					}
					category = Category(i);

					for(auto& item : itemList[category])
						item->setPosition(sf::Vector2f(inventory.getPosition().x + 23.0f, inventory.getPosition().y + 86.0f + item->getOrder() * 20.0f - scrollValue[category]));
				}
			}
		}

		float condition {mousePos.y + scrollValue[category]};
		if(itemDescribed) condition -= (itemList[category][describedIndex]->getDescriptionText().getLocalBounds().height + 8.0f);

		if(!itemDrag && mousePos.x > inventory.getPosition().x + 22.0f && mousePos.x < inventory.getPosition().x + 236.0f &&
			mousePos.y > inventory.getPosition().y + 86.0f && condition < inventory.getPosition().y + 86.0f + itemList[category].size() * 20.0f)
		{
			int order = int((mousePos.y + scrollValue[category] - inventory.getPosition().y - 86.0f) / 20.0f);
			if(itemDescribed && order >= describedIndex)
			{
				int offset = int((mousePos.y + scrollValue[category] - describedIndex * 20.0f - inventory.getPosition().y - 86.0f - (itemList[category][describedIndex]->getDescriptionText().getLocalBounds().height + 8.0f)) / 20.0f);
				offset = std::max(offset, 0);
				order = describedIndex + offset;
			}

			itemHighlight.setPosition(inventory.getPosition().x + 22.0f, inventory.getPosition().y + 85.0f + order * 20.0f - scrollValue[category]);

			if(itemDescribed && order > describedIndex)
				itemHighlight.setPosition(itemHighlight.getPosition().x, itemHighlight.getPosition().y + itemList[category][describedIndex]->getDescriptionText().getLocalBounds().height + 8.0f);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mousePressed)
			{
				if(itemDescribed)
				{
					itemList[category][describedIndex]->setDescribed(false);
					itemDescribed = false;
				}
				dragItem = itemList[category][order].get();
				clickedPosition = mousePos.y - dragItem->getPosition().y;
				itemDrag = true;
			}
			else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rightMouseButton)
			{
				if(itemDescribed)
					itemList[category][describedIndex]->setDescribed(false);

				// Show description
				if((describedIndex != order && itemDescribed) || !itemDescribed)
				{
					describedIndex = order;
					itemDescribed = true;
					itemList[category][describedIndex]->setDescribed(true);

					if(itemDescribed)
						itemList[category][describedIndex]->updateDescriptionText();

					for(auto&& item : itemList[category])
					{
						item->setPosition(sf::Vector2f(inventory.getPosition().x + 23.0f, inventory.getPosition().y + 86.0f + item->getOrder() * 20.0f - scrollValue[category]));
						if(itemDescribed && item->getOrder() > describedIndex)
							item->setPosition(sf::Vector2f(item->getPosition().x, item->getPosition().y + itemList[category][describedIndex]->getDescriptionText().getLocalBounds().height + 8.0f));
					}
				}
				else
					itemDescribed = false;
			}
		}

		if(itemDrag)
		{
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				for(auto&& item : itemList[category])
				{
					if(item.get() == dragItem)
					{
						item->setPosition(sf::Vector2f(item->getPosition().x, mousePos.y - clickedPosition));
						if(item->getPosition().y < inventory.getPosition().y + 86.0f)
						{
							item->setPosition(sf::Vector2f(item->getPosition().x, inventory.getPosition().y + 86.0f));
							scrollValue[category] -= itemScrollSpeed * elapsedTime.asSeconds();
							if(scrollValue[category] < 0.0f)
								scrollValue[category] = 0.0f;
						}
						if(item->getPosition().y + 20.0f > inventoryBackground.getPosition().y + inventoryBackground.getLocalBounds().height && inventory.getPosition().y + 85.0f +
							itemList[category].size() * 20.0f - scrollValue[category] > inventoryBackground.getPosition().y + inventoryBackground.getLocalBounds().height)
						{
							item->setPosition(sf::Vector2f(item->getPosition().x, inventoryBackground.getPosition().y + inventoryBackground.getLocalBounds().height - 20.0f));
							scrollValue[category] += itemScrollSpeed * elapsedTime.asSeconds();
							if(scrollValue[category] > itemList[category].size() * 20.0f - 20.0f)
								scrollValue[category] = itemList[category].size() * 20.0f - 20.0f;
						}
						else if(item->getPosition().y + scrollValue[category] > inventory.getPosition().y + 86.0f + itemList[category].size() * 20.0f - 20.0f)
							item->setPosition(sf::Vector2f(item->getPosition().x, inventory.getPosition().y + 86.0f + itemList[category].size() * 20.0f - 20.0f - scrollValue[category]));

						break;
					}
				}

				if(oldMousePos.y < mousePos.y)
					for(float yPos = oldMousePos.y; yPos <= mousePos.y; yPos += 10.0f)
						changeOrder(yPos);
				else
					for(float yPos = oldMousePos.y; yPos >= mousePos.y; yPos -= 10.0f)
						changeOrder(yPos);
			}
			else
			{
				if(!itemMoved && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePressed)
				{
					if(category == ALL)
						equipItem(itemList[toCategory(dragItem->getProperties()["category"])]);
					else
						equipItem(itemList[ALL]);

					itemList[category][dragItem->getOrder()]->equip();
				}

				itemDrag = false;
				itemMoved = false;
			}
		}
	}

	if(!moving && mousePos.x > scrollSprite.getPosition().x && mousePos.x < scrollSprite.getPosition().x + scrollSprite.getLocalBounds().width &&
		mousePos.y > scrollSprite.getPosition().y && mousePos.y < scrollSprite.getPosition().y + scrollSprite.getLocalBounds().height)
	{
		showScroll = true;

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mousePressed)
			moving = true;
	}
	else
		showScroll = false;

	if(itemDescribed && itemList[category][describedIndex]->getPosition().y + 20.0f <= inventory.getPosition().y + 86.0f)
	{
		itemList[category][describedIndex]->setDescribed(false);
		itemDescribed = false;
	}

	if(itemDescribed)
		itemList[category][describedIndex]->update(elapsedTime);

	mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	rightMouseButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	oldMousePos = mousePos;
}

void Inventory::render()
{
	// Save the camera and set the default view
	sf::View camera = context.window->getView();
	context.window->setView(context.window->getDefaultView());

	// Render the crafting menu
	context.crafting->render();

	// Render the inventory
	context.window->draw(inventoryBackground);

	for(auto&& item : itemList[category])
		if(item->getPosition().y + 20.0f > inventory.getPosition().y + 86.0f && item->getPosition().y < inventory.getPosition().y + inventory.getGlobalBounds().height)
			item->render();

	context.window->draw(itemHighlight);
	context.window->draw(inventory);
	context.window->draw(categorySelected);
	context.window->draw(categoryHighlight);
	context.window->draw(weightText);
	
	if(showScroll)
		context.window->draw(scrollSprite);

	// Restore the camera
	context.window->setView(camera);
}

const bool Inventory::isMouseInside() const
{
	sf::FloatRect rect {inventory.getPosition().x + 10.0f, inventory.getPosition().y, inventory.getLocalBounds().width, inventory.getLocalBounds().height};
	return rect.contains(sf::Vector2f(sf::Mouse::getPosition(*context.window)));
}

void Inventory::updateInventoryPosition()
{
	// Set the position of the inventory
	inventory.setPosition(context.window->getSize().x - inventory.getLocalBounds().width, context.window->getSize().y - 24.0f);
	moving = false;
	open = false;
}

void Inventory::handleEvent(const sf::Event& windowEvent)
{
	if(windowEvent.type == sf::Event::KeyPressed)
	{
		// Open / close the inventory
		if(windowEvent.key.code == sf::Keyboard::I && !moving)
			moving = true;
	}
	else if(windowEvent.type == sf::Event::MouseWheelMoved)
	{
		if(open && isMouseInside() && !itemList[category].empty())
		{
			if(windowEvent.mouseWheel.delta > 0)
				scrollValue[category] -= 10.0f;
			else if(windowEvent.mouseWheel.delta < 0)
				scrollValue[category] += 10.0f;

			if(scrollValue[category] < 0.0f)
				scrollValue[category] = 0.0f;
			if(scrollValue[category] > itemList[category].size() * 20.0f - 20.0f)
				scrollValue[category] = itemList[category].size() * 20.0f - 20.0f;
		}
	}
}

void Inventory::moveInventory(sf::Time elapsedTime)
{
	if(open)
	{
		if(inventory.getPosition().y + 24.0f < context.window->getSize().y)
			inventory.move(0, speed * elapsedTime.asSeconds());
		if(inventory.getPosition().y + 24.0f > context.window->getSize().y)
		{
			inventory.setPosition(inventory.getPosition().x, context.window->getSize().y - 24.0f);
			scrollSprite.setTexture(context.contentManager->getTexture(Textures::SCROLL_UP));
			moving = false;
			open = !open;
		}
	}
	else
	{
		if(inventory.getPosition().y + inventory.getLocalBounds().height > context.window->getSize().y)
			inventory.move(0, -speed * elapsedTime.asSeconds());
		if(inventory.getPosition().y + inventory.getLocalBounds().height < context.window->getSize().y)
		{
			inventory.setPosition(inventory.getPosition().x, context.window->getSize().y - inventory.getLocalBounds().height);
			scrollSprite.setTexture(context.contentManager->getTexture(Textures::SCROLL_DOWN));
			moving = false;
			open = !open;
		}
	}
}

void Inventory::equipItem(std::vector<std::unique_ptr<Item>>& itemList)
{
	for(auto&& item : itemList)
	{
		if(item->getProperties() == dragItem->getProperties())
		{
			item->equip();
			break;
		}
	}
}

void Inventory::changeOrder(float yPos)
{
	if(yPos - clickedPosition < inventory.getPosition().y + 85.0f)
		yPos = inventory.getPosition().y + 85.0f + clickedPosition;
	if(yPos - clickedPosition + 20.0f > inventoryBackground.getPosition().y + inventoryBackground.getLocalBounds().height)
		yPos = inventoryBackground.getPosition().y + inventoryBackground.getLocalBounds().height - 20.0f + clickedPosition;

	int order = int((yPos - clickedPosition - inventory.getPosition().y - 86.0f + 10.5f + scrollValue[category]) / 20.0f);
	if(order >= int(itemList[category].size())) order = itemList[category].size() - 1;
	if(order < 0) order = 0;

	if(dragItem->getOrder() != order)
	{
		int tempOrder = dragItem->getOrder();
		dragItem->setOrder(order);
		itemList[category][order]->setOrder(tempOrder);
		std::sort(itemList[category].begin(), itemList[category].end());
		itemMoved = true;
	}
}

bool Inventory::checkIfIngredientAvailable(std::pair<int, int> ingredient)
{
	bool available {false};

	for(auto& item : itemList[ALL])
	{
		if(std::stoi(item->getProperties()["id"]) == ingredient.first)
		{
			ingredient.second -= std::stoi(item->getProperties()["number"]);

			if(ingredient.second <= 0)
			{
				available = true;
				break;
			}
		}
	}

	return available;
}

bool Inventory::checkIfRecipeComplete(const std::vector<int>& ingredients)
{
	bool complete {true};
	
	std::vector<bool> available(ingredients.size());
	for(auto& i : available)
		i = false;

	// Create temp properties, to be able to change values without restoring them
	std::vector<std::map<std::string, std::string>> tempProperties;
	for(auto& item : itemList[ALL])
		tempProperties.push_back(item->getProperties());

	for(size_t i {0}; i != ingredients.size(); ++i)
	{
		for(auto& item : tempProperties)
		{
			if(std::stoi(item["id"]) == ingredients[i] && std::stoi(item["number"]) != 0)
			{
				available[i] = true;
				std::stringstream stream;
				stream << std::stoi(item["number"]) - 1;
				item["number"] = stream.str();
				break;
			}
		}
	}

	for(auto& i : available)
		if(!i) complete = false;

	return complete;
}

void Inventory::craft(std::vector<Item> ingredients, ItemID id, unsigned int number)
{
	// Remove the ingredients from the inventory
	for(auto& ingredient : ingredients)
	{
		int ingredientAmount {std::stoi(ingredient.getProperties()["number"])};
		while(ingredientAmount > 0)
		{
			for(auto iter = std::begin(itemList[ALL]); iter != std::end(itemList[ALL]);)
			{
				//std::cout << (*iter)->getProperties()["category"] << std::endl;
				if(std::stoi((*iter)->getProperties()["id"]) == std::stoi(ingredient.getProperties()["id"]))
				{
					int amount = std::stoi((*iter)->getProperties()["number"]);
					float itemWeight = std::stof((*iter)->getProperties()["weight"]);

					if(amount - ingredientAmount <= 0)
					{
						for(auto iter2 = std::begin(itemList[toCategory((*iter)->getProperties()["category"])]); iter2 != std::end(itemList[toCategory((*iter)->getProperties()["category"])]);)
						{
							if(std::stoi((*iter2)->getProperties()["id"]) == std::stoi(ingredient.getProperties()["id"]))
							{
								updateOrder(itemList[toCategory((*iter)->getProperties()["category"])], (*iter2)->getOrder());
								iter2 = itemList[toCategory((*iter)->getProperties()["category"])].erase(iter2);
								break;
							}
							else
								++iter2;
						}

						updateOrder(itemList[ALL], (*iter)->getOrder());
						iter = itemList[ALL].erase(iter);
					}
					else
					{
						std::stringstream stream;
						stream << std::stoi((*iter)->getProperties()["number"]) - ingredientAmount;
						(*iter)->getProperties()["number"] = stream.str();
						for(auto iter2 = std::begin(itemList[toCategory((*iter)->getProperties()["category"])]); iter2 != std::end(itemList[toCategory((*iter)->getProperties()["category"])]);)
						{
							if(std::stoi((*iter2)->getProperties()["id"]) == std::stoi(ingredient.getProperties()["id"]))
							{
								(*iter2)->getProperties()["number"] = stream.str();
								break;
							}
							else
								++iter2;
						}
						++iter;
					}

					// Decrease the ammount and weight
					ingredientAmount -= amount;
					weight -= itemWeight * amount;

					if(ingredientAmount <= 0)
						break;
				}
				else
					++iter;
			}
		}
	}

	// Add the crafted item to the inventory
	addItem(id, number);
}

void Inventory::updateOrder(std::vector<std::unique_ptr<Item>>& itemList, int order)
{
	for(auto& item : itemList)
		if(item->getOrder() > order)
			item->setOrder(item->getOrder() - 1);
}

void Inventory::addItem(ItemID id, unsigned int number)
{
	std::map<std::string, std::string> properties;
	std::stringstream stream; stream << id;

	for(auto& item : itemList[ALL])
	{
		if(std::stoi(item->getProperties()["id"]) == id)
		{
			// An item stack can be filled until it is full
			int difference {std::stoi(item->getProperties()["max_number"]) - std::stoi(item->getProperties()["number"])};
			if(int(number) > difference)
			{
				number -= difference;
				item->getProperties()["number"] = item->getProperties()["max_number"];

				for(auto& item : itemList[toCategory(item->getProperties()["category"])])
					if(std::stoi(item->getProperties()["id"]) == id && int(number) > std::stoi(item->getProperties()["max_number"]) - std::stoi(item->getProperties()["number"]))
						item->getProperties()["number"] = item->getProperties()["max_number"];

				float itemWeight {std::stof(item->getProperties()["weight"])};
				weight += itemWeight * difference;
			}
			else	// Or just filled up
			{
				std::stringstream stream;
				stream << std::stoi(item->getProperties()["number"]) + number;
				item->getProperties()["number"] = stream.str();

				for(auto& item : itemList[toCategory(item->getProperties()["category"])])
					if(std::stoi(item->getProperties()["id"]) == id && int(number) <= std::stoi(item->getProperties()["max_number"]) - std::stoi(item->getProperties()["number"]))
						item->getProperties()["number"] = stream.str();

				float itemWeight {std::stof(item->getProperties()["weight"])};
				weight += itemWeight * number;
				number = 0;
			}
		}
	}

	if(number)
	{
		// Properties which are fix
		properties["id"] = stream.str();					// ItemID
		stream.str(""); stream << number;
		properties["number"] = stream.str();				// Number

		stream.str(""); stream << "item_" << id;
		LuaScript script {context, "Content/Scripts/items.lua"};

		// Variable properties
		properties["category"] = script.get<std::string>(stream.str() + ".category");
		properties["name"] = script.get<std::string>(stream.str() + ".name");
		properties["max_number"] = script.get<std::string>(stream.str() + ".max_number");
		properties["weight"] = script.get<std::string>(stream.str() + ".weight");
		properties["value"] = script.get<std::string>(stream.str() + ".value");
		properties["equipable"] = script.get<std::string>(stream.str() + ".equipable");
		properties["craftable"] = script.get<std::string>(stream.str() + ".craftable");
		properties["consumable"] = script.get<std::string>(stream.str() + ".consumable");
		properties["description"] = script.get<std::string>(stream.str() + ".description");
		properties["can_sell"] = script.get<std::string>(stream.str() + ".description");
		properties["can_drop"] = script.get<std::string>(stream.str() + ".description");

		if(!context.crafting->hasRecipe(std::stoi(properties["id"])) && properties["craftable"] == "True")
			context.crafting->addRecipe(stream.str(), id);

		int iconID = std::stoi(script.get<std::string>(stream.str() + ".iconID"));
		sf::IntRect rect {(iconID % 8) * 16, int(iconID / 8) * 16, 16, 16};
		sf::IntRect iconRect {(id % 8) * 32, int(id / 8) * 32, 32, 32};

		if(int(number) > std::stoi(properties["max_number"]))
			properties["number"] = properties["max_number"];

		float itemWeight {std::stof(properties["weight"])};

		std::unique_ptr<Item> item(new Item(context, rect, iconRect, itemList[toCategory(properties["category"])].size(), properties));
		std::unique_ptr<Item> allItem(new Item(context, rect, iconRect, itemList[ALL].size(), properties));
		itemList[toCategory(properties["category"])].push_back(std::move(item));
		itemList[ALL].push_back(std::move(allItem));

		if(int(number) > std::stoi(properties["max_number"]))
		{
			number -= std::stoi(properties["max_number"]);
			addItem(id, number);
		}

		weight += itemWeight * number;
	}

	if(weight > maxWeight)
		context.console->logWarning("Inventory is full!");

	itemList[category][describedIndex]->updateDescriptionText();
	context.crafting->updateRecipes();
}