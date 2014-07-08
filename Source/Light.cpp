#include "Light.h"
#include "Player.h"
#include "GameMap.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Constructor of the light class
 * ----------------------------------------------------------------------
 */
Light::Light(State::Context context, float xPos, float yPos, float radius, sf::Color color)
: context(context)
, color(color)
, size(10.0f)
, radius(radius)
, position(xPos, yPos)
, lightArray(sf::Quads, 4)
, newFlickering(true)
, isFlickering(false)
, moveBack(false)
, flickerRadius(0.0f)
, flickerSpeed(0.0f)
, flickerTime(0.0f)
, flickerPosition(position)
, flickerDirection(0.0f, 0.0f)
, shader(context.contentManager->getShader(Shaders::LIGHT))
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Inits the light
 * ----------------------------------------------------------------------
 */
void Light::init()
{
	// Init the vertex array for the light rendering
	lightArray[1].texCoords = lightArray[1].position = sf::Vector2f(0, float(context.window->getSize().y));
    lightArray[2].texCoords = lightArray[2].position = sf::Vector2f(float(context.window->getSize().x), float(context.window->getSize().y));
    lightArray[3].texCoords = lightArray[3].position = sf::Vector2f(float(context.window->getSize().x), 0);
	lightArray[0].color = lightArray[1].color = lightArray[2].color = lightArray[3].color = color;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Updates the light
 * ----------------------------------------------------------------------
 */
void Light::update(sf::Time elapsedTime)
{
	if(isFlickering)
	{
		// Create the new values for a flicker
		if(newFlickering)
		{
			newFlickering = false;
			flickerDirection = sf::Vector2f(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
			flickerDirection = normalize(flickerDirection);
			flickerPosition = position;
			flickerTime = 0.0f;
		}

		if(!moveBack)
		{
			flickerTime += elapsedTime.asSeconds();
			flickerPosition += flickerDirection * flickerSpeed * elapsedTime.asSeconds();
			if(distance(flickerPosition, position) > flickerRadius)
			{
				flickerPosition -= flickerDirection * flickerSpeed * elapsedTime.asSeconds();
				moveBack = true;
			}
		}
		else
		{
			if(flickerTime > 0)
			{
				flickerTime -= elapsedTime.asSeconds();
				flickerPosition -= flickerDirection * flickerSpeed * elapsedTime.asSeconds();
			}
			else
			{
				moveBack = false;
				newFlickering = true;
			}
		}
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Renders the light
 * ----------------------------------------------------------------------
 */
void Light::render(const sf::View& camera, sf::RenderTexture& lightTexture)
{
	// Set the camera and the light position
	sf::Vector2f cameraPos(camera.getCenter() - sf::Vector2f(context.window->getSize() / 2u));
	sf::Vector2f lightPos(flickerPosition - cameraPos);

	// Pass some parameters to the shader
	shader.setParameter("center", lightPos);
	shader.setParameter("radius", radius);

	// Render the light on the light texture with the shader and BlendAdd
	sf::RenderStates states;
	states.blendMode = sf::BlendAdd;
	states.shader = &shader;
	lightTexture.draw(lightArray, states);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 25 March 2014
 * Description: Sets a new position for the light
 * ----------------------------------------------------------------------
 */
void Light::setPosition(float xPos, float yPos)
{
	// Set the position of the light
	position = sf::Vector2f(xPos, yPos);
	flickerPosition = position;
}

void Light::enableFlickering(float radius, float speed)
{
	flickerRadius = radius;
	flickerSpeed = speed;
	isFlickering = true;
}