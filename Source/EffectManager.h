#pragma once

#include "State.h"
#include "Effect.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <vector>

/*	The effect manager handles all effects and
	destroys them as soon as they are useless.
	It's nothing fancy to say about that.
*/
class EffectManager
{
	public:
		EffectManager(State::Context context);

		void clear();
		void render();
		void createTexture();
		void add(Effect effect);
		void update(sf::Time elapsedTime);

		std::vector<Effect>& getEffectList();

	private:
		State::Context context;
		sf::RenderTexture overlayTexture;
		std::vector<Effect> effectList;
};