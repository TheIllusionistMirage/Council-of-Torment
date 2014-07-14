#include "Item.h"

Item::Item(State::Context context, sf::IntRect rect, sf::IntRect iconRect, int order, std::map<std::string, std::string> properties)
: context(context)
, order(order)
, properties(properties)
, line(sf::Lines, 2)
, scaleFactor {2.0f}
, equipped(false)
, described {false}
, increaseScale {false}
, descriptionLines {std::stoi(properties["description_lines"])}
, itemIconShape {{32, 32}}
, item(context.contentManager->getTexture(Textures::ITEMS), rect)
, itemIcon(context.contentManager->getTexture(Textures::ICON_ITEMS), iconRect)
, description {properties["description"], context.contentManager->getFont(Fonts::SEGOEUI), 12}
, name(properties["name"], context.contentManager->getFont(Fonts::SEGOEUI), 12)
{
	this->properties["equipped"] = "False";

	line[0].color = sf::Color(48, 48, 48);
	line[1].color = sf::Color(48, 48, 48);

	equipShape = sf::RectangleShape(sf::Vector2f(214.0f, 20.0f));
	equipShape.setFillColor(sf::Color(100, 100, 100, 100));

	itemIconShape.setOutlineThickness(1);
	itemIconShape.setOutlineColor(sf::Color(100, 100, 100));
	itemIconShape.setFillColor(sf::Color::Transparent);
}

void Item::update(sf::Time elapsedTime)
{
	if(!increaseScale)
	{
		itemIcon.setScale(sf::Vector2f(itemIcon.getScale().x - scaleFactor * elapsedTime.asSeconds(), 1.0f));
		if(itemIcon.getScale().x < 0.5f)
			increaseScale = true;
	}
	else
	{
		itemIcon.setScale(sf::Vector2f(itemIcon.getScale().x + scaleFactor * elapsedTime.asSeconds(), 1.0f));
		if(itemIcon.getScale().x > 1.0f)
		{
			itemIcon.setScale(sf::Vector2f(1.0f, 1.0f));
			increaseScale = false;
		}
	}

	description.setPosition(name.getPosition() + sf::Vector2f(-10.0f, 20.0f));
	descriptionBackground.setPosition(description.getPosition() - sf::Vector2f(8.0f, 0.0f));
	itemIconShape.setPosition(name.getPosition() + sf::Vector2f(187.0f - itemIconShape.getLocalBounds().width, 28.0f));
	itemIcon.setPosition(itemIconShape.getPosition().x + itemIconShape.getLocalBounds().width / 2 - itemIcon.getGlobalBounds().width / 2 - 1.0f, itemIconShape.getPosition().y);
}

void Item::render()
{
	context.window->draw(item);
	context.window->draw(line);
	context.window->draw(name);

	if(equipped)
		context.window->draw(equipShape);

	if(std::stoi(properties["number"]) > 1)
	{
		std::stringstream stream;
		stream << "x" << std::stoi(properties["number"]);
		sf::Text number {stream.str(), context.contentManager->getFont(Fonts::SEGOEUI), 12};
		number.setPosition(name.getPosition() + sf::Vector2f(190.0f - number.getLocalBounds().width, 0.0f));
		context.window->draw(number);
	}

	// If the player wants to have more information by right clicking on the item
	if(described)
	{
		context.window->draw(descriptionBackground);
		context.window->draw(description);
		context.window->draw(itemIconShape);
		context.window->draw(itemIcon);
	}
}

void Item::updateDescriptionText()
{
	std::stringstream stream;
	stream << "Value: " << std::stof(properties["value"]) << "g";
	if(std::stoi(properties["number"]) > 1) stream << "\t(" << std::stoi(properties["number"]) * std::stof(properties["value"]) << "g total)\n";
	else stream << "\n";

	stream << "Weight: " << std::stof(properties["weight"]);
	if(std::stoi(properties["number"]) > 1) stream << "\t(" << std::stoi(properties["number"]) * std::stof(properties["weight"]) << " total)\n";
	else stream << "\n";

	stream << "\n\"" << properties["description"] << "\"";
	description.setString(stream.str());

	descriptionBackground = sf::RectangleShape {{212, description.getLocalBounds().height + 3.0f}};
	descriptionBackground.setOutlineThickness(1);
	descriptionBackground.setOutlineColor(sf::Color(100, 100, 100));
	descriptionBackground.setFillColor(sf::Color(10, 10, 10));
}

void Item::setPosition(sf::Vector2f pos)
{
	item.setPosition(pos);
	name.setPosition(pos.x + 18.0f, pos.y);
	equipShape.setPosition(pos.x - 1.0f, pos.y - 1.0f);
	line[0].position = sf::Vector2f(pos.x - 1.0f, pos.y + 18.0f);
	line[1].position = sf::Vector2f(pos.x + 213.0f, pos.y + 18.0f);
}

void Item::equip()
{
	if(properties["equipable"] == "True")
		equipped = !equipped;

	if(equipped)
		this->properties["equipped"] = "True";
	else
		this->properties["equipped"] = "False";
}