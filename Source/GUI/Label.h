#pragma once

#include "Component.h"
#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	/*	A label is a GUI element and can be very powerful
		when using with a GUI container. You can add as many
		labels as you want and each label has it's own text.
		Labels are the simplest elements in the GUI.
	*/
	class Label : public Component
	{
		public:
			typedef std::shared_ptr<Label> Ptr;

		public:
			Label(const std::string& text, const sf::Font& font, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
			Label();

			void setText(const std::string& text);
			void setColor(const sf::Color& color);
			void setCharacterSize(unsigned int size);

			virtual bool isSelectable() const;
			virtual sf::FloatRect getBounds() const;
			virtual void handleEvent(const sf::Event& windowEvent);

			sf::Text& getText() { return text; }

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:
			sf::Color color;
			sf::Text text;
	};
}