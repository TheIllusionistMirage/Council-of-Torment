#include "Inventory.h"
#include "Player.h"
#include "Console.h"
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
, itemScrollSpeed(200.0f)
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

	addItem(HEALING_POTION);
	addItem(HEALTH_FLASK);
	addItem(DUSTY_TOME);
	addItem(SCROLL_OF_FIREBALL);
	addItem(RAW_SALMON);
	addItem(RUSTY_BLADE);
}

void Inventory::update(sf::Time elapsedTime)
{
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
		item->setPosition(sf::Vector2f(inventory.getPosition().x + 23.0f, inventory.getPosition().y + 86.0f + item->getOrder() * 20.0f - scrollValue[category]));

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
					category = Category(i);

				for(auto&& item : itemList[category])
					item->setPosition(sf::Vector2f(inventory.getPosition().x + 23.0f, inventory.getPosition().y + 86.0f + item->getOrder() * 20.0f - scrollValue[category]));
			}
		}

		if(!itemDrag && mousePos.x > inventory.getPosition().x + 22.0f && mousePos.x < inventory.getPosition().x + 236.0f &&
			mousePos.y > inventory.getPosition().y + 86.0f && mousePos.y + scrollValue[category] < inventory.getPosition().y + 86.0f + itemList[category].size() * 20.0f)
		{
			int order = int((mousePos.y + scrollValue[category] - inventory.getPosition().y - 86.0f) / 20.0f);
			itemHighlight.setPosition(inventory.getPosition().x + 22.0f, inventory.getPosition().y + 85.0f + order * 20.0f - scrollValue[category]);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mousePressed)
			{
				dragItem = itemList[category][order].get();
				clickedPosition = mousePos.y - dragItem->getPosition().y;
				itemDrag = true;
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

	mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	oldMousePos = mousePos;
}

void Inventory::render()
{
	// Save the camera and set the default view
	sf::View camera = context.window->getView();
	context.window->setView(context.window->getDefaultView());

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
		if(open)
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

void Inventory::addItem(ItemID id, unsigned int number)
{
	std::map<std::string, std::string> properties;
	std::stringstream stream; stream<<id;
	bool unknownID = false;

	// Properties which are fix
	properties["id"] = stream.str();					// ItemID
	stream.str(""); stream<<number;
	properties["number"] = stream.str();				// Number

	// Variable properties
	properties["category"] = "FOOD";					// Category		FOOD
	properties["name"] = "NoName";						// Name			NoName
	properties["max_number"] = "1";						// MaxNumber	1
	properties["weight"] = "0.0f";						// Weight		0.0f
	properties["value"] = "0.0f";						// Value		0.0f
	properties["equipable"] = "False";					// Equipable	false
	properties["consumable"] = "False";					// Consumable	false
	sf::IntRect rect;									// CHANGE!!!

	switch(id)
	{
		case(HEALING_POTION):
			properties["name"] = "Potion of Healing";
			rect = sf::IntRect(0, 0, 16, 16);
			break;
		case(HEALTH_FLASK):
			
			properties["name"] = "Flask of Health";
			rect = sf::IntRect(16, 0, 16, 16);
			break;
		case(RUSTY_BLADE):
		{
			properties["category"] = "EQUIPMENT";
			properties["equipable"] = "True";
			properties["name"] = "Rusty Blade";
			rect = sf::IntRect(0, 48, 16, 16);
		} break;
		case(DUSTY_TOME):
		{
			properties["category"] = "BOOKS";
			properties["equipable"] = "True";
			properties["name"] = "Dusty Tome";
			rect = sf::IntRect(0, 16, 16, 16);
		} break;
		case(SCROLL_OF_FIREBALL):
		{
			properties["category"] = "BOOKS";
			properties["equipable"] = "True";
			properties["name"] = "Scroll of Fireball";
			rect = sf::IntRect(16, 16, 16, 16);
		} break;
		case(RAW_SALMON):
		{
			properties["name"] = "Raw Salmon";
			rect = sf::IntRect(0, 32, 16, 16);
		} break;
		default:
			unknownID = true;
	}

	if(!unknownID)
	{
		std::unique_ptr<Item> item(new Item(context, context.contentManager->getTexture(Textures::ITEMS), rect, itemList[toCategory(properties["category"])].size(), properties));
		std::unique_ptr<Item> allItem(new Item(context, context.contentManager->getTexture(Textures::ITEMS), rect, itemList[ALL].size(), properties));
		itemList[toCategory(properties["category"])].push_back(std::move(item));
		itemList[ALL].push_back(std::move(allItem));
	}
	else
	{
		stream.str(""); stream<<"Unknown ItemID! ("<<id<<")";
		context.console->logError(stream.str());
	}
}