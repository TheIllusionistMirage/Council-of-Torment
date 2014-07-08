#include "DefocusState.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of defocus state
 * ----------------------------------------------------------------------
 */
DefocusState::DefocusState(StateStack& stack, Context context)
: State(stack, context)
, defocusText()
{
	// Get the font and the window size
	sf::Font& font = context.contentManager->getFont(Fonts::CLOISTER_BLACK);
	sf::Vector2f windowSize(context.window->getSize());

	// Create the defocus text
	defocusText.setFont(font);
	defocusText.setString("Game out of focus!");	
	defocusText.setCharacterSize(80);
	centerOrigin(defocusText);
	defocusText.setPosition(0.5f * windowSize.x, 0.45f * windowSize.y);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the state
 * ----------------------------------------------------------------------
 */
bool DefocusState::update(sf::Time elapsedTime)
{
	return false;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the state
 * ----------------------------------------------------------------------
 */
void DefocusState::render()
{
	// Get the window and set the default view
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create an background sprite to darken the screen
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	// Render everything
	window.draw(backgroundShape);
	window.draw(defocusText);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles the events of the state
 * ----------------------------------------------------------------------
 */
bool DefocusState::handleEvent(const sf::Event& windowEvent)
{
	return false;
}