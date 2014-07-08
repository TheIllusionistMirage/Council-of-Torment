#include "Link.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Constructor of the link GUI element
	 * ----------------------------------------------------------------------
	 */
	Link::Link(const std::string& text, const sf::Font& font)
	: text(text, font, 20)
	, color(sf::Color::White)
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Renders the link
	 * ----------------------------------------------------------------------
	 */
	void Link::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Render the text
		states.transform *= getTransform();
		target.draw(text, states);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Selects the link
	 * ----------------------------------------------------------------------
	 */
	void Link::select()
	{
		// Select the link
		Component::select();
		text.setColor(sf::Color(color.r, color.g, color.b, 150));
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deselects the link
	 * ----------------------------------------------------------------------
	 */
	void Link::deselect()
	{
		// Deselect the link
		Component::deselect();
		text.setColor(color);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Activates the link
	 * ----------------------------------------------------------------------
	 */
	void Link::activate()
	{
		// Activate the link
		Component::activate();

		// Call the function if available
		if(callback)
			callback();

		// Deactivate the link
		deactivate();
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deactivates the link
	 * ----------------------------------------------------------------------
	 */
	void Link::deactivate()
	{
		// Deactivate the link
		Component::deactivate();

		// Darken the color if it is selected
		if(isSelected())
			text.setColor(sf::Color(color.r, color.g, color.b, 150));
		else
			text.setColor(color);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Sets the character size of the link
	 * ----------------------------------------------------------------------
	 */
	void Link::setCharacterSize(unsigned int size)
	{
		// Set the character size
		text.setCharacterSize(size);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Sets the link the color
	 * ----------------------------------------------------------------------
	 */
	void Link::setColor(const sf::Color& color)
	{
		// Set a new color
		this->color = color;
		text.setColor(color);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Sets the link the text
	 * ----------------------------------------------------------------------
	 */
	void Link::setText(const std::string& text)
	{
		// Set a new text
		this->text.setString(text);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Handles the events of the link
	 * ----------------------------------------------------------------------
	 */
	void Link::handleEvent(const sf::Event& windowEvent)
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Sets the link the callback function
	 * ----------------------------------------------------------------------
	 */
	void Link::setCallback(Callback callback)
	{
		// Set the new callback function
		this->callback = std::move(callback);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns the bounds of the text of the link
	 * ----------------------------------------------------------------------
	 */
	sf::FloatRect Link::getBounds() const
	{
		// Return the bounds of the text
		return getTransform().transformRect(text.getLocalBounds());
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns true if the link is selectable
	 * ----------------------------------------------------------------------
	 */
	bool Link::isSelectable() const
	{
		// Links are selectable
		return true;
	}
}