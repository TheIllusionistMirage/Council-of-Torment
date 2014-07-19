#pragma once

#include "ArmorManager.h"
#include "DialogueBox.h"
#include "Inventory.h"
#include "GUI/Label.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <map>

/*	The player is the main character of the game,
	because the user plays him. Ingame the camera
	will follow him, he will be able to trigger
	map triggers, he has an inventory, etc...
	He inherits from the humanoid class, but he
	is able to do so much more than that.
*/
class Player : public Humanoid
{
	public:
		enum Action
		{
			MOVE_LEFT,
			MOVE_RIGHT,
			MOVE_UP,
			MOVE_DOWN,
			ACTION_COUNT
		};

	public:
		Player(State::Context context, std::string entName, float xPos = 0.0f, float yPos = 0.0f);

		void render();
		void renderGUI();
		void updateCamera();
		void update(sf::Time elapsedTime);
		void setCamera(sf::View camera);
		void renderCollisionShapes();
		void handleEvent(const sf::Event& windowEvent);

		void changeSprite(std::string fileName);
		void assignKey(Action action, sf::Keyboard::Key key);
		void toggleChangingState();
		void setDestinationLevelExit(std::string s);

		bool isAbleToMove();
		bool showCollision() { return collision; }
		void showCollision(bool show) { collision = show; }
		std::string getDstinationLevel();
		sf::Keyboard::Key getAssignedKey(Action action) const;
		Inventory& getInventory();
		sf::View& getCamera();

	private:
		void zoomOut();
		void zoomIn();

	private:
		ArmorManager armorManager;
		Inventory inventory;
		sf::View camera;
		std::map<sf::Keyboard::Key, Action> keyBindings;
		std::map<sf::String, std::unique_ptr<GUI::Label>> statsLabels;
		std::map<sf::String, DialogueBox> dialogueReplies;

		int hunger;
		int thirst;
		bool collision;
		std::string destinationLevel;
};