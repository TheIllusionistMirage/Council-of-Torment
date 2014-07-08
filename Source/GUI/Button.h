#pragma once

#include "Component.h"
#include "../Contentmanager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>
#include <string>

namespace GUI
{
	/*	A button is a GUI element and can be very powerful
		when using with a GUI container. You can add as many
		buttons as you want and each button has it's own
		texture and text, as well as a unique callback
		function, which activates when the user clicks on it.
		Buttons are also toggleable.
	*/
	class Button : public Component
	{
		public:
			typedef std::shared_ptr<Button> Ptr;
			typedef std::function<void()> Callback;

		public:
			Button(const ContentManager& contentManager);

			void setCallback(Callback callback);
			void setText(const std::string& text);
			void setToggle(bool toggle);

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
			const sf::Texture& normalTexture;
			const sf::Texture& selectedTexture;
			const sf::Texture& pressedTexture;
			sf::Sprite sprite;
			sf::Text text;
			bool toggle;
	};
}