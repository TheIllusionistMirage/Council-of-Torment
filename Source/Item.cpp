#include "Item.h"

Item::Item(State::Context context, sf::Texture& texture, sf::IntRect rect, int order, std::map<std::string, std::string> properties)
: context(context)
, order(order)
, properties(properties)
, item(texture, rect)
, line(sf::Lines, 2)
, equipped(false)
, name(properties["name"], context.contentManager->getFont(Fonts::SEGOEUI), 12)
{
	this->properties["equipped"] = "False";

	line[0].color = sf::Color(48, 48, 48);
	line[1].color = sf::Color(48, 48, 48);

	equipShape = sf::RectangleShape(sf::Vector2f(214.0f, 20.0f));
	equipShape.setFillColor(sf::Color(100, 100, 100, 100));
}

void Item::render()
{
	context.window->draw(item);
	context.window->draw(line);
	context.window->draw(name);

	if(equipped)
		context.window->draw(equipShape);
}

void Item::setPosition(sf::Vector2f pos)
{
	item.setPosition(pos);
	name.setPosition(pos.x + 18.0f, pos.y + 1.0f);
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