#include "Container.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace GUI
{
	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Constructor of the container
	 * ----------------------------------------------------------------------
	 */
	Container::Container(State::Context context)
	: childrenList()
	, context(context)
	, selectedChild(-1)
	{
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Renders all components
	 * ----------------------------------------------------------------------
	 */
	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Use the transform class
		states.transform *= getTransform();

		// Render all components
		for(const Component::Ptr& child : childrenList)
			target.draw(*child, states);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Adds a new component to the vector
	 * ----------------------------------------------------------------------
	 */
	void Container::add(Component::Ptr component)
	{
		// Add a new component to the list
		childrenList.push_back(component);

		// If no component is selected, the new one gets selected if possible
		if(!hasSelection() && component->isSelectable())
			select(childrenList.size() - 1);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Handles the events for all components
	 * ----------------------------------------------------------------------
	 */
	void Container::handleEvent(const sf::Event& windowEvent)
	{
		// If a component is selected and active the events get passed to it
		if(hasSelection() && childrenList[selectedChild]->isActive())
			childrenList[selectedChild]->handleEvent(windowEvent);

		// Otherwise the events get executed here
		else
		{
			if(windowEvent.type == sf::Event::KeyPressed)
			{
				// Select the previous component
				if(windowEvent.key.code == sf::Keyboard::W || windowEvent.key.code == sf::Keyboard::Up ||
					windowEvent.key.code == sf::Keyboard::A || windowEvent.key.code == sf::Keyboard::Left)
					selectPrevious();

				// Select the next component
				else if(windowEvent.key.code == sf::Keyboard::S || windowEvent.key.code == sf::Keyboard::Down ||
					windowEvent.key.code == sf::Keyboard::D || windowEvent.key.code == sf::Keyboard::Right)
					selectNext();

				// Activate the selected component
				else if(hasSelection() && windowEvent.key.code == sf::Keyboard::Return || windowEvent.key.code == sf::Keyboard::Space)
					childrenList[selectedChild]->activate();
			}
			else if(windowEvent.type == sf::Event::MouseMoved)
			{
				// Get the mouse position
				sf::Vector2i mousePos = sf::Mouse::getPosition(*context.window);

				// Some variables to check if the mouse hovers over a component
				std::size_t index = 0;
				bool hover = false;

				// Check if a component contains the mouse position
				for(auto&& child : childrenList)
				{
					if(child->getBounds().contains(static_cast<sf::Vector2f>(mousePos)))
					{
						hover = true;
						break;
					}
					++index;
				}

				// If the mouse hovers over a component it gets selected
				if(hover)
					select(index);
			}
			else if(windowEvent.type == sf::Event::MouseButtonPressed)
			{
				// Activate a component if the mouse still hovers over it
				if(windowEvent.mouseButton.button == sf::Mouse::Left)
				{
					// If no component is selected it has no sence to continue
					if(!hasSelection())
						return;

					// Get the mouse position
					sf::Vector2i mousePos = sf::Mouse::getPosition(*context.window);

					if(childrenList[selectedChild]->getBounds().contains(static_cast<sf::Vector2f>(mousePos)))
						childrenList[selectedChild]->activate();
				}
			}
		}
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Selects the next component
	 * ----------------------------------------------------------------------
	 */
	void Container::selectNext()
	{
		// If no component is selected it has no sence to continue
		if(!hasSelection())
			return;

		// Search the next selectable component using the modulo
		int next = selectedChild;
		do
			next = (next + 1) % childrenList.size();
		while(!childrenList[next]->isSelectable());

		// Select the component
		select(next);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Selects the previous component
	 * ----------------------------------------------------------------------
	 */
	void Container::selectPrevious()
	{
		// If no component is selected it has no sence to continue
		if(!hasSelection())
			return;

		// Search the previous selectable component using the modulo
		int prev = selectedChild;
		do
			prev = (prev + childrenList.size() - 1) % childrenList.size();
		while (!childrenList[prev]->isSelectable());

		// Select the component
		select(prev);
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Selects a certain component
	 * ----------------------------------------------------------------------
	 */
	void Container::select(std::size_t index)
	{
		// Check if the component is selectable
		if(childrenList[index]->isSelectable())
		{
			// Check, if it is a new selection
			if(index != selectedChild)
			{
				// Poll the selection sound
				if(selectedChild != -1)
					context.contentManager->pollSoundEffect(Sounds::TEST, 50);

				// If another component is selected, it gets deselected
				if(hasSelection())
					childrenList[selectedChild]->deselect();

				// Select the component
				childrenList[index]->select();
				selectedChild = index;
			}
		}
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns true if the container is selectable
	 * ----------------------------------------------------------------------
	 */
	bool Container::isSelectable() const
	{
		// The container is not selectable
		return false;
	}

	/* ----------------------------------------------------------------------
	 * Author: Julian
	 * Date: 19 January 2014
	 * Description: Returns true if a component is selected
	 * ----------------------------------------------------------------------
	 */
	bool Container::hasSelection() const
	{
		// If nothing is selected this will return false
		return selectedChild >= 0;
	}
}
