#include "StateStack.h"
#include <cassert>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the state stack class
 * ----------------------------------------------------------------------
 */
StateStack::StateStack(State::Context context)
: stack()
, pendingList()
, context(context)
, factories()
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the pending change struct
 * ----------------------------------------------------------------------
 */
StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
: action(action)
, stateID(stateID)
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the stack
 * ----------------------------------------------------------------------
 */
void StateStack::update(sf::Time elapsedTime)
{
	// Iterate from top to bottom and stop if the update function returns false
	for(auto i = stack.rbegin(); i != stack.rend(); ++i)
	{
		// Update all the states
		if(!(*i)->update(elapsedTime))
			break;
	}

	// Apply the pending changes if there are any
	applyPendingChanges();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the stack
 * ----------------------------------------------------------------------
 */
void StateStack::render()
{
	// Render all the states
	for(State::Ptr& state : stack)
		state->render();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all events of the stack
 * ----------------------------------------------------------------------
 */
void StateStack::handleEvent(const sf::Event& windowEvent)
{
	// Iterate from top to bottom and stop if the handle event function returns false
	for(auto i = stack.rbegin(); i != stack.rend(); ++i)
	{
		// Handle all the events for the states
		if(!(*i)->handleEvent(windowEvent))
			break;
	}

	// Apply the pending changes if there are any
	applyPendingChanges();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Pushes a new state
 * ----------------------------------------------------------------------
 */
void StateStack::pushState(States::ID stateID)
{
	// Push a new state
	pendingList.push_back(PendingChange(Push, stateID));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Pops a state
 * ----------------------------------------------------------------------
 */
void StateStack::popState()
{
	// Pop a state
	pendingList.push_back(PendingChange(Pop));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Clears all states
 * ----------------------------------------------------------------------
 */
void StateStack::clearStates()
{
	// Clear all states
	pendingList.push_back(PendingChange(Clear));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the context
 * ----------------------------------------------------------------------
 */
State::Context StateStack::getContext() const
{
	// Return the context
	return context;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the size of the stack
 * ----------------------------------------------------------------------
 */
std::size_t StateStack::getSize() const
{
	// Return the size of the stack
	return stack.size();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns true if the stack is empty
 * ----------------------------------------------------------------------
 */
bool StateStack::isEmpty() const
{
	// Simply return true if empty
	return stack.empty();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Creates a new state
 * ----------------------------------------------------------------------
 */
State::Ptr StateStack::createState(States::ID stateID)
{
	// Search for a registered state and return it
	auto found = factories.find(stateID);
	assert(found != factories.end());
	return found->second();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Applies a pending change
 * ----------------------------------------------------------------------
 */
void StateStack::applyPendingChanges()
{
	// Go through all requests and handle them
	for(PendingChange change : pendingList)
	{
		switch(change.action)
		{
			// Push the new state
			case(Push):
				stack.push_back(createState(change.stateID));
				break;

			// Pop the state
			case(Pop):
				stack.pop_back();
				break;

			// Clear all states
			case(Clear):
				stack.clear();
				break;
		}
	}

	// Clear all handled requests
	pendingList.clear();
}