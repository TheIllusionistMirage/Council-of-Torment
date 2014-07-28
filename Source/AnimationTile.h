#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"

/*	An AnimationTile is effectivelly an animation
	that plays on its own. It's a 32x32 (default)
	tile that is animated. It can't only be used
	in maps, you can create it anywhere.
*/
class AnimationTile
{
	public:
		AnimationTile(State::Context context, std::string l, std::string name, int frame_size, float frame_time);

		void setPosition(sf::Vector2f pos);
		void update(sf::Time elapsedTime);
		void setDrawAtNight(bool state);
		void setFrameSize(int size);
		void setFrameTime(float s);
		std::string getLayer();
		bool getDrawAtNight();
		void setID(int i);
		void render();

		sf::Sprite& getSprite();

	private:
		State::Context context;
		std::string layer;
		sf::Texture texture;
		sf::Sprite sprite;
		sf::IntRect bounds;
		float frameSwitchTime;
		bool drawAtNight;
		int frameX, frameY;
		int frameSize;
		float timer;
		int id;
};