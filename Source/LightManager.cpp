#include "LightManager.h"
#include "Player.h"
#include "Console.h"
#include "GameMap.h"
#include "GlobalTime.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Constructor of the light manager class
 * ----------------------------------------------------------------------
 */
LightManager::LightManager(State::Context context)
: context(context)
, duskColor(sf::Color::Transparent)
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Inits the light manager
 * ----------------------------------------------------------------------
 */
void LightManager::init()
{
	// Create the render texture
	if(!lightTexture.create(context.window->getSize().x, context.window->getSize().y))
		context.console->logError("Failed to create the light texture!");

	// Init all the lights
	for(auto&& light : lightList)
		light.init();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Updates the light manager
 * ----------------------------------------------------------------------
 */
void LightManager::update(GameMap* gameMap, sf::Time elapsedTime)
{
	// Update all the lights
	for(auto&& light : lightList)
		light.update(elapsedTime);

	// Update the duskColor by natural lighting
	if(gameMap->isNaturalLighting())
	{
		int hours = context.globalTime->getHours();
		float minutes = context.globalTime->getMinutes() + hours * 60.0f;
		float multiplier = 0.0f;

		if(hours >= START_MORNING && hours < MORNING)			// Morning -> From Night to Morning
		{
			multiplier = (minutes - START_MORNING * 60.0f) / (MORNING * 60.0f - START_MORNING * 60.0f);
			duskColor = sf::Color(night_color.r - int((night_color.r - morning_color.r) * multiplier + 0.5f), night_color.g - int((night_color.g - morning_color.g) * multiplier + 0.5f),
				night_color.b - int((night_color.b - morning_color.b) * multiplier + 0.5f), night_color.a + int((morning_color.a - night_color.a) * multiplier + 0.5f));
		}
		else if(hours >= START_DAWN && hours < DAWN)			// Dawn -> From Morning to Dawn
		{
			multiplier = (minutes - START_DAWN * 60.0f) / (DAWN * 60.0f - START_DAWN * 60.0f);
			duskColor = sf::Color(morning_color.r + int((dawn_color.r - morning_color.r) * multiplier + 0.5f), morning_color.g + int((dawn_color.g - morning_color.g) * multiplier + 0.5f),
				morning_color.b + int((dawn_color.b - morning_color.b) * multiplier + 0.5f), morning_color.a + int((dawn_color.a - morning_color.a) * multiplier + 0.5f));
		}
		else if(hours >= START_NOON && hours < NOON)			// Noon -> From Dawn to Noon
		{
			multiplier = (minutes - START_NOON * 60.0f) / (NOON * 60.0f - START_NOON * 60.0f);
			duskColor = sf::Color(dawn_color.r + int((noon_color.r - dawn_color.r) * multiplier + 0.5f), dawn_color.g + int((noon_color.g - dawn_color.g) * multiplier + 0.5f),
				dawn_color.b + int((noon_color.b - dawn_color.b) * multiplier + 0.5f), dawn_color.a + int((noon_color.a - dawn_color.a) * multiplier + 0.5f));
		}
		else if(hours >= START_AFTERNOON && hours < AFTERNOON)	// Afternoon -> From Noon to Afternoon
		{
			multiplier = (minutes - START_AFTERNOON * 60.0f) / (AFTERNOON * 60.0f - START_AFTERNOON * 60.0f);;
			duskColor = sf::Color(noon_color.r - int((noon_color.r - afternoon_color.r) * multiplier + 0.5f), noon_color.g - int((noon_color.g - afternoon_color.g) * multiplier + 0.5f),
				noon_color.b - int((noon_color.b - afternoon_color.b) * multiplier + 0.5f), noon_color.a - int((noon_color.a - afternoon_color.a) * multiplier + 0.5f));
		}
		else if(hours >= START_DUSK && hours < DUSK)			// Dusk -> From Afternoon to Dusk
		{
			multiplier = (minutes - START_DUSK * 60.0f) / (DUSK * 60.0f - START_DUSK * 60.0f);;
			duskColor = sf::Color(afternoon_color.r - int((afternoon_color.r - dusk_color.r) * multiplier + 0.5f), afternoon_color.g - int((afternoon_color.g - dusk_color.g) * multiplier + 0.5f),
				afternoon_color.b - int((afternoon_color.b - dusk_color.b) * multiplier + 0.5f), afternoon_color.a - int((afternoon_color.a - dusk_color.a) * multiplier + 0.5f));
		}
		else if(hours >= START_NIGHT && hours < NIGHT)			// Night -> From Dusk to Night
		{
			multiplier = (minutes - START_NIGHT * 60.0f) / (NIGHT * 60.0f - START_NIGHT * 60.0f);;
			duskColor = sf::Color(dusk_color.r + int((night_color.r - dusk_color.r) * multiplier + 0.5f), dusk_color.g + int((night_color.g - dusk_color.g) * multiplier + 0.5f),
				dusk_color.b + int((night_color.b - dusk_color.b) * multiplier + 0.5f), dusk_color.a - int((dusk_color.a - night_color.a) * multiplier + 0.5f));
		}
		else if(hours < START_MORNING)
			duskColor = night_color;
		else if(hours < START_DAWN)
			duskColor = morning_color;
		else if(hours < START_NOON)
			duskColor = dawn_color;
		else if(hours < START_AFTERNOON)
			duskColor = noon_color;
		else if(hours < START_DUSK)
			duskColor = afternoon_color;
		else if(hours < START_NIGHT)
			duskColor = dusk_color;
		else if(hours < 25)
			duskColor = night_color;
	}
	else
		duskColor = night_color;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Renders the light manager
 * ----------------------------------------------------------------------
 */
void LightManager::render(const sf::View& camera)
{
	// Set the view of the camera
	sf::Vector2f cameraPos(camera.getCenter() - sf::Vector2f(context.window->getSize() / 2u));
	context.window->setView(camera);

	// Clear the window to see light only
	//context.window->clear(sf::Color::White);

	// Clear the light texture
	lightTexture.clear();

	// Render all the lights
	for(auto&& light : lightList)
		light.render(camera, lightTexture);

	// Render the dusk color over the light texture
	sf::RectangleShape shape(sf::Vector2f(context.window->getSize()));
	shape.setFillColor(duskColor);
	lightTexture.draw(shape);

	// Render the light texture on the rest of the window with BlendMultiply!
	lightTexture.display();
	sf::Sprite sprite(lightTexture.getTexture());
	sprite.setPosition(cameraPos);
	context.window->draw(sprite, sf::BlendMultiply);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Adds a new light to the manager
 * ----------------------------------------------------------------------
 */
void LightManager::addLight(float xPos, float yPos, float radius, sf::Color color)
{
	// Push a new light to the vector
	lightList.push_back(Light(context, xPos, yPos, radius, color));
	lightList.back().init();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Clears all lights
 * ----------------------------------------------------------------------
 */
void LightManager::clearLights()
{
	// Clear all the lights
	lightList.clear();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 29 March 2014
 * Description: Sets a new dusk color
 * ----------------------------------------------------------------------
 */
void LightManager::setDuskColor(sf::Color duskColor)
{
	// Set a new dusk color
	this->duskColor = duskColor;
}