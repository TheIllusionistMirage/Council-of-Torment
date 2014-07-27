#include "SettingsState.h"
#include "Contentmanager.h"
#include "Game.h"
#include "GUI/Link.h"
#include "GUI/Checkbox.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <fstream>
#include <sstream>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the settings state
 * ----------------------------------------------------------------------
 */
SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, container(context)
, modeList(sf::VideoMode::getFullscreenModes())
, shader(context.contentManager->getShader(Shaders::MENU))
, textLogo(context.contentManager->getTexture(Textures::TEXT_LOGO))
, shaderShape(sf::Vector2f(340, float(context.window->getSize().y)))
, bookBackground(context.contentManager->getTexture(Textures::OPTIONS_BOOK))
, currentMode(modeList.size() - 1)
, currentStyle(sf::Style::Close)
, resolutionText()
{
	// Set the current video mode and create the resolution text
	setCurrentMode();

	// Set some settings for the book background
	bookBackground.setTextureRect(sf::IntRect(0, 0, 63, 47));
	bookBackground.setScale(9.0f, 8.5f);
	bookBackground.setPosition(float(int(context.window->getSize().x / 2.0f - bookBackground.getGlobalBounds().width / 2.0f)),
		float(int(context.window->getSize().y / 2.0f - bookBackground.getGlobalBounds().height / 2.0f)));

	// Add all the components for the GUI
	addComponents(context);

	// Set the position of the text logo and the shader shape and some parameters for the shader
	textLogo.setPosition(context.window->getSize().x - 280.0f, 20);
	shaderShape.setPosition(context.window->getSize().x - shaderShape.getLocalBounds().width, 0);
	shaderShape.setFillColor(sf::Color::Black);
	shader.setParameter("leftPos", shaderShape.getPosition().x);
	shader.setParameter("width", 40);

	// Change the state
	Game::currentState = States::SETTINGS;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the state
 * ----------------------------------------------------------------------
 */
bool SettingsState::update(sf::Time elapsedTime)
{
	return true;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the state
 * ----------------------------------------------------------------------
 */
void SettingsState::render()
{
	// Gets the window and sets the default view
	sf::RenderTexture& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Render everything
	window.draw(shaderShape, &shader);
	window.draw(bookBackground);
	window.draw(textLogo);
	window.draw(resolutionText);
	window.draw(container);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Handles all the events of the state
 * ----------------------------------------------------------------------
 */
bool SettingsState::handleEvent(const sf::Event& windowEvent)
{
	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	/*for(size_t action = 0; action != Player::ACTION_COUNT; ++action)
	{
		if(bindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if(windowEvent.type == sf::Event::KeyPressed)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), windowEvent.key.code);
				bindingButtons[action]->deactivate();
			}
			break;
		}
	}*/

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	/*if(isKeyBinding)
		updateLabels();
	else*/
		container.handleEvent(windowEvent);

	return false;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Sets the current video mode
 * ----------------------------------------------------------------------
 */
void SettingsState::setCurrentMode()
{
	// Get the context
	Context context = getContext();

	// Go through all modes and delete the too small ones
	for(auto mode = modeList.begin(); mode != modeList.end();)
	{
		// Delete too small resolutions
		if(mode->width < 800 || mode->height < 600)
			mode = modeList.erase(mode);
		else
			++mode;
	}

	// Go through all modes and check which one is the current one
	unsigned int modeCounter = 0;
	for(auto&& mode : modeList)
	{
		// Search for the current one
		if(mode.width == context.window->getSize().x && mode.height == context.window->getSize().y)
			currentMode = modeCounter;

		++modeCounter;
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Applies all changes from the settings
 * ----------------------------------------------------------------------
 */
void SettingsState::applyChanges()
{
	// Get the context
	Context context = getContext();

	// If the resolution changed save it
	if(!(modeList[currentMode].width == context.window->getSize().x && modeList[currentMode].height == context.window->getSize().y) || currentStyle != Game::windowStyle)
		saveResolution();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Saves the resolution
 * ----------------------------------------------------------------------
 */
void SettingsState::saveResolution()
{
	// Get the context
	Context context = getContext();

	// Open/Create the resolution file
	std::ofstream resolutionFile("Content/Saves/resolution.cff", std::ios::binary);
	if(resolutionFile)
	{
		// Save the resolution
		unsigned int width = modeList[currentMode].width;
		unsigned int height = modeList[currentMode].height;
		currentStyle = Game::windowStyle;
		resolutionFile.write(reinterpret_cast<char*>(&width), sizeof(unsigned int));
		resolutionFile.write(reinterpret_cast<char*>(&height), sizeof(unsigned int));
		resolutionFile.write(reinterpret_cast<char*>(&currentStyle), sizeof(sf::Uint32));
		
		// If the resolution changed, recreate the window and reload the state
		context.renderWindow->create(sf::VideoMode(width, height), "Council of Torment", Game::windowStyle);
		requestStackPop();
		requestStackPush(States::SETTINGS);
	}
	else
		throw std::runtime_error("Failed to save resolution: Content/Saves/resolution.cff");

	// Don't forget to close the file
	resolutionFile.close();

	// Enable vSync
	context.renderWindow->setVerticalSyncEnabled(true);
	context.window->create(context.renderWindow->getSize().x, context.renderWindow->getSize().y);

	// Create the effect manager texture
	context.effectManager->createTexture();

	// Init the light manager
	context.lightManager->init();
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Adds all the components to the container
 * ----------------------------------------------------------------------
 */
void SettingsState::addComponents(Context context)
{
	// Build key binding buttons and labels
	/*addButtonLabel(Player::MOVE_LEFT, 150, "Move Left", context);
	addButtonLabel(Player::MOVE_RIGHT, 200, "Move Right", context);
	addButtonLabel(Player::MOVE_UP, 250, "Move Up", context);
	addButtonLabel(Player::MOVE_DOWN, 300, "Move Down", context);*/

	// Update the labels
	//updateLabels();

	// Add the control label
	auto controlLabel = std::make_shared<GUI::Label>("Controls", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	controlLabel->setPosition(bookBackground.getPosition().x + 86.0f, bookBackground.getPosition().y + 9.0f);
	controlLabel->setColor(sf::Color(140, 60, 30));
	controlLabel->setCharacterSize(34);

	// Add the general label
	auto generalLabel = std::make_shared<GUI::Label>("General", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	generalLabel->setPosition(bookBackground.getPosition().x + 374.0f, bookBackground.getPosition().y + 9.0f);
	generalLabel->setColor(sf::Color(140, 60, 30));
	generalLabel->setCharacterSize(34);

	// Add the apply link
	auto applyLink = std::make_shared<GUI::Link>("Apply changes", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	applyLink->setColor(sf::Color(100, 40, 20));
	applyLink->setCharacterSize(28);
	applyLink->setPosition(bookBackground.getPosition().x + 350.0f, bookBackground.getPosition().y + 310.0f);
	applyLink->setCallback([this] ()
	{
		// Apply changes if there are any
		applyChanges();
	});

	// Create the resolution text
	std::stringstream stream;
	stream<<"Resolution: "<<modeList[currentMode].width<<"x"<<modeList[currentMode].height;
	resolutionText = sf::Text(stream.str(), context.contentManager->getFont(Fonts::SEGOEUI), 23);
	resolutionText.setColor(sf::Color(100, 40, 20));
	resolutionText.setPosition(bookBackground.getPosition().x + 315.0f, bookBackground.getPosition().y + 80.0f);

	// Add the previous link
	auto previousLink = std::make_shared<GUI::Link>("Previous", context.contentManager->getFont(Fonts::SEGOEUI));
	previousLink->setColor(sf::Color(100, 40, 20));
	previousLink->setCharacterSize(20);
	previousLink->setPosition(bookBackground.getPosition().x + 360.0f, bookBackground.getPosition().y + 110.0f);
	previousLink->setCallback([this] ()
	{
		// Select previous mode
		currentMode = (currentMode + 1) % modeList.size();
		std::stringstream stream;
		stream<<"Resolution: "<<modeList[currentMode].width<<"x"<<modeList[currentMode].height;
		resolutionText.setString(stream.str());
	});

	// Add the next link
	auto nextLink = std::make_shared<GUI::Link>("Next", context.contentManager->getFont(Fonts::SEGOEUI));
	nextLink->setColor(sf::Color(100, 40, 20));
	nextLink->setCharacterSize(20);
	nextLink->setPosition(bookBackground.getPosition().x + 470.0f, bookBackground.getPosition().y + 110.0f);
	nextLink->setCallback([this] ()
	{
		// Select next mode
		currentMode = (currentMode + modeList.size() - 1) % modeList.size();
		std::stringstream stream;
		stream<<"Resolution: "<<modeList[currentMode].width<<"x"<<modeList[currentMode].height;
		resolutionText.setString(stream.str());
	});

	// Add the label for the fullscreen checkbox
	auto fullscreenLabel = std::make_shared<GUI::Label>("Fullscreen:", context.contentManager->getFont(Fonts::SEGOEUI));
	fullscreenLabel->setColor(sf::Color(100, 40, 20));
	fullscreenLabel->setCharacterSize(24);
	fullscreenLabel->setPosition(bookBackground.getPosition().x + 320.0f, bookBackground.getPosition().y + 150.0f);

	// Add the fullscreen checkbox
	auto fullscreenBox = std::make_shared<GUI::Checkbox>(context.contentManager->getTexture(Textures::CHECKBOX), BRIGHT_CHECKBOX);
	fullscreenBox->setPosition(fullscreenLabel->getPosition().x + fullscreenLabel->getBounds().width + 8.0f, fullscreenLabel->getPosition().y + 7.0f);
	fullscreenBox->setCallback([this] ()
	{
		// Change the window style
		if(Game::windowStyle == sf::Style::Close)
			Game::windowStyle = sf::Style::Fullscreen;
		else
			Game::windowStyle = sf::Style::Close;
	});

	// If the game already runs in fullscreen mode, check the checkbox
	if(Game::windowStyle == sf::Style::Fullscreen)
	{
		currentStyle = Game::windowStyle;
		fullscreenBox->setChecked(true);
	}

	// Add the back link
	auto backLink = std::make_shared<GUI::Link>("Back to menu", context.contentManager->getFont(Fonts::CLOISTER_BLACK));
	backLink->setColor(sf::Color(100, 40, 20));
	backLink->setCharacterSize(32);
	backLink->setPosition(context.window->getSize().x - backLink->getBounds().width - 50.0f, context.window->getSize().y - 80.0f);
	backLink->setCallback([this] ()
	{
		// Apply changes if there are any
		applyChanges();

		// Push the menu state
		requestStackPop();
		requestStackPush(States::MENU);
	});

	// Add all components to the container
	container.add(controlLabel);
	container.add(generalLabel);
	container.add(previousLink);
	container.add(nextLink);
	container.add(fullscreenLabel);
	container.add(fullscreenBox);
	container.add(applyLink);
	container.add(backLink);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the labels of the controls
 * ----------------------------------------------------------------------
 */
void SettingsState::updateLabels()
{
	// Get the player
	Player& player = *getContext().player;

	for(size_t i = 0; i != Player::ACTION_COUNT; ++i)
	{
		// Update the text of all labels
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		bindingLabels[i]->setText(toString(key));
	}
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Adds the buttons + labels for the controls
 * ----------------------------------------------------------------------
 */
void SettingsState::addButtonLabel(Player::Action action, float yPos, const std::string& text, Context context)
{
	// Add a button for each action
	//bindingButtons[action] = std::make_shared<GUI::Button>(*context.contentManager);
	//bindingButtons[action]->setPosition(80.0f, yPos);
	//bindingButtons[action]->setText(text);
	//bindingButtons[action]->setToggle(true);

	//// Add the label of the key for each button
	//bindingLabels[action] = std::make_shared<GUI::Label>("", context.contentManager->getFont(Fonts::FRENCH_GROTESQUE));
	//bindingLabels[action]->setPosition(300.0f, yPos + 15.0f);
	//
	//// Add both to the container
	//container.add(bindingButtons[action]);
	//container.add(bindingLabels[action]);
}