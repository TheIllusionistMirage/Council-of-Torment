#include "Label.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Constructor of the label GUI element
* ----------------------------------------------------------------------
*/
	Label::Label(const std::string& text, const sf::Font& font, const sf::Vector2f& position)
	: text(text, font, 20)
	{
		this->text.setPosition(position);
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 19 April 2014
* Description: Default constructor of the label GUI element
* ----------------------------------------------------------------------
*/
	Label::Label()
	{
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Renders the label
* ----------------------------------------------------------------------
*/
	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Render the text
		states.transform *= getTransform();
		target.draw(text, states);
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Sets the label a new text
* ----------------------------------------------------------------------
*/
	void Label::setText(const std::string& text)
	{
		// Set a new text
		this->text.setString(text);
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Sets the label a new color
* ----------------------------------------------------------------------
*/
	void Label::setColor(const sf::Color& color)
	{
		// Set a new color
		this->color = color;
		text.setColor(color);
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Sets the label a new character size
* ----------------------------------------------------------------------
*/
	void Label::setCharacterSize(unsigned int size)
	{
		// Set the character size
		text.setCharacterSize(size);
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Returns the bounds of the text of the label
* ----------------------------------------------------------------------
*/
	sf::FloatRect Label::getBounds() const
	{
		// Return the bounds of the text
		return getTransform().transformRect(text.getLocalBounds());
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Returns true if the label is selectable
* ----------------------------------------------------------------------
*/
	bool Label::isSelectable() const
	{
		// Labels are not selectable
		return false;
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Handles the events of the label
* ----------------------------------------------------------------------
*/
	void Label::handleEvent(const sf::Event& windowEvent)
	{
	}
}