#pragma once

#include "Humanoid.h"

enum class ArmorType
{
	HEAD,
	NECK,
	TORSO,
	HANDS,
	RING,
	LEGS,
	FEET,
	WEAPON_1H,
	WEAPON_2H,
	SHIELD
};

/*	The armor part is one part of the hole armor a humanoid
	can have. It is only used with the armor manager.
	An armor part has an armor type and a sprite with the
	correct texture. It will be drawn over the humanoid.
*/
class ArmorPart
{
	public:
		ArmorPart() {}
		ArmorPart(ArmorType type, State::Context context, std::string fileName);

		ArmorType getType() const;
		void setType(ArmorType type);
		void render(sf::RenderWindow& window);
		void update(Humanoid& humanoid);

	private:
		sf::Texture texture;
		sf::Sprite sprite;
		ArmorType type;
};