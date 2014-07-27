#include "Console.h"
#include "Game.h"
#include "LivingEntity.h"

#include <algorithm>
#include <string>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Constructor of the console class
 * ----------------------------------------------------------------------
 */
Console::Console(State::Context context)
: context(context)
, scrollCoordinates(0, 0)
, dragCoordinates(0, 0)
, minBarHeight(10.0f)
, characterSize(13)
, currentAmount(-1)
, scrollIndex(0)
, scrollY(0.0f)
, visible(false)
, isMoving(false)
, flag_moved(false)
, moveScroll(false)
{
	// Creates the console window
	consoleWindow = sf::RectangleShape(sf::Vector2f(600, 405));
	consoleWindow.setFillColor(sf::Color(35, 35, 35));
	consoleWindow.setOutlineThickness(1);
	consoleWindow.setOutlineColor(sf::Color(51, 51, 51));
	consoleWindow.setPosition(sf::Vector2f(200, 100));

	// Creates the moveBar
	moveBar = sf::RectangleShape(sf::Vector2f(consoleWindow.getSize().x, 20.0f));
	moveBar.setFillColor(sf::Color(30, 30, 30));
	moveBar.setPosition(consoleWindow.getPosition());

	// Make the line
	line = sf::VertexArray(sf::Lines, 2);
	line[0] = sf::Vertex(sf::Vector2f(consoleWindow.getPosition().x, consoleWindow.getPosition().y + consoleWindow.getSize().y - characterSize - 10));
	line[1] = sf::Vertex(sf::Vector2f(consoleWindow.getPosition().x + consoleWindow.getSize().x, consoleWindow.getPosition().y + consoleWindow.getSize().y - characterSize - 10));
	line[0].color = sf::Color(51, 51, 51);
	line[1].color = sf::Color(51, 51, 51);

	// Creates the standard texts for the console!
	sf::Text tempText("", context.contentManager->getFont(Fonts::ARIAL), characterSize);
	tempText.setPosition(sf::Vector2f(consoleWindow.getPosition().x + 20, consoleWindow.getPosition().y + consoleWindow.getSize().y - characterSize - 6));
	consoleTextList.push_back(tempText);

	tempText.setString("Console");
	tempText.setPosition(sf::Vector2f(consoleWindow.getPosition().x + 5, consoleWindow.getPosition().y + 2));
	consoleTextList.push_back(tempText);

	tempText.setPosition(sf::Vector2f(consoleWindow.getPosition().x + 5, consoleWindow.getPosition().y + consoleWindow.getSize().y - characterSize - 6));
	tempText.setString(">");
	tempText.setColor(sf::Color(91, 91, 91));
	consoleTextList.push_back(tempText);

	// Creates the scrollbars
	scrollBar[1] = sf::RectangleShape(sf::Vector2f(16.0f, consoleWindow.getSize().y - moveBar.getSize().y - characterSize - 10));
	scrollBar[1].setFillColor(sf::Color(80, 80, 80));
	scrollBar[1].setPosition(consoleWindow.getPosition().x + consoleWindow.getGlobalBounds().width - scrollBar[1].getGlobalBounds().width - 2, consoleWindow.getPosition().y + moveBar.getSize().y);
	scrollBar[0] = sf::RectangleShape(sf::Vector2f(12.0f, scrollBar[1].getSize().y));
	scrollBar[0].setFillColor(sf::Color(200, 200, 200));
	scrollBar[0].setPosition(scrollBar[1].getPosition().x + 2, scrollBar[1].getPosition().y);

	updateScroll();

	iCommand = commandList.begin();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Updates the console
 * ----------------------------------------------------------------------
 */
void Console::update(sf::Time elapsedTime)
{
	// Gets the mousePosition
	const float mouseX = static_cast<float>(sf::Mouse::getPosition(*context.renderWindow).x);
	const float mouseY = static_cast<float>(sf::Mouse::getPosition(*context.renderWindow).y);

	// Set the color of the scrollbar
	scrollBar[0].setFillColor(sf::Color(120, 120, 120));
	if(scrollBar[0].getGlobalBounds().contains(mouseX, mouseY))
		scrollBar[0].setFillColor(sf::Color(150, 150, 150));

	// Save the position of the console once and let the console move
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && moveBar.getGlobalBounds().contains(mouseX, mouseY) && !flag_moved)
	{
		isMoving = true;
		dragCoordinates.x = mouseX - moveBar.getPosition().x;
		dragCoordinates.y = mouseY - moveBar.getPosition().y;
	}

	// Save the position of the scrollBar once and let the console move
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && scrollBar[0].getGlobalBounds().contains(mouseX, mouseY) && !flag_moved)
	{
		moveScroll = true;
		scrollCoordinates.x = mouseX - scrollBar[0].getPosition().x;
		scrollCoordinates.y = mouseY - scrollBar[0].getPosition().y;
	}

	// Move the console
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMoving)
	{
		// Sets the position of the console
		consoleWindow.setPosition(mouseX - dragCoordinates.x, mouseY - dragCoordinates.y);

		// Adjust the position, if it goes out of screen
		if(consoleWindow.getPosition().x < 0)
			consoleWindow.setPosition(0, consoleWindow.getPosition().y);
		if(consoleWindow.getPosition().y < 0)
			consoleWindow.setPosition(consoleWindow.getPosition().x, 0);
		if(consoleWindow.getPosition().x + consoleWindow.getSize().x > context.window->getSize().x)
			consoleWindow.setPosition(context.window->getSize().x - consoleWindow.getSize().x, consoleWindow.getPosition().y);
		if(consoleWindow.getPosition().y + consoleWindow.getSize().y > context.window->getSize().y)
			consoleWindow.setPosition(consoleWindow.getPosition().x, context.window->getSize().y - consoleWindow.getSize().y);

		moveBar.setPosition(consoleWindow.getPosition());

		// The position of the console
		const float x = consoleWindow.getPosition().x;
		const float y = consoleWindow.getPosition().y;

		// Sets the position of the texts
		consoleTextList[0].setPosition(sf::Vector2f(x + 20, y + consoleWindow.getSize().y - characterSize - 6));
		consoleTextList[1].setPosition(sf::Vector2f(x + 5, y + 2));
		consoleTextList[2].setPosition(sf::Vector2f(x + 5, y + consoleWindow.getSize().y - characterSize - 6));

		// Keep the line's position
		line[0].position = sf::Vector2f(consoleWindow.getPosition().x, consoleWindow.getPosition().y + consoleWindow.getSize().y - characterSize - 10);
		line[1].position = sf::Vector2f(consoleWindow.getPosition().x + consoleWindow.getSize().x, consoleWindow.getPosition().y + consoleWindow.getSize().y - characterSize - 10);

		// Set the position of the scrollbar
		scrollBar[1].setPosition(consoleWindow.getPosition().x + consoleWindow.getGlobalBounds().width - scrollBar[1].getGlobalBounds().width - 2, consoleWindow.getPosition().y + moveBar.getSize().y);
		scrollBar[0].setPosition(scrollBar[1].getPosition().x + 2, scrollBar[1].getPosition().y + scrollY);

		// Moves the commands
		moveCommands();
	}

	// Move the scrollbar
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && moveScroll)
	{
		// Sets the color of the scrollbar
		scrollBar[0].setFillColor(sf::Color(200, 200, 200));

		// Moves it and checks if it is outside
		scrollBar[0].setPosition(scrollBar[0].getPosition().x, mouseY - scrollCoordinates.y);

		// Update scroll values
		updateScroll();
	}

	// Set the bools
	flag_moved = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		isMoving = false;
		moveScroll = false;
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Renders everything from the console
 * ----------------------------------------------------------------------
 */
void Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(visible)
	{
		target.draw(consoleWindow);
		target.draw(scrollBar[1]);
		target.draw(scrollBar[0]);
		target.draw(moveBar);
		target.draw(line);

		for(auto&& text : consoleTextList)
			target.draw(text);

		for(auto&& strings : consoleStrings)
			if(strings.getPosition().y - characterSize > moveBar.getPosition().y && strings.getPosition().y + characterSize < line[0].position.y)
				target.draw(strings);
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Updates the scroll bar
 * ----------------------------------------------------------------------
 */
void Console::updateScroll()
{
	if(scrollBar[0].getPosition().y < scrollBar[1].getPosition().y)
		scrollBar[0].setPosition(scrollBar[0].getPosition().x, scrollBar[1].getPosition().y);

	if(scrollBar[0].getPosition().y + scrollBar[0].getSize().y > scrollBar[1].getPosition().y + scrollBar[1].getSize().y)
		scrollBar[0].setPosition(scrollBar[0].getPosition().x, scrollBar[1].getPosition().y + scrollBar[1].getSize().y - scrollBar[0].getSize().y);

	// scrollY is needed to keep the position where it should be ;)
	scrollY = scrollBar[0].getPosition().y - scrollBar[1].getPosition().y;

	// Sets the index of the text, where the scrollbar is atm
	scrollIndex = static_cast<int>(scrollBar[1].getPosition().y - scrollBar[0].getPosition().y + currentAmount);
	if(scrollIndex < 0)
		scrollIndex = 0;

	// Moves the commands
	moveCommands();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Submits a command
 * ----------------------------------------------------------------------
 */
void Console::submitCommand()
{
	// Adds the text to the console!
	log("> " + consoleTextList[0].getString());

	// Adds the command to the commandList
	commandList.push_back(consoleTextList[0]);
	iCommand = commandList.end();

	// Check for commands (and maybe a values)
	std::string inputLine = consoleTextList[0].getString();
	size_t pos = inputLine.find(' ', 0);

	// The two strings, one for the command and one for the value
	std::string command = inputLine.substr(0, pos);
	std::string values = inputLine.substr(pos + 1);

	// Searches for the right command and triggers it
	newCommand(command, values);

	// Clears the text ;)
	consoleTextList[0].setString("");

	// Sets the scrollbar down again ;)
	scrollBar[0].setPosition(scrollBar[0].getPosition().x, line[0].position.y - scrollBar[0].getGlobalBounds().height);
	updateScroll();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Moves the commands if the user wrote a new one
 * ----------------------------------------------------------------------
 */
void Console::moveCommands()
{
	// Update the messages which are logged
	if(!consoleStrings.empty())
	{
		consoleStrings[consoleStrings.size()-1].setPosition(sf::Vector2f(line[0].position.x + 5, line[0].position.y - characterSize - 5));

		for(int i = consoleStrings.size() - 2 ; i >= 0 ; --i)
			consoleStrings[i].setPosition(consoleStrings[i+1].getPosition().x, consoleStrings[i+1].getPosition().y - characterSize - 5);

		// Moves all texts up/down
		for(int i = 0; i != consoleStrings.size(); ++i)
			consoleStrings[i].setPosition(consoleStrings[i].getPosition().x, consoleStrings[i].getPosition().y + scrollIndex * (characterSize + 5));
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Clears all commands
 * ----------------------------------------------------------------------
 */
void Console::clearAll()
{
	currentAmount = -1;
	consoleStrings.clear();
	commandList.clear();
	scrollBar[0].setSize(sf::Vector2f(scrollBar[0].getSize().x, scrollBar[1].getSize().y));
	updateScroll();

	log("Cleared the console.", success_color);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Gets the current system time
 * ----------------------------------------------------------------------
 */
std::string Console::getSystemTime()
{
	std::string actualTime = "";

	const time_t myTime = time(NULL);
	struct tm buffer;
	char timeStamp[9];

	errno_t err = localtime_s(&buffer, &myTime);

	if(!err)
	{
		std::stringstream tempStream;
		strftime(timeStamp, sizeof(timeStamp), "%H:%M:%S", &buffer);
		tempStream << "[" << timeStamp << "]";
		actualTime = tempStream.str();
	}

	return actualTime;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Logs a command
 * ----------------------------------------------------------------------
 */
void Console::log(std::string text, sf::Color color)
{
	std::string finalstr = getSystemTime() + text;

	sf::Text message(finalstr, context.contentManager->getFont(Fonts::SEGOEUI), characterSize);
	message.setColor(color);
	consoleStrings.push_back(message);

	// Adjust the scrollbar
	adjustScrollHeight();

	// Moves the commands
	moveCommands();

	// Open the console
	//visible = true;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Adjusts the height of the scroll bar
 * ----------------------------------------------------------------------
 */
void Console::adjustScrollHeight()
{
	// Resize the scrollbar
	++currentAmount;
	if(currentAmount != 0)
	{
		scrollBar[0].setSize(sf::Vector2f(scrollBar[0].getSize().x, scrollBar[0].getSize().y - 1));
		scrollBar[0].move(0, 1);
	}
	if(scrollBar[0].getSize().y <  minBarHeight)
	{
		scrollBar[0].setSize(sf::Vector2f(scrollBar[0].getSize().x, minBarHeight));
		consoleStrings.erase(consoleStrings.begin());
		--currentAmount;
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Handles the text the user enters
 * ----------------------------------------------------------------------
 */
void Console::handleText(const sf::Event& windowEvent)
{
	// If you pressed return it will send the message, otherwise keep writing
	if(windowEvent.text.unicode == 13)
	{
		// You can't send an empty command!
		if(!consoleTextList[0].getString().isEmpty())
			submitCommand();
	}
	else if(windowEvent.text.unicode == 8)
	{
		// Erases a sign
		if(!consoleTextList[0].getString().isEmpty())
		{
			std::string temp = consoleTextList[0].getString().toAnsiString();
			temp.erase(temp.size() - 1, 1);
			consoleTextList[0].setString(temp);
		}
	}
	else if(windowEvent.text.unicode != 96)
		consoleTextList[0].setString(consoleTextList[0].getString() + windowEvent.text.unicode);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Handles all the events
 * ----------------------------------------------------------------------
 */
void Console::handleEvent(const sf::Event& windowEvent)
{
	// Switches between the events
	switch(windowEvent.type)
	{
		case(sf::Event::KeyPressed):

			// Changes the visibility of the console
			if(windowEvent.key.code == sf::Keyboard::Tilde || windowEvent.key.code == sf::Keyboard::F1)
				visible = !visible;
			else if(windowEvent.key.code == sf::Keyboard::Up)
			{
				if(!commandList.empty())
				{
					if(iCommand != commandList.begin())
						--iCommand;
					consoleTextList[0] = (*iCommand);
					const float x = consoleWindow.getPosition().x;
					const float y = consoleWindow.getPosition().y;
					consoleTextList[0].setPosition(sf::Vector2f(x + 20, y + consoleWindow.getSize().y - characterSize - 6));
				}
			}
			else if(windowEvent.key.code == sf::Keyboard::Down)
			{
				if(!commandList.empty() && iCommand != commandList.end())
				{
					if(iCommand != commandList.end() - 1)
					{
						++iCommand;
						consoleTextList[0] = (*iCommand);
					}
					else
					{
						consoleTextList[0].setString("");
						iCommand = commandList.end();
					}
					const float x = consoleWindow.getPosition().x;
					const float y = consoleWindow.getPosition().y;
					consoleTextList[0].setPosition(sf::Vector2f(x + 20, y + consoleWindow.getSize().y - characterSize - 6));
				}
			}
		break;


		case(sf::Event::TextEntered):

			// Writes a command
			if(visible && windowEvent.text.unicode < 128)
				handleText(windowEvent); 
		break;


		case(sf::Event::MouseWheelMoved):

			// Depending on the mousewheel, it moves up or down
			if(windowEvent.mouseWheel.delta > 0)
			{
				scrollBar[0].move(0, -3.0f);
				updateScroll();
			}
			if(windowEvent.mouseWheel.delta < 0)
			{
				scrollBar[0].move(0, 3.0f);
				updateScroll();
			}
		break;
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Adds the values of a command to a list
 * ----------------------------------------------------------------------
 */
void Console::getValues(std::vector<std::string>& valueList, std::string& values, std::string command)
{
	// Check for a dobble point
	std::string::size_type pos = values.find(':', 0);

	// Push the first value into the list and continue searching
	if(pos != std::string::npos)
	{
		valueList.push_back(std::string(values.substr(0, pos)));
		values = values.substr(pos + 1);
		getValues(valueList, values, command);
	}
	else
		if(command != values)
			valueList.push_back(std::string(values.substr(0, pos)));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Returns true if the console is visible
 * ----------------------------------------------------------------------
 */
bool Console::isVisible() const
{
	return visible;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Logs a warning
 * ----------------------------------------------------------------------
 */
void Console::logWarning(std::string text)
{
	log(" [Warning]: " + text, warning_color);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Logs an info
 * ----------------------------------------------------------------------
 */
void Console::logInfo(std::string text)
{
	log(" [Info]: " + text, sf::Color::White);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Logs an error and opens the console
 * ----------------------------------------------------------------------
 */
void Console::logError(std::string text)
{
	log(" [Error]: " + text, error_color);
	visible = true;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Handles the commands
 * ----------------------------------------------------------------------
 */
void Console::newCommand(std::string command, std::string values)
{
	std::transform(command.begin(), command.end(), command.begin(), ::tolower);
	std::vector<std::string> valueList;
	getValues(valueList, values, command);

//==========================================================================================================================
	if(command == "clear_console" ||										// clear_console
		command == "clean_console")											// clean_console
		clearAll();

//==========================================================================================================================
	else if(command == "exit")												// exit
		context.renderWindow->close();
//==========================================================================================================================
	else if(command == "player_set_position")								// player_set_position x:y
	{
		if(valueList.size() == 2)
		{
			const float xPos = stof(valueList[0]);
			const float yPos = stof(valueList[1]);

			context.player->setPosition(xPos, yPos);

			logInfo("Changed Player position to " + valueList[0] + " : " + valueList[1]);
		}
		else
			logError("Syntax is: player_set_position x:y");
	}
//==========================================================================================================================
	else if(command == "player_set_speed")									// player_set_animation value
	{
		if(valueList.size() == 1)
		{
			context.player->setDefaultSpeed(stoi(valueList[0]));
			logInfo("Changed Player velocity to: " + valueList[0]);
		}
		else
			logError("Syntax is: player_set_speed value");
	}
//==========================================================================================================================
	else if(command == "player_play_animation")								// player_play_animation value
	{
		if(valueList.size() == 1 && stoi(valueList[0]) <= TOTAL_ANIMATIONS)
			context.player->playAnimationOnce(stoi(valueList[0]));
		else if(valueList.size() == 1 && stoi(valueList[0]) == TOTAL_ANIMATIONS)
			logError("Your value is too large.");
		else
			logError("Syntax is: player_set_animation value");
	}
//==========================================================================================================================
	else if(command == "player_set_health")									// player_set_health value
	{
		if(valueList.size() == 1)
			context.player->setHealth(stoi(valueList[0]));
		else
			logError("Syntax is: player_set_health value");
	}
//==========================================================================================================================
	else if(command == "show_collision")									// show_collision value
	{
		if(valueList.size() == 1)
			context.player->showCollision(stoi(valueList[0]) !=0); // The !=0 is there in order to prevent a warning, ignore it
		else
			logError("Syntax is: show_collision value (1 or 0)");
	}
//==========================================================================================================================
	else if(command == "player_set_mana")									// player_set_mana value
	{
		if(valueList.size() == 1)
			context.player->setMana(stoi(valueList[0]));
		else
			logError("Syntax is: player_set_mana value");
	}
//==========================================================================================================================
	else if(command == "player_get_position")								// player_get_position
	{
		std::stringstream s, s2;

		s << context.player->getPosition().x / TILE_SIZE;
		s2 << (context.player->getPosition().y - TILE_SIZE/2) / TILE_SIZE;

		logInfo("You are at " + s.str() + ":" + s2.str());
	}
//==========================================================================================================================
	else if(command == "GFX_add" || command == "gfx_add")					// GFX_add
	{
		if(valueList.size() == 1)
		{
			if(valueList[0] == "fade")
				context.effectManager->add(Effect(EffectType::FADE));
		}
		else
			logError("Syntax is: GFX_add value (fade for example)");
	}
//==========================================================================================================================
	else if(command == "time_set")											// time_set
	{
		if(valueList.size() == 3)
		{
			context.globalTime->setTime(std::stoi(valueList[0]), std::stoi(valueList[1]), std::stof(valueList[2]));
			std::stringstream stream;
			stream<<"Current time: "<<context.globalTime->getHours()<<":"<<context.globalTime->getMinutes()<<":"<<int(context.globalTime->getSeconds());
			logInfo(stream.str());
		}
		else
			logError("Syntax is: time_set hours:minutes:seconds");
	}
//==========================================================================================================================
	else if(command == "print")											// print
	{
		if(valueList.size() == 1)
			logInfo(valueList[0]);
		else
			logError("Syntax is: print message");
	}
//==========================================================================================================================
	else if(command == "print_time")											// print_time
	{
		if(valueList.size() == 0)
		{
			std::stringstream stream;
			stream<<"Current time: "<<context.globalTime->getHours()<<":"<<context.globalTime->getMinutes()<<":"<<int(context.globalTime->getSeconds());
			logInfo(stream.str());
		}
		else
			logError("Syntax is: print_time");
	}

//==========================================================================================================================
	else if(command == "help")											// help
	{
		log(" clean_console\tclear_console\texit\tGFX_add <string>", {230, 200, 120});
		log(" gfx_add <string>\titem_add <int>\titem_add <int:int>\tmap_load <string>", {230, 200, 120});
		log(" map_reload\tplayer_get_position\tplayer_play_animation <int>", {230, 200, 120});
		log(" player_set_health <int>\tplayer_set_mana <int>\tplayer_set_position <float:float>", {230, 200, 120});
		log(" player_set_speed <int>\tplayer_set_sprite <string>\tprint <string>\tprint_time", {230, 200, 120});
		log(" show_collision <int>\ttime_set <int:int:int>\ttoggle_gui", {230, 200, 120});
	}
//==========================================================================================================================
	else if (command == "toggle_gui")											// toggle_gui
	{
		if (valueList.size() == 0)
		{
			context.player->setProperty("hideGUI", !context.player->getProperty("hideGUI"));
		}
		else
			logError("Syntax is: toggle_gui");
	}
//==========================================================================================================================
	else if(command == "map_load")											// map_load name
	{
		if(valueList.size() == 1)
		{
			const std::string mapName = valueList[0] + ".tmx";
			context.gameMap->loadMap(mapName);
		}
		else
			logError("Syntax is: map_load name");
	}
//==========================================================================================================================
	else if(command == "map_reload")										// map_reload
	{
		if(valueList.size() == 0)
		{
			context.gameMap->reload();
			logInfo("Updated the map with the latest changes.");
		}

		else
			logError("Syntax is: map_reload");
	}

//==========================================================================================================================
	else if(command == "player_set_sprite")								// player_set_sprite
	{
		if(valueList.size() == 1)
			context.player->changeSprite(valueList[0]);
		else
			logError("Syntax is: player_set_sprite name");
	}
//==========================================================================================================================
	else if(command == "item_add")										// item_add ID
	{
		if(valueList.size() == 2)
			context.player->getInventory().addItem(ItemID(stoi(valueList[0])), stoi(valueList[1]));

		else if(valueList.size() == 1)
			context.player->getInventory().addItem(ItemID(stoi(valueList[0])));

		else
			logError("Syntax is: item_add ID AMMOUNT (amount is optional)");
	}
//==========================================================================================================================
	// No command is correct
	else
		logError("Unknown command.");
}


void Console::setEntryText(std::string s)
{
	consoleTextList[0].setString(s);
}