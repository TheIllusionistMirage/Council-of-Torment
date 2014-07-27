#include "MenuState.h"
#include "GUI/Link.h"
#include "Game.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the menu state
 * ----------------------------------------------------------------------
 */
MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, container(context)
, shader(context.contentManager->getShader(Shaders::MENU))
, textLogo(context.contentManager->getTexture(Textures::TEXT_LOGO))
, shaderShape(sf::Vector2f(340, float(context.window->getSize().y)))
{
	// Add all the components for the GUI
	addComponents(context);

	// Set the position of the text logo and the shader shape and some parameters for the shader
	textLogo.setPosition(context.window->getSize().x - 300.0f, 50);
	shaderShape.setPosition(context.window->getSize().x - shaderShape.getLocalBounds().width, 0);
	shaderShape.setFillColor(sf::Color::Black);
	shader.setParameter("leftPos", shaderShape.getPosition().x);
	shader.setParameter("width", 40);

	// Change the state
	Game::currentState = States::MENU;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the state
 * ----------------------------------------------------------------------
 */
bool MenuState::update(sf::Time elapsedTime)
{
	return true;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the state
 * ----------------------------------------------------------------------
 */
void MenuState::render()
{
	// Get the window and sets the default view
	sf::RenderTexture& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Render the menu
	window.draw(shaderShape, &shader);
	window.draw(textLogo);
	window.draw(container);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events of the state
 * ----------------------------------------------------------------------
 */
bool MenuState::handleEvent(const sf::Event& windowEvent)
{
	// Let the container handle all events
	container.handleEvent(windowEvent);

	return false;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Adds all components to the container
 * ----------------------------------------------------------------------
 */
void MenuState::addComponents(Context context)
{
	// Add a play link
	auto playLink = std::make_shared<GUI::Link>("Journey Onward", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	playLink->setPosition(context.window->getSize().x - 250.0f, context.window->getSize().y - 400.0f);
	playLink->setColor(sf::Color(100, 40, 20));
	playLink->setCharacterSize(30);
	playLink->setCallback([this] ()
	{
		// Start the game
		getContext().effectManager->add(Effect(EffectType::FADE));
		getContext().effectManager->getEffectList().back().setCallback([this] ()
		{
			requestStackPop();
			requestStackPush(States::GAME);
		});
	});

	// Add a new character link
	auto newCharacterLink = std::make_shared<GUI::Link>("New Character", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	newCharacterLink->setPosition(context.window->getSize().x - 250.0f, context.window->getSize().y - 370.0f);
	newCharacterLink->setColor(sf::Color(100, 40, 20));
	newCharacterLink->setCharacterSize(30);
	newCharacterLink->setCallback([this] ()
	{
	});

	// Add an options link
	auto optionLink = std::make_shared<GUI::Link>("Options", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	optionLink->setPosition(context.window->getSize().x - 250.0f, context.window->getSize().y - 340.0f);
	optionLink->setColor(sf::Color(100, 40, 20));
	optionLink->setCharacterSize(30);
	optionLink->setCallback([this] ()
	{
		// Push the settings state
		requestStackPop();
		requestStackPush(States::SETTINGS);
	});

	// Add a credit link
	auto creditLink = std::make_shared<GUI::Link>("Credits", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	creditLink->setPosition(context.window->getSize().x - 250.0f, context.window->getSize().y - 310.0f);
	creditLink->setColor(sf::Color(100, 40, 20));
	creditLink->setCharacterSize(30);
	creditLink->setCallback([this] ()
	{
	});

	// Add an exit link
	auto exitLink = std::make_shared<GUI::Link>("Exit Game", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	exitLink->setPosition(context.window->getSize().x - 250.0f, context.window->getSize().y - 280.0f);
	exitLink->setColor(sf::Color(100, 40, 20));
	exitLink->setCharacterSize(30);
	exitLink->setCallback([this] ()
	{
		// Close the game
		requestStackPop();
	});

	// Add a multiplayer link
	auto multiplayerLink = std::make_shared<GUI::Link>("Join Multiplayer", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	multiplayerLink->setPosition(context.window->getSize().x - 250.0f, context.window->getSize().y - 150.0f);
	multiplayerLink->setColor(sf::Color(100, 40, 20));
	multiplayerLink->setCharacterSize(30);
	multiplayerLink->setCallback([this] ()
	{
	});

	// Add all the links to the container
	container.add(playLink);
	container.add(newCharacterLink);
	container.add(optionLink);
	container.add(creditLink);
	container.add(exitLink);
	container.add(multiplayerLink);
}