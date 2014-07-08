#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace GUI
{
	/*	A Component is the basis element for every GUI element.
		It inherits from sf::Drawable, sf::Transformable and
		sf::NonCopyable, which makes it very easy to use it.
		It also has some basic attributes, which will be used
		in all GUI elements, nothing special.
	*/
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
		public:
			typedef std::shared_ptr<Component> Ptr;

		public:
			Component();
			virtual ~Component();

			virtual sf::FloatRect getBounds() const;
			virtual bool isSelectable() const = 0;
			bool isSelected() const;
			virtual void select();
			virtual void deselect();

			virtual bool isActive() const;
			virtual void activate();
			virtual void deactivate();
			virtual void handleEvent(const sf::Event& windowEvent) = 0;

		private:
			bool selected;
			bool active;
	};
}