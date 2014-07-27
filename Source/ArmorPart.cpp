#include "ArmorPart.h"
#include "LivingEntity.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the armor part
 * ----------------------------------------------------------------------
 */
ArmorPart::ArmorPart(ArmorType type, State::Context context, std::string fileName)
: type(type)
, texture(context.contentManager->loadTexture(fileName))
, sprite(texture)
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the armor part
 * ----------------------------------------------------------------------
 */
void ArmorPart::update(LivingEntity& LivingEntity)
{
	// Update the position and texture rect
	sprite.setPosition(LivingEntity.getPosition());
	sprite.setTextureRect(LivingEntity.getTextureRext());
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the armor part
 * ----------------------------------------------------------------------
 */
void ArmorPart::render(sf::RenderTexture& window)
{
	// Render the armor part
	window.draw(sprite);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Sets another type for the armor part
 * ----------------------------------------------------------------------
 */
void ArmorPart::setType(ArmorType type)
{
	// Set the armor type
	this->type = type;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the type of the armor part
 * ----------------------------------------------------------------------
 */
ArmorType ArmorPart::getType() const
{
	// Return the armor type
	return type;
}