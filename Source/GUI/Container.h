#pragma once

#include "Component.h"
#include "../State.h"
#include <vector>
#include <memory>

namespace GUI
{
	/*	A Container can hold as many GUI elements as the
		user wants it to hold. It handles all components:
		selects them, activates them, updates them, handles
		all events for them and renders them.
	*/
	class Container : public Component
	{
		public:
			typedef std::shared_ptr<Container> Ptr;

		public:
			Container(State::Context context);

			void add(Component::Ptr component);
			virtual bool isSelectable() const;
			virtual void handleEvent(const sf::Event& windowEvent);

		private:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			bool hasSelection() const;
			void select(std::size_t index);
			void selectNext();
			void selectPrevious();

		private:
			State::Context context;
			std::vector<Component::Ptr> childrenList;
			int selectedChild;
	};
}