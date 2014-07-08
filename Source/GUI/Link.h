#pragma once

#include "Component.h"
#include <SFML/Graphics/Text.hpp>
#include <functional>

namespace GUI
{
	/*	A link is a GUI element and can be very powerful
		when using with a GUI container. You can add as many
		links as you want and each link has it's own text,
		as well as a unique callback function, which activates
		when the user clicks on it. Links are also toggleable.
	*/
	class Link : public Component
	{
		public:
			typedef std::shared_ptr<Link> Ptr;
			typedef std::function<void()> Callback;

		public:
			Link(const std::string& text, const sf::Font& font);

			void setCallback(Callback callback);
			virtual bool isSelectable() const;
			virtual void select();
			virtual void deselect();

			void setCharacterSize(unsigned int size);
			void setColor(const sf::Color& color);
			void setText(const std::string& text);

			virtual void activate();
			virtual void deactivate();
			virtual sf::FloatRect getBounds() const;
			virtual void handleEvent(const sf::Event& windowEvent);

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:
			Callback callback;
			sf::Color color;
			sf::Text text;
	};
}