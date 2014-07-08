#pragma once

#include "State.h"
#include "TmxParser/Tmx.h"

/*	The game map consists of some layers to get the
	cool 2.5d look. Every layer can be seen as a unique
	part of the map and can be individually modified.
*/
class MapLayer
{
	public:
		MapLayer(State::Context context, std::string name, std::string mapName, int width, int height);
		void pushTile(const std::vector<Tmx::MapTile>& tile);
		void render(); 

		std::string getName() const;
		std::vector<std::vector<Tmx::MapTile>>& getTileList();

	private:
		State::Context context;
		std::vector<std::vector<Tmx::MapTile>> tileList;

		sf::VertexArray mapVertices;
		sf::Texture mapTexture;
		std::string mapName;
		std::string name;
		int height;
		int width;
};