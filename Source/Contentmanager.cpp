#include "ContentManager.h"
#include <cassert>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 26 January 2014
 * Description: Constructor of content manager
 * ----------------------------------------------------------------------
 */
	ContentManager::ContentManager()
	: currentMusicID(Music::NO_MUSIC)
	, nextMusicID(Music::NO_MUSIC)
	, musicVolume(0.0f)
	, fadeSpeed(0.0f)
	, halfMusicFade(false)
	, nextMusic(false)
	{
		// Load the textures
		loadTexture(Textures::TITLE_SCREEN, "Content/Textures/title_screen.png");
		loadTexture(Textures::TEXT_LOGO, "Content/Textures/text_logo.png");
		loadTexture(Textures::BUTTON_NORMAL, "Content/Textures/GUI/button_normal.png");
		loadTexture(Textures::BUTTON_SELECTED, "Content/Textures/GUI/button_selected.png");
		loadTexture(Textures::BUTTON_PRESSED, "Content/Textures/GUI/button_pressed.png");
		loadTexture(Textures::CHECKBOX, "Content/Textures/GUI/checkbox.png");
		loadTexture(Textures::OPTIONS_BOOK, "Content/Textures/options_book.png");
		loadTexture(Textures::INVENTORY, "Content/Textures/GUI/inventory.png");
		loadTexture(Textures::ITEMS, "Content/Textures/GUI/items.png");
		loadTexture(Textures::FISHERMAN, "Content/Animations/fisherman.png");
		loadTexture(Textures::SCROLL_UP, "Content/Textures/GUI/scroll_up.png");
		loadTexture(Textures::SCROLL_DOWN, "Content/Textures/GUI/scroll_down.png");
		loadTexture(Textures::ICON_ITEMS, "Content/Textures/GUI/icon_items.png");
		loadTexture(Textures::CRAFTING_WINDOW, "Content/Textures/GUI/crafting_window.png");

		// Load the images
		loadImage(Images::HEAT_HAZE_DISTORTION, "Content/Textures/heat_haze_distortion_map.png");

		// Load the tilesets
		std::string name;

		name = "gideon_tower";				loadTileset(name, "Content/Textures/Tilesets/"+name+".png");
		name = "gideon_tower_outside";		loadTileset(name, "Content/Textures/Tilesets/" + name + ".png");
		name = "gideon_tower_outside_west";	loadTileset(name, "Content/Textures/Tilesets/" + name + ".png");
		name = "gideon_tower_outside_camp";	loadTileset(name, "Content/Textures/Tilesets/" + name + ".png");
		name = "peasant_farm_house_1";		loadTileset(name, "Content/Textures/Tilesets/gideon_tower_outside.png");
		name = "southdell_north_forest";	loadTileset(name, "Content/Textures/Tilesets/gideon_tower_outside.png");

		// Load the soundbuffers
		loadSoundBuffer(Sounds::TEST, "Content/Sounds/test.wav");

		// Load the music
		loadMusic(Music::GIDEON_TOWER, "Content/Music/gideon_tower.ogg");
		loadMusic(Music::TOWN_SQUARE, "Content/Music/town_square.ogg");

		// Load the fonts
		loadFont(Fonts::FRENCH_GROTESQUE, "Content/Fonts/french_grotesque.ttf");
		loadFont(Fonts::CLOISTER_BLACK, "Content/Fonts/cloister_black.ttf");
		loadFont(Fonts::SEGOEUI, "Content/Fonts/segoeui.ttf");
		loadFont(Fonts::ARIAL, "Content/Fonts/arial.ttf");

		// Load the shaders
		loadShader(Shaders::MENU, "Content/Shaders/menu.frag", sf::Shader::Fragment);
		loadShader(Shaders::LIGHT, "Content/Shaders/light.frag", sf::Shader::Fragment);
		loadShader(Shaders::HEAT_HAZE, "Content/Shaders/heat_haze.frag", sf::Shader::Fragment);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Updates the sounds and music
 * ----------------------------------------------------------------------
 */
	void ContentManager::update(sf::Time elapsedTime)
	{
		// Check if a sound has stopped -> remove it
		for(auto sound = soundList.begin(); sound != soundList.end();)
		{
			if(sound->getStatus() == sound->Stopped)
				sound = soundList.erase(sound++);
			else
				++sound;
		}

		// Fade away the old song and play the new one
		if(nextMusic)
		{
			// Change the volume of the music
			if(halfMusicFade)
				musicVolume += fadeSpeed * elapsedTime.asSeconds();
			else
				musicVolume -= fadeSpeed * elapsedTime.asSeconds();
			if(currentMusicID != Music::NO_MUSIC)
				musicMap[currentMusicID]->setVolume(musicVolume);

			if(musicVolume < 0.0f)
			{
				// Stop the old music and play the new one
				if(currentMusicID != Music::NO_MUSIC)
					musicMap[currentMusicID]->stop();
				if(nextMusicID != Music::NO_MUSIC)
				{
					musicMap[nextMusicID]->setLoop(true);
					musicMap[nextMusicID]->setVolume(0);
					musicMap[nextMusicID]->play();
				}

				// Update some values
				halfMusicFade = true;
				currentMusicID = nextMusicID;
			}
			else if(musicVolume > 100.0f)
			{
				// Stop the fading
				nextMusic = false;
				halfMusicFade = false;
				musicVolume = 100.0f;
			}
		}
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Adds a new sound effect
 * ----------------------------------------------------------------------
 */
	void ContentManager::pollSoundEffect(Sounds::ID soundID, float volume, float pitch)
	{
		// Add a new sound effect and play it
		soundList.push_back(sf::Sound(*soundBufferMap[Sounds::TEST]));
		soundList.back().setVolume(volume);
		soundList.back().setPitch(pitch);
		soundList.back().play();
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 30 January 2014
 * Description: Adds a new sound effect
 * ----------------------------------------------------------------------
 */
	void ContentManager::playMusic(Music::ID musicID, float fadeSpeed)
	{
		// Prepare the next song
		this->fadeSpeed = fadeSpeed;
		nextMusicID = musicID;
		nextMusic = true;

		// If no music is playing atm, skip the fade out
		if(currentMusicID == Music::NO_MUSIC)
			musicVolume = 0.0f;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Loads a shader
 * ----------------------------------------------------------------------
 */
	void ContentManager::loadShader(Shaders::ID shaderID, const std::string& filename, sf::Shader::Type type)
	{
		// Create a new shader, load it and save it on the map
		std::unique_ptr<sf::Shader> shader(new sf::Shader());
		if(!shader->loadFromFile(filename, type))
			throw std::runtime_error("Failed to load shader: " + filename);
		auto inserted = shaderMap.insert(std::make_pair(shaderID, std::move(shader)));
		assert(inserted.second);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Loads a texture
 * ----------------------------------------------------------------------
 */
	void ContentManager::loadTexture(Textures::ID textureID, const std::string& filename)
	{
		// Create a new texture, load it and save it on the map
		std::unique_ptr<sf::Texture> texture(new sf::Texture());
		if(!texture->loadFromFile(filename))
			throw std::runtime_error("Failed to load texture: " + filename);
		auto inserted = textureMap.insert(std::make_pair(textureID, std::move(texture)));
		assert(inserted.second);
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 28 Juli 2014
* Description: Loads an image
* ----------------------------------------------------------------------
*/
	void ContentManager::loadImage(Images::ID imageID, const std::string& filename)
	{
		// Create a new image, load it and save it on the map
		std::unique_ptr<sf::Image> image(new sf::Image());
		if(!image->loadFromFile(filename))
			throw std::runtime_error("Failed to load image: " + filename);
		auto inserted = imageMap.insert(std::make_pair(imageID, std::move(image)));
		assert(inserted.second);
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Loads a tileset
* ----------------------------------------------------------------------
*/
	void ContentManager::loadTileset(std::string key, const std::string& filename)
	{
		std::unique_ptr<sf::Texture> texture(new sf::Texture());
		if (!texture->loadFromFile(filename))
			throw std::runtime_error("Failed to load tileset texture: " + filename);
		auto inserted = tilesetsMap.insert(std::make_pair(key, std::move(texture)));
		assert(inserted.second);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Loads a sound buffer
 * ----------------------------------------------------------------------
 */
	void ContentManager::loadSoundBuffer(Sounds::ID soundID, const std::string& filename)
	{
		// Create a new soundbuffer, load it and save it on the map
		std::unique_ptr<sf::SoundBuffer> soundbuffer(new sf::SoundBuffer());
		if(!soundbuffer->loadFromFile(filename))
			throw std::runtime_error("Failed to load soundbuffer: " + filename);
		auto inserted = soundBufferMap.insert(std::make_pair(soundID, std::move(soundbuffer)));
		assert(inserted.second);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Loads a font
 * ----------------------------------------------------------------------
 */
	void ContentManager::loadFont(Fonts::ID fontID, const std::string& filename)
	{
		// Create a new font, load it and save it on the map
		std::unique_ptr<sf::Font> font(new sf::Font());
		if(!font->loadFromFile(filename))
			throw std::runtime_error("Failed to load font: " + filename);
		auto inserted = fontMap.insert(std::make_pair(fontID, std::move(font)));
		assert(inserted.second);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 30 January 2014
 * Description: Loads a music
 * ----------------------------------------------------------------------
 */
	void ContentManager::loadMusic(Music::ID musicID, const std::string& filename)
	{
		// Create a new music, load it and save it on the map
		std::unique_ptr<sf::Music> music(new sf::Music());
		if(!music->openFromFile(filename))
			throw std::runtime_error("Failed to load music: " + filename);
		auto inserted = musicMap.insert(std::make_pair(musicID, std::move(music)));
		assert(inserted.second);
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Loads a texture and returns it
 * ----------------------------------------------------------------------
 */
	sf::Texture ContentManager::loadTexture(const std::string& filename) const
	{
		// Create a new texture, load it and return it
		sf::Texture texture;
		if(!texture.loadFromFile(filename))
			throw std::runtime_error("Failed to load texture: " + filename);

		// Return the texture
		return texture;
	}


/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns a texture
 * ----------------------------------------------------------------------
 */
	sf::Texture& ContentManager::getTexture(Textures::ID textureID) const
	{
		// Search the texture and return it
		auto found = textureMap.find(textureID);
		assert(found != textureMap.end());
		return *found->second;
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 28 Juli 2014
* Description: Returns an image
* ----------------------------------------------------------------------
*/
	sf::Image& ContentManager::getImage(Images::ID imageID) const
	{
		// Search the texture and return it
		auto found = imageMap.find(imageID);
		assert(found != imageMap.end());
		return *found->second;
	}

/* ----------------------------------------------------------------------
* Author: Julian
* Date: 19 January 2014
* Description: Returns a tileset
* ----------------------------------------------------------------------
*/
	sf::Texture& ContentManager::getTileset(std::string key) const
	{
		// Search the texture and return it
		auto found = tilesetsMap.find(key);
		assert(found != tilesetsMap.end());
		return *found->second;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns a shader
 * ----------------------------------------------------------------------
 */
	sf::Shader& ContentManager::getShader(Shaders::ID shaderID) const
	{
		// Search the shader and return it
		auto found = shaderMap.find(shaderID);
		assert(found != shaderMap.end());
		return *found->second;
	}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns a font
 * ----------------------------------------------------------------------
 */
	sf::Font& ContentManager::getFont(Fonts::ID fontID) const
	{
		// Search the font and return it
		auto found = fontMap.find(fontID);
		assert(found != fontMap.end());
		return *found->second;
	}