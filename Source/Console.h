#pragma once

#include "State.h"
#include "GUI/Button.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <sstream>
#include <vector>
#include <string>

/*	The console is a class that encapsulates all the functionality
	of the regular Visual-Studio console into an in-game container
	which is capable of logging text messages or accepting commands.
	The reason I made an in-game console instead of using the
	default windows console is because text buffer can be slower
	than displaying a sprite on the screen, and doesn't accept commands
*/
class Console : public sf::Drawable
{
	public:
		Console(State::Context context);

		void hide();
		void show();
		void update(sf::Time elapsedTime);
		void handleEvent(const sf::Event& windowEvent);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void log(std::string text, sf::Color color = sf::Color::White);
		void newCommand(std::string command, std::string values);
		void logWarning(std::string text);
		void logError(std::string text);
		void logInfo(std::string text);
		void setEntryText(std::string s);
		void submitCommand();

		bool isVisible() const;

	private:
		State::Context context;
		sf::Vector2f dragCoordinates;
		sf::Vector2f scrollCoordinates;

		std::vector<sf::Text> commandList;
		std::vector<sf::Text>::iterator iCommand;
		std::vector<sf::Text> consoleTextList;
		std::vector<sf::Text> consoleStrings;
		sf::RectangleShape consoleWindow;
		sf::RectangleShape scrollBar[2];
		sf::RectangleShape moveBar;
		sf::VertexArray line;

		unsigned int characterSize;
		int currentAmount;
		int scrollIndex;
		float minBarHeight;
		float scrollY;
		bool flag_moved;
		bool moveScroll;
		bool isMoving;
		bool visible;

		void clearAll();
		void setStrings();
		void moveCommands();
		void updateScroll();
		void adjustScrollHeight();
		void handleText(const sf::Event& windowEvent);
		void getValues(std::vector<std::string>& valueList, std::string& values, std::string command);
		bool checkMouseCollision(sf::Vector2f pos);
		std::string getSystemTime();
};