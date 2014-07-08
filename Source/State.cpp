#include "State.h"
#include "StateStack.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the context struct
 * ----------------------------------------------------------------------
 */
State::Context::Context(sf::RenderWindow& window, ContentManager& contentManager, EffectManager& effectManager, LightManager& lightManager,
						Player& player, GameMap& gameMap, Console& console, GlobalTime& globalTime)
: window(&window)
, player(&player)
, gameMap(&gameMap)
, console(&console)
, globalTime(&globalTime)
, lightManager(&lightManager)
, effectManager(&effectManager)
, contentManager(&contentManager)
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the state class
 * ----------------------------------------------------------------------
 */
State::State(StateStack& stack, Context context)
: stack(&stack)
, context(context)
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Destructor of the state class
 * ----------------------------------------------------------------------
 */
State::~State()
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Pushes a new state to the stack
 * ----------------------------------------------------------------------
 */
void State::requestStackPush(States::ID stateID)
{
	// Push a new state to the stack
	stack->pushState(stateID);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Pops a state from the stack
 * ----------------------------------------------------------------------
 */
void State::requestStackPop()
{
	// Pop a state from the stack
	stack->popState();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Clears all states
 * ----------------------------------------------------------------------
 */
void State::requestStateClear()
{
	// Clear all states
	stack->clearStates();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the context
 * ----------------------------------------------------------------------
 */
State::Context State::getContext() const
{
	// Return the context
	return context;
}