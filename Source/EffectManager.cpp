#include "EffectManager.h"
#include "Console.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the effect manager
 * ----------------------------------------------------------------------
 */
EffectManager::EffectManager(State::Context context)
: context(context)
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates all the effects
 * ----------------------------------------------------------------------
 */
void EffectManager::update(sf::Time elapsedTime)
{
	if(!effectList.empty())
	{
		// Check if an effect is done delete it
		for(auto effect = effectList.begin(); effect != effectList.end();)
		{
			if(effect->isDone())
			{
				effect = effectList.erase(effect++);
				context.console->logInfo("Removed element from GFX_Manager container...");
			}
			else
				++effect;
		}

		// Update the effects
		for(auto&& effect : effectList)
		{
			effect.update(elapsedTime, overlayTexture);

			// Activate an effect
			if(effect.canExecuteTrigger())
				effect.executeTrigger();
		}
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders all the effects
 * ----------------------------------------------------------------------
 */
void EffectManager::render()
{
	// Set the default view
	context.window->setView(context.window->getDefaultView());

	// Draw the overlay sprite
	if(!effectList.empty())
		context.window->draw(sf::Sprite(overlayTexture.getTexture()));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Creates the render texture
 * ----------------------------------------------------------------------
 */
void EffectManager::createTexture()
{
	// Create the overlay texture
	overlayTexture.create(context.window->getSize().x, context.window->getSize().y);
	overlayTexture.clear(sf::Color::Transparent);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Adds a new effect
 * ----------------------------------------------------------------------
 */
void EffectManager::add(Effect effect)
{
	// Add a new effect to the manager
	effectList.push_back(effect);
	context.console->logInfo("Added a new GFX to the system.");
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Clears all effects
 * ----------------------------------------------------------------------
 */
void EffectManager::clear()
{
	// Clear all effects
	effectList.clear();
	context.console->logInfo("Effect manager has been cleared.");
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns all the effects
 * ----------------------------------------------------------------------
 */
std::vector<Effect>& EffectManager::getEffectList()
{
	// Return the effect list
	return effectList;
}