#pragma once

#include "Component.h"
#include "../Contentmanager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>
#include <string>

namespace GUI
{
	/*	A checkbox is a GUI element and can be very powerful
		when using with a GUI container. You can add as many
		checkboxes as you want and each checkbox has it's own
		texture, as well as a unique callback function, which
		activates when the user clicks on it.
		Checkboxes can be toggled on or off.
	*/
	class Checkbox : public Component
	{
		public:
			typedef std::shared_ptr<Checkbox> Ptr;
			typedef std::function<void()> Callback;

		public:
			Checkbox(const sf::Texture& texture, int addHeight);

			void setChecked(bool checked);
			void setCallback(Callback callback);
			virtual bool isSelectable() const;
			virtual void select();
			virtual void deselect();

			virtual void activate();
			virtual void deactivate();
			virtual sf::FloatRect getBounds() const;
			virtual void handleEvent(const sf::Event& windowEvent);

		private:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:
			Callback callback;
			const sf::Texture& texture;
			sf::Sprite checkedSprite;
			sf::Sprite boxSprite;
			int addHeight;
			bool checked;
	};
}