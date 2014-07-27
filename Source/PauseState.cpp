#include "PauseState.h"
#include "GUI/Button.h"
#include <SFML/Graphics.hpp>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the pause state
 * ----------------------------------------------------------------------
 */
PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, container(context)
, pauseText()
{
	// Get the font and the window size
	sf::Font& font = context.contentManager->getFont(Fonts::FRENCH_GROTESQUE);
	sf::Vector2f windowSize(context.window->getSize());

	// Create the pause text
	pauseText.setFont(font);
	pauseText.setString("Game Paused");	
	pauseText.setCharacterSize(70);
	centerOrigin(pauseText);
	pauseText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	// Create the return button
	auto returnButton = std::make_shared<GUI::Button>(*context.contentManager);
	returnButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	returnButton->setText("Return");
	returnButton->setCallback([this] ()
	{
		requestStackPop();
	});

	// Create the back to menu button
	auto backToMenuButton = std::make_shared<GUI::Button>(*context.contentManager);
	backToMenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	backToMenuButton->setText("Back to menu");
	backToMenuButton->setCallback([this] ()
	{
		requestStateClear();
		requestStackPush(States::MENU);
		getContext().contentManager->playMusic(Music::NO_MUSIC);
	});

	// Add the buttons to the container
	container.add(returnButton);
	container.add(backToMenuButton);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the state
 * ----------------------------------------------------------------------
 */
bool PauseState::update(sf::Time elapsedTime)
{
	return false;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the state
 * ----------------------------------------------------------------------
 */
void PauseState::render()
{
	// Get the window and set the default view
	sf::RenderTexture& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create an background sprite to darken the screen
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	// Render everything
	window.draw(backgroundShape);
	window.draw(pauseText);
	window.draw(container);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events of the state
 * ----------------------------------------------------------------------
 */
bool PauseState::handleEvent(const sf::Event& windowEvent)
{
	// Let the container handle all events
	container.handleEvent(windowEvent);
	return false;
}