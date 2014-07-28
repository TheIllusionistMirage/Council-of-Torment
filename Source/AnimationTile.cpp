#include "AnimationTile.h"
#include "Contentmanager.h"
#include "Player.h"
#include "Stdafx.h"

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: Constructor for an animation
 * ----------------------------------------------------------------------
 */
AnimationTile::AnimationTile(State::Context context, std::string l, std::string name, int frame_size, float frame_time)
	: context(context)
	, texture(context.contentManager->loadTexture("Content/Animations/" + name))
	, sprite(texture)
	, drawAtNight(false)
	{
		frameSwitchTime = frame_time;
		frameSize = frame_size;
		id = 0;
		frameX = 0;
		frameY = id;
		timer = 0;

		layer = l;

		bounds.width = frameSize;
		bounds.height = frameSize;
		bounds.left = frameX;
		bounds.top = frameY;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: Constructor for an animation
 * ----------------------------------------------------------------------
 */
	void AnimationTile::update(sf::Time elapsedTime)
	{
		if(isOnScreen(sprite, context.player -> getCamera()))
		{
			timer += elapsedTime.asSeconds();
			int maxFrames = texture.getSize().x/frameSize;

			if(timer >= frameSwitchTime)
			{
				timer = 0;
				if(frameX < maxFrames-1)
					frameX++;
				else
					frameX = 0;
			}

			frameY = id;
			bounds.top = frameY;
			bounds.left = frameX*frameSize;
			bounds.width = frameSize;
			bounds.height = frameSize;

			sprite.setTextureRect(bounds);
		}
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: Sets the id of the animation
 * ----------------------------------------------------------------------
 */
	void AnimationTile::setID(int i)
	{
		id = i;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: Sets the frame time of the animation
 * ----------------------------------------------------------------------
 */
	void AnimationTile::setFrameTime(float s)
	{
		frameSwitchTime = s;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: sets the size of the frame
 * ----------------------------------------------------------------------
 */
	void AnimationTile::setFrameSize(int s)
	{
		frameSize = s;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: draws the animation
 * ----------------------------------------------------------------------
 */
	void AnimationTile::render()
	{
		if(isOnScreen(sprite, context.player -> getCamera()))
			context.window->draw(sprite);
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: sets the position
 * ----------------------------------------------------------------------
 */
	void AnimationTile::setPosition(sf::Vector2f pos)
	{
		sprite.setPosition(pos);
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: Returns the sprite
 * ----------------------------------------------------------------------
 */
	sf::Sprite& AnimationTile::getSprite()
	{
		return sprite;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 28 March 2014
 * Description: Returns the layer of the sprite
 * ----------------------------------------------------------------------
 */
	std::string AnimationTile::getLayer()
	{
		return layer;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 27 july 2014
* Description: Sets whether the animation draws at night or not
* ----------------------------------------------------------------------
*/
	void AnimationTile::setDrawAtNight(bool state)
	{
		drawAtNight = state;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 27 july 2014
* Description: Gets whether the animation draws at night or not
* ----------------------------------------------------------------------
*/
	bool AnimationTile::getDrawAtNight()
	{
		return drawAtNight;
	}