#include "Checkbox.h"
#include "../Stdafx.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Constructor of the checkbox GUI element
	 * ----------------------------------------------------------------------
	 */
	Checkbox::Checkbox(const sf::Texture& texture, int addHeight)
	: callback()
	, texture(texture)
	, addHeight(addHeight)
	, boxSprite(texture, sf::IntRect(0, 0 + addHeight, 20, 20))
	, checkedSprite(texture, sf::IntRect(40, 0 + addHeight, 20, 20))
	, checked(false)
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Renders the checkbox
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Render the box
		states.transform *= getTransform();
		target.draw(boxSprite, states);

		// Render the checked sprite only if checked
		if(checked)
			target.draw(checkedSprite, states);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Selects the checkbox
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::select()
	{
		// Select the checkbox
		Component::select();
		boxSprite.setTextureRect(sf::IntRect(20, 0 + addHeight, 20, 20));
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deselects the checkbox
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::deselect()
	{
		// Deselect the checkbox
		Component::deselect();
		boxSprite.setTextureRect(sf::IntRect(0, 0 + addHeight, 20, 20));
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Activates the checkbox
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::activate()
	{
		// Activate the checkbox
		Component::activate();

		// Call the function if available
		if(callback)
			callback();

		// Change the bool
		checked = !checked;

		// Deactivate the checkbox
		deactivate();
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deactivates the checkbox
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::deactivate()
	{
		// Deactivate the checkbox
		Component::deactivate();

		// Set another texture rect if it is selected
		if(isSelected())
			boxSprite.setTextureRect(sf::IntRect(20, 0 + addHeight, 20, 20));
		else
			boxSprite.setTextureRect(sf::IntRect(0, 0 + addHeight, 20, 20));
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Handles the events of the checkbox
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::handleEvent(const sf::Event& windowEvent)
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Changes the flag if it is checked
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::setChecked(bool checked)
	{
		// Set if it is already checked
		this->checked = checked;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Sets the callback function for the checkbox
	 * ----------------------------------------------------------------------
	 */
	void Checkbox::setCallback(Callback callback)
	{
		// Set the new callback function
		this->callback = std::move(callback);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns the bounds of the sprite of the checkbox
	 * ----------------------------------------------------------------------
	 */
	sf::FloatRect Checkbox::getBounds() const
	{
		// Return the bounds of the checkbox
		return getTransform().transformRect(boxSprite.getLocalBounds());
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns true if the checkbox is selectable
	 * ----------------------------------------------------------------------
	 */
	bool Checkbox::isSelectable() const
	{
		// Checkboxes are selectable
		return true;
	}
}