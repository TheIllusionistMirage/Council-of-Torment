#include "TitleState.h"
#include <SFML/Graphics.hpp>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the title state
 * ----------------------------------------------------------------------
 */
TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, text()
, showText(true)
, textTime(sf::Time::Zero)
{
	// Load a background sprite for the title screen
	backgroundSprite.setTexture(context.contentManager->getTexture(Textures::TITLE_SCREEN));

	// Create some text and place it in the middle of the screen
	text.setFont(context.contentManager->getFont(Fonts::FRENCH_GROTESQUE));
	text.setString("Press any key to start");
	centerOrigin(text);
	text.setPosition(context.window->getView().getSize() / 2.0f);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the state
 * ----------------------------------------------------------------------
 */
bool TitleState::update(sf::Time elapsedTime)
{
	// Add the elapsedTime to a trigger time
	textTime += elapsedTime;

	// Let the text blink
	if(textTime >= sf::seconds(0.4f))
	{
		showText = !showText;
		textTime = sf::Time::Zero;
	}

	return true;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the state
 * ----------------------------------------------------------------------
 */
void TitleState::render()
{
	// Get the window
	sf::RenderTexture& window = *getContext().window;

	// Render the background and the text
	window.draw(backgroundSprite);
	if(showText)
		window.draw(text);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events of the state
 * ----------------------------------------------------------------------
 */
bool TitleState::handleEvent(const sf::Event& windowEvent)
{
	// If the user presses any key it pops the title state and pushes the menu state
	if(windowEvent.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::MENU);
	}

	return true;
}