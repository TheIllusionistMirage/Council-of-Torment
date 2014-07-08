#pragma once

#include "State.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <vector>

/*	The Light class is a very powerful class
	which is used to give the game some
	awesome lighting effects.
	You can give a light various properties.
*/
class Light
{
	public:
		Light(State::Context context, float xPos, float yPos, float radius, sf::Color color);

		void init();
		void update(sf::Time elapsedTime);
		void setPosition(float xPos, float yPos);
		void enableFlickering(float radius = 5.0f, float speed = 50.0f);
		void render(const sf::View& camera, sf::RenderTexture& lightTexture);

	private:
		State::Context context;
		sf::VertexArray lightArray;
		sf::Vector2f position;
		sf::Vector2f flickerPosition;
		sf::Vector2f flickerDirection;
		sf::Shader& shader;
		sf::Color color;
		float radius;
		float size;
		float flickerTime;
		float flickerRadius;
		float flickerSpeed;
		bool isFlickering;
		bool newFlickering;
		bool moveBack;
};