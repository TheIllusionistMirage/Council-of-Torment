#pragma once

#include "ArmorPart.h"
#include <string>
#include <map>

/*	The armor manager is like a container for all the
	different armor parts a humanoid can have.
	It handles all the armor parts, and the user
	can simply add parts.
*/
class ArmorManager
{
	private:
		State::Context context;
		ArmorPart head;
		ArmorPart torso;
		ArmorPart neck;
		ArmorPart legs;
		ArmorPart hands;
		ArmorPart feet;
		ArmorPart ring;
		ArmorPart sword;
		ArmorPart shield;

	public:
		ArmorManager(State::Context context);

		void add(ArmorPart part);
		void update(Humanoid& humanoid);
		void render();

		bool containsPart(ArmorType type) const;
};