#include "ArmorManager.h"
#include "Humanoid.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the armor manager
 * ----------------------------------------------------------------------
 */
ArmorManager::ArmorManager(State::Context context)
: context(context)
, torso(ArmorType::TORSO, context, "Content/Textures/Armor/default_torso.png")
, feet(ArmorType::FEET, context, "Content/Textures/Armor/default_robe.png")
, head()
, hands()
, neck()
, ring()
, legs()
, sword(ArmorType::WEAPON_2H, context, "Content/Textures/Armor/wooden_staff.png")
, shield()
{
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Adds a new armor part
 * ----------------------------------------------------------------------
 */
void ArmorManager::add(ArmorPart part)
{
	switch(part.getType())
	{
		case ArmorType::FEET:
			feet = part;
			break;

		case ArmorType::HANDS:
			hands = part;	
			break;

		case ArmorType::HEAD:
			head = part;	
			break;

		case ArmorType::LEGS:
			legs = part;	
			break;

		case ArmorType::NECK:
			neck = part;	
			break;

		case ArmorType::RING:
			ring = part;	
			break;

		case ArmorType::TORSO:
			torso = part;	
			break;

		case ArmorType::WEAPON_1H:
			sword = part;
			break;

		case ArmorType::WEAPON_2H:
			sword = part;
			break;

		case ArmorType::SHIELD:
			shield = part;
			break;
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates all armor parts
 * ----------------------------------------------------------------------
 */
void ArmorManager::update(Humanoid& humanoid)
{
	// Update all armor parts
	head.update(humanoid);
	torso.update(humanoid);
	neck.update(humanoid);
	legs.update(humanoid);
	hands.update(humanoid);
	feet.update(humanoid);
	ring.update(humanoid);
	sword.update(humanoid);
	shield.update(humanoid);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders all armor parts
 * ----------------------------------------------------------------------
 */
void ArmorManager::render()
{
	// Render all armor parts
	feet.render(*context.window);
	legs.render(*context.window);
	torso.render(*context.window);
	head.render(*context.window);
	neck.render(*context.window);
	hands.render(*context.window);
	sword.render(*context.window);
	shield.render(*context.window);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns true if a certain armor part is already added
 * ----------------------------------------------------------------------
 */
bool ArmorManager::containsPart(ArmorType type) const
{
	if(sword.getType() == type)
		return true;
	else if(head.getType() == type)
		return true;
	else 
		return false;
}