#include "Component.h"

namespace GUI
{
	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Constructor of the component
	 * ----------------------------------------------------------------------
	 */
	Component::Component()
	: selected(false)
	, active(false)
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Destructor of the component
	 * ----------------------------------------------------------------------
	 */
	Component::~Component()
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns a default float rect as bounds
	 * ----------------------------------------------------------------------
	 */
	sf::FloatRect Component::getBounds() const
	{
		return sf::FloatRect();
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns true if the component is selected
	 * ----------------------------------------------------------------------
	 */
	bool Component::isSelected() const
	{
		return selected;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Selects a component
	 * ----------------------------------------------------------------------
	 */
	void Component::select()
	{
		selected = true;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deselects a component
	 * ----------------------------------------------------------------------
	 */
	void Component::deselect()
	{
		selected = false;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns true if the component is active
	 * ----------------------------------------------------------------------
	 */
	bool Component::isActive() const
	{
		return active;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Activates the component
	 * ----------------------------------------------------------------------
	 */
	void Component::activate()
	{
		active = true;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Deactivates the component
	 * ----------------------------------------------------------------------
	 */
	void Component::deactivate()
	{
		active = false;
	}
}