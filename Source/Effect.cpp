#include "Effect.h"
#include "Stdafx.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the effect class
 * ----------------------------------------------------------------------
 */
Effect::Effect(EffectType effectType)
: effectType(effectType)
, callback()
, fadeOutRatio(300)
, fadeInRatio(300)
, reachPoint(false)
, executeOnce(false)
, done(false)
{
	// Sets some values depending on the type
	switch(effectType)
	{
		case(FADE):
			alphaValue = 0;
			reachPoint = false;
			done = false;
			break;
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the effect
 * ----------------------------------------------------------------------
 */
void Effect::update(sf::Time elapsedTime, sf::RenderTexture& overlayTexture)
{
	switch(effectType)
	{
		case(FADE):

			// If the effect reached a certain point, the effect gets triggered
			if(!reachPoint && alphaValue >= 255)
			{
				reachPoint = true;
				executeOnce = true;
			}

			// Check if the effect is done
			if(reachPoint && alphaValue <= 0)
				done = true;

			// Update the transparency
			if(reachPoint)
				alphaValue -= fadeOutRatio * elapsedTime.asSeconds();
			else
				alphaValue += fadeInRatio * elapsedTime.asSeconds();
			break;
	}

	// AlphaValue can't have negative or +255 values
	if(alphaValue <= 0)
		alphaValue = 0;
	if(alphaValue >= 255)
		alphaValue = 255;

	// Clear the texture
	overlayTexture.clear(sf::Color(0, 0, 0, sf::Uint8(alphaValue)));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Sets another callback function
 * ----------------------------------------------------------------------
 */
void Effect::setCallback(Callback callback)
{
	// Set the new callback function
	this->callback = std::move(callback);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Calls the callback funtion (executes the trigger)
 * ----------------------------------------------------------------------
 */
void Effect::executeTrigger()
{
	// Call the callback function
	if(callback)
	{
		callback();
		executeOnce = false;
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns true if the effect is done
 * ----------------------------------------------------------------------
 */
bool Effect::isDone() const
{
	// Return if it is done
	return done;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns true if the effect can be triggered
 * ----------------------------------------------------------------------
 */
bool Effect::canExecuteTrigger() const
{
	// Return if it can execute a trigger
	return executeOnce;
}