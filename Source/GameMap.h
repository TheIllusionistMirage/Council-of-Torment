#pragma once

#include "AnimationTile.h"
#include "MapTrigger.h"
#include "MapLayer.h"
#include "Humanoid.h"
#include <memory>

/*	The game map loads a map using the Tmx-parser.
	It also places triggers, sets the spawn, etc...
	Without the game map, the player would walk in
	the endless void of darkness.
*/
class GameMap
{
	public:
		GameMap(State::Context context);

		void update(sf::Time elapsedTime);
		void updateNPCs(sf::Time elapsedTime);
		void renderNPCs();
		void render(std::string layerName);
		void renderAnimations(std::string layerName);
		void loadMap(const std::string& filename, Direction direction = NO_DIRECTION);
		void executeTrigger(std::string name);
		void parseScripts();
		void parseFile();
		void reload();

		int getMapHeight() const;
		int getMapWidth() const;
		bool isNaturalLighting() const;
		MapLayer& getLayer(std::string name);
		std::unique_ptr<Tmx::Map>& getMapObject();
		std::map<int, std::map<int, std::vector<sf::ConvexShape>>>& getCollisionMap();
		std::map<std::string, std::unique_ptr<Humanoid>>& getNPCs();

	private:
		State::Context context;
		std::vector<Tmx::Object> objectList;
		std::vector<MapLayer> layerList;
		std::map<int, std::map<int, std::vector<sf::ConvexShape>>> collisionMap;
		std::map<std::string, std::unique_ptr<Humanoid>> NPCs;
		std::vector<std::unique_ptr<AnimationTile>> animationMap;

		std::unique_ptr<Tmx::Map> map;
		sf::Vector2f spawnLocation;
		std::string currentMapName;
		int mapHeight;
		int mapWidth;
		bool naturalLight;

		std::map<std::string, std::unique_ptr<MapTrigger>> mapScripts;
		std::map<std::string, std::unique_ptr<MapTrigger>> npcScripts;
};