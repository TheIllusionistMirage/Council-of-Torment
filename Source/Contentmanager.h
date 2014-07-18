#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace Textures
{
	enum ID
	{
		TITLE_SCREEN,
		TEXT_LOGO,
		BUTTON_NORMAL,
		BUTTON_SELECTED,
		BUTTON_PRESSED,
		CHECKBOX,
		OPTIONS_BOOK,
		INVENTORY,
		ITEMS,
		FISHERMAN,
		SCROLL_UP,
		SCROLL_DOWN,
		ICON_ITEMS,
		CRAFTING_WINDOW
	};
}

namespace Sounds
{
	enum ID
	{
		TEST
	};
}

namespace Music
{
	enum ID
	{
		NO_MUSIC,
		GIDEON_TOWER,
		TOWN_SQUARE
	};
}

namespace Fonts
{
	enum ID
	{
		FRENCH_GROTESQUE,
		CLOISTER_BLACK,
		SEGOEUI
	};
}

namespace Shaders
{
	enum ID
	{
		MENU,
		LIGHT
	};
}

/*	The content manager is one of the most powerful
	classes in this project. Without it the game has
	no textures, no sounds, no shaders, no fonts...
	It loads everything needed on startup and it
	also loads things during runtime, like map textures.
	Furthermore it handles the sounds and music.
*/
class ContentManager
{
	public:
		ContentManager();

		void update(sf::Time elapsedTime);
		void playMusic(Music::ID musicID, float fadeSpeed = 115.0f);
		void pollSoundEffect(Sounds::ID soundID, float volume = 100.0f, float pitch = 1.0f);

	public:
		sf::Texture loadTexture(const std::string& filename) const;
		sf::Texture& getTexture(Textures::ID textureID) const;
		sf::Texture& getTileset(std::string key) const;
		sf::Shader& getShader(Shaders::ID shaderID) const;
		sf::Font& getFont(Fonts::ID fontID) const;

	private:
		std::map<Sounds::ID, std::unique_ptr<sf::SoundBuffer>> soundBufferMap;
		std::map<Textures::ID, std::unique_ptr<sf::Texture>> textureMap;
		std::map<std::string, std::unique_ptr<sf::Texture>> tilesetsMap;
		std::map<Shaders::ID, std::unique_ptr<sf::Shader>> shaderMap;
		std::map<Music::ID, std::unique_ptr<sf::Music>> musicMap;
		std::map<Fonts::ID, std::unique_ptr<sf::Font>> fontMap;

		std::vector<sf::Sound> soundList;
		Music::ID currentMusicID;
		Music::ID nextMusicID;
		float musicVolume;
		float fadeSpeed;
		bool halfMusicFade;
		bool nextMusic;

	private:
		void loadShader(Shaders::ID shaderID, const std::string& filename, sf::Shader::Type type);
		void loadTexture(Textures::ID textureID, const std::string& filename);
		void loadTileset(std::string key, const std::string& filename);
		void loadSoundBuffer(Sounds::ID soundID, const std::string& filename);
		void loadMusic(Music::ID musicID, const std::string& filename);
		void loadFont(Fonts::ID fontID, const std::string& filename);
};