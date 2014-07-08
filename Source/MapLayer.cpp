#include "MapLayer.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Constructor of the map layer class
 * ----------------------------------------------------------------------
 */
MapLayer::MapLayer(State::Context context, std::string name, std::string mapName, int width, int height)
: context(context)
, mapName(mapName)
, height(height)
, width(width)
, name(name)
{
	// Set the primitive type of the vertices and load the map texture
	mapVertices.setPrimitiveType(sf::Quads);
	mapTexture = context.contentManager->loadTexture("Content/Textures/Tilesets/" + mapName.substr(0, mapName.size() - 4) + ".png");
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Adds a new tile to the tile list
 * ----------------------------------------------------------------------
 */
void MapLayer::pushTile(const std::vector<Tmx::MapTile>& tile)
{
	// Push a new tile into the vector
	tileList.push_back(tile);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Renders the hole layer which is visible by the camera
 * ----------------------------------------------------------------------
 */
void MapLayer::render()
{
	// Get the camera and clear the vertices
	sf::View camera = context.window->getView();
	mapVertices.clear();

	float zoom = context.window->getSize().x / camera.getSize().x;
	int camCoordX = int(camera.getCenter().x - camera.getSize().x / 2) / TILE_SIZE;
	int camCoordY = int(camera.getCenter().y - camera.getSize().y / 2) / TILE_SIZE;
	int stopX = int(camCoordX + context.window->getSize().x / TILE_SIZE / zoom + 2.5f);
	int stopY = int(camCoordY + context.window->getSize().y / TILE_SIZE / zoom + 2.5f);

	for(int x = camCoordX; x != stopX; ++x)
	{
		for(int y = camCoordY; y != stopY; ++y)
		{
			//====================================================================\\
			// Figure showing the quad order. Important.                          \\
			//====================================================================\\
			//  quad[0]			quad[1]			   |                              \\
			//        +-----------+                |        (1)    (2)            \\
			//        |           |                |          +----+              \\
			//        |           |                |          |    |              \\
			//        |           |                |          +----+              \\
			//        |           |                |        (4)    (3)            \\
			//        +-----------+                |                              \\
			// quad[3]			quad[2]			   |    Order of the points       \\
			//====================================================================\\

			if(x >= 0 && x < width && y >= 0 && y < height)
			{
				int id = tileList[y][x].id;

				// Declare 4 points of the quad
				sf::Vertex quad[4];

				// Set their positions
				quad[0].position = sf::Vector2f(float(x * TILE_SIZE), float(y * TILE_SIZE));
				quad[1].position = sf::Vector2f(float(x * TILE_SIZE + TILE_SIZE), float(y * TILE_SIZE));
				quad[2].position = sf::Vector2f(float(x * TILE_SIZE + TILE_SIZE), float(y * TILE_SIZE + TILE_SIZE));
				quad[3].position = sf::Vector2f(float(x * TILE_SIZE), float(y * TILE_SIZE + TILE_SIZE));

				// Set their texture bounding box
				quad[0].texCoords =	sf::Vector2f(float((id % TILES_PER_ROW) * TILE_SIZE), float((id / TILES_PER_ROW) * TILE_SIZE));
				quad[1].texCoords =	sf::Vector2f(float((id % TILES_PER_ROW * TILE_SIZE) + TILE_SIZE), float((id / TILES_PER_ROW) * TILE_SIZE));
				quad[2].texCoords =	sf::Vector2f(float((id % TILES_PER_ROW) * TILE_SIZE + TILE_SIZE), float((id / TILES_PER_ROW * TILE_SIZE) + TILE_SIZE));
				quad[3].texCoords =	sf::Vector2f(float((id % TILES_PER_ROW) * TILE_SIZE), float((id / TILES_PER_ROW * TILE_SIZE) + TILE_SIZE));

				if(tileList[y][x].flippedHorizontally)
				{
					sf::Vector2f p1 = quad[0].texCoords;
					sf::Vector2f p2 = quad[1].texCoords;
					sf::Vector2f p3 = quad[2].texCoords;
					sf::Vector2f p4 = quad[3].texCoords;
				
					quad[0].texCoords =	p2;
					quad[1].texCoords =	p1;
					quad[2].texCoords =	p4;
					quad[3].texCoords =	p3;
				}

				if(tileList[y][x].flippedVertically)
				{
					sf::Vector2f p1 = quad[0].texCoords;
					sf::Vector2f p2 = quad[1].texCoords;
					sf::Vector2f p3 = quad[2].texCoords;
					sf::Vector2f p4 = quad[3].texCoords;
				
					quad[0].texCoords =	p4;
					quad[1].texCoords =	p3;
					quad[2].texCoords =	p2;
					quad[3].texCoords =	p1;
				}

				if(tileList[y][x].flippedDiagonally)
				{
					sf::Vector2f p1 = quad[0].texCoords;
					sf::Vector2f p2 = quad[1].texCoords;
					sf::Vector2f p3 = quad[2].texCoords;
					sf::Vector2f p4 = quad[3].texCoords;

					quad[0].texCoords = p4;
					quad[1].texCoords = p1;
					quad[2].texCoords = p2;
					quad[3].texCoords = p3;
				}

				mapVertices.append(quad[0]);
				mapVertices.append(quad[1]);
				mapVertices.append(quad[2]);
				mapVertices.append(quad[3]);
			}
		}
	}

	// Draws the array
	context.window->draw(mapVertices, &mapTexture);
}

 /* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the name of the layer
 * ----------------------------------------------------------------------
 */
std::string MapLayer::getName() const
{
	// Return the name of the layer
	return name;
}

std::vector<std::vector<Tmx::MapTile>>& MapLayer::getTileList()
{
	return tileList;
}