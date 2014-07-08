#include "Button.h"
#include "../Stdafx.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Constructor of the button GUI element
	 * ----------------------------------------------------------------------
	 */
	Button::Button(const ContentManager& contentManager)
	: callback()
	, normalTexture(contentManager.getTexture(Textures::BUTTON_NORMAL))
	, selectedTexture(contentManager.getTexture(Textures::BUTTON_SELECTED))
	, pressedTexture(contentManager.getTexture(Textures::BUTTON_PRESSED))
	, text("", contentManager.getFont(Fonts::FRENCH_GROTESQUE), 20)
	, sprite(normalTexture)
	, toggle(false)
	{
		// Center the text
		sf::FloatRect bounds = sprite.getLocalBounds();
		text.setPosition(bounds.width / 2.0f, bounds.height / 2.0f - text.getCharacterSize() / 4.0f);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Renders the button with it's text
	 * ----------------------------------------------------------------------
	 */
	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Render the sprite and the text
		states.transform *= getTransform();
		target.draw(sprite, states);
		target.draw(text, states);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Selects the button
	 * ----------------------------------------------------------------------
	 */
	void Button::select()
	{
		// Select the button
		Component::select();
		sprite.setTexture(selectedTexture);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deselects the button
	 * ----------------------------------------------------------------------
	 */
	void Button::deselect()
	{
		// Deselect the button
		Component::deselect();
		sprite.setTexture(normalTexture);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Activates the button
	 * ----------------------------------------------------------------------
	 */
	void Button::activate()
	{
		// Activate the button
		Component::activate();

		// Set the pressed texture if it is toggled
		if(toggle)
			sprite.setTexture(pressedTexture);

		// Call the function if available
		if(callback)
			callback();

		// Deactivate the button if it isn't toggled
		if(!toggle)
			deactivate();
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deactivates the button
	 * ----------------------------------------------------------------------
	 */
	void Button::deactivate()
	{
		// Deactivate the button
		Component::deactivate();

		if(toggle)
		{
			// Set another texture if it is toggled
			if(isSelected())
				sprite.setTexture(selectedTexture);
			else
				sprite.setTexture(normalTexture);
		}
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Handles the events for the button
	 * ----------------------------------------------------------------------
	 */
	void Button::handleEvent(const sf::Event& windowEvent)
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Sets another callback function for the button if you press it
	 * ----------------------------------------------------------------------
	 */
	void Button::setCallback(Callback callback)
	{
		// Set the new callback function
		this->callback = std::move(callback);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Sets another text for the button text
	 * ----------------------------------------------------------------------
	 */
	void Button::setText(const std::string& text)
	{
		// Set a new text and centers the origin
		this->text.setString(text);
		centerOrigin(this->text);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Changes the toggle flag
	 * ----------------------------------------------------------------------
	 */
	void Button::setToggle(bool toggle)
	{
		// Set the toggle
		this->toggle = toggle;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns the bounds of the button sprite
	 * ----------------------------------------------------------------------
	 */
	sf::FloatRect Button::getBounds() const
	{
		// Return the bounds of the button
		return getTransform().transformRect(sprite.getLocalBounds());
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns true if the button is selectable
	 * ----------------------------------------------------------------------
	 */
	bool Button::isSelectable() const
	{
		// Buttons are selectable
		return true;
	}
}