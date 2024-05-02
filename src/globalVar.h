#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include <string>

// screen size (if changed than maps should be updated!)

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// file paths

extern const std::string REL_SAVE_FILE_PATH;
extern const std::string REL_MAP_FILE_PATH;
extern const std::string REL_ASSET_TEXTURE_FILE_PATH;
extern const std::string REL_SOUND_FILE_PATH;
extern const std::string REL_MUSIC_FILE_PATH;
extern const std::string REL_FONT_FILE_PATH;

extern SDL_Window *G_WINDOW;
extern SDL_Renderer *G_RENDERER;
