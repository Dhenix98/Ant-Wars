#pragma once

#ifdef __linux__
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#elif _WIN32
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#endif

#include <map>
#include <string>
#include <iostream>
#include "CTexture.h"
#include "globalVar.h"
#include "Exceptions.h"

/// @brief Database containing all assets needed in application
class CDatabase {
public:
	/// @brief initializes all textures, ttfs, sounds and music
	CDatabase();
	/// @brief destroys and free all objects
	~CDatabase();
	/// @return pointer on const value texture
	CTexture *const getTexture(unsigned int id);
	/// @return pointer on const value font
	TTF_Font *const getFont(unsigned int id);
	/// @return pointer on const value sound
	Mix_Chunk *const getSound(unsigned int id);
	/// @return pointer on const value music
	Mix_Music *const getMusic(unsigned int id);

private:
	std::map<unsigned int, CTexture> m_Textures;
	std::map<unsigned int, TTF_Font *> m_Fonts;
	std::map<unsigned int, Mix_Chunk *> m_Sounds;
	std::map<unsigned int, Mix_Music *> m_Music;

	bool loadTextures();
	bool loadFonts();
	bool loadSounds();
	bool loadMusic();

	// contains list of textures
	bool loadMenuSharedTextures();
	bool loadMainMenuTextures();
	bool loadNewGameTextures();
	bool loadGameTextures();
	bool loadVictoryTextures();
	bool loadDefeatTextures();
	bool loadSaveTextures();
	bool loadLoadTextures();

	// load asset into correct map structure
	bool loadTexture(std::string path, unsigned int textureId);
	bool loadFont(std::string fontFileName, int fontSize, unsigned int fontName);
	bool loadSound(std::string soundFileName, unsigned int soundId);
	bool loadMusicFile(std::string musicFileName, unsigned int musicId);
};

using DatabasePtr = std::shared_ptr<CDatabase>;

/// @brief asset ids
namespace DATABASE_ID {
	namespace TEXTURE_ID {
		static int currID = 0;
		namespace MENU {
			const int BTN_AVAILABLE = currID++;
			const int BTN_NOT_AVAILABLE = currID++;

			const int SAVE_FILE_AVAIL = currID++;
			const int SAVE_FILE_NOT_AVAIL = currID++;
			const int SAVE_FILE_SELECTED = currID++;

			namespace MAIN_MENU {
				const int SCREEN = currID++;
			}
			namespace NEW_GAME {
				const int SCREEN = currID++;
				const int SEL_BTN = currID++;
				const int BLUE_BTN = currID++;
				const int GREEN_BTN = currID++;
				const int RED_BTN = currID++;
				const int DIFF_BTN = currID++;
			}
			namespace SAVE {
				const int SCREEN = currID++;
			}
			namespace LOAD {
				const int SCREEN = currID++;
			}
		}
		namespace GAME {
			const int SCREEN = currID++;
			const int BLUE_HIVE = currID++;
			const int GREEN_HIVE = currID++;
			const int RED_HIVE = currID++;
			const int GRAY_HIVE = currID++;
			const int HIGHLIGHTED_BLUE_HIVE = currID++;
			const int HIGHLIGHTED_GREEN_HIVE = currID++;
			const int HIGHLIGHTED_RED_HIVE = currID++;
			const int HIGHLIGHTED_GRAY_HIVE = currID++;
		}
		namespace VICTORY {
			const int SCREEN = currID++;
		}
		namespace DEFEAT {
			const int SCREEN = currID++;
		}
	}
	namespace SOUND_ID {
		static int currID = 0;
		const int INVALID_MOVE = currID++;
	}
	namespace MUSIC_ID {
		static int currID = 0;
		const int MAIN = currID++;
	}
	namespace FONT_ID {
		static int currID = 0;

		const int HIVE_SIZE = currID++;
		const int DIFFICULTY_END_SCREEN = currID++;
		const int INPUT = currID++;
		const int BUTTON = currID++;
		const int DIFFICULTY_LETTER = currID++;
		const int SAVE_FILE = currID++;
	}
}
