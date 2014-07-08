#pragma once

#include "State.h"
#include "Light.h"

/*	The Lighmanager class is basicially
	just a manager used for managing all
	different light sources. It renders
	them properly and makes sure everything
	looks just fine.
*/
class LightManager
{
	public:
		LightManager(State::Context context);

		void init();
		void clearLights();
		void render(const sf::View& camera);
		void setDuskColor(sf::Color duskColor);
		void update(GameMap* gameMap,sf::Time elapsedTime);
		void addLight(float xPos, float yPos, float radius, sf::Color color);
		std::vector<Light>& getLightList() {return lightList;}

	private:
		State::Context context;
		std::vector<Light> lightList;
		sf::RenderTexture lightTexture;
		sf::Color duskColor;
};