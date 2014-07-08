#pragma once

#include "State.h"
#include <SFML/System/NonCopyable.hpp>
#include <functional>
#include <vector>
#include <map>

/*	The state stack registers all states, handles them,
	destroys them, adds new states...
	It is a very powerful class and is only used once
	in the game class.
*/
class StateStack : private sf::NonCopyable
{
	public:
		enum Action
		{
			Push,
			Pop,
			Clear
		};

	public:
		explicit StateStack(State::Context context);

		template<typename T>
		void registerState(States::ID stateID);

		void update(sf::Time elapsedTime);
		void render();
		void handleEvent(const sf::Event& windowEvent);

		void pushState(States::ID stateID);
		void popState();
		void clearStates();

		bool isEmpty() const;
		std::size_t getSize() const;
		State::Context getContext() const;

	private:
		struct PendingChange
		{
			explicit PendingChange(Action action, States::ID stateID = States::NONE);
			Action action;
			States::ID stateID;
		};

	private:
		State::Ptr createState(States::ID stateID);
		void applyPendingChanges();

		std::vector<State::Ptr> stack;
		std::vector<PendingChange> pendingList;

		State::Context context;
		std::map<States::ID, std::function<State::Ptr()>> factories;
};

// Register a state using a lambda expression
template<typename T>
void StateStack::registerState(States::ID stateID)
{
	factories[stateID] = [this] ()
	{
		return State::Ptr(new T(*this, context));
	};
}