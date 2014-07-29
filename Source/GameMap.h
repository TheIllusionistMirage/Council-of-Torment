#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "AnimationTile.h"
#include "MapTrigger.h"
#include "MapLayer.h"
#include "LivingEntity.h"
#include <memory>

struct HeatHaze
{
	HeatHaze(sf::Shader& shader, sf::ConvexShape shape)
	: distortionFactor {0.04f}
	, riseFactor {0.4f}
	, shader {shader}
	, shape {shape}
	{
	}

	sf::ConvexShape shape;
	sf::Texture texture;
	sf::Shader& shader;
	sf::Clock clock;

	float distortionFactor;
	float riseFactor;
};

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
		void renderShaderEffects();
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
		std::map<std::string, std::unique_ptr<LivingEntity>>& getNPCs();
		std::map<int, std::map<int, std::vector<sf::ConvexShape>>>& getCollisionMap();
		std::vector<sf::Sprite>& getImageLayer();
		std::unique_ptr<Tmx::Map>& getMapObject();
		MapLayer& getLayer(std::string name);

	private:
		std::map<int, std::map<int, std::vector<sf::ConvexShape>>> collisionMap;
		std::vector<std::unique_ptr<AnimationTile>> animationMap;
		std::map<std::string, std::unique_ptr<LivingEntity>> NPCs;
		std::vector<Tmx::Object> objectList;
		std::vector<sf::Sprite> imageLayer;
		std::vector<MapLayer> layerList;
		std::vector<HeatHaze> heatHazes;
		State::Context context;

		std::unique_ptr<Tmx::Map> map;
		sf::Vector2f spawnLocation;
		std::string currentMapName;
		int mapHeight;
		int mapWidth;
		bool naturalLight;

		std::map<std::string, std::unique_ptr<MapTrigger>> mapTriggers;
		std::map<std::string, std::unique_ptr<MapTrigger>> npcTriggers;
};