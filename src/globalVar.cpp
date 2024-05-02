#include "globalVar.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const std::string REL_SAVE_FILE_PATH = "save";
const std::string REL_MAP_FILE_PATH = "maps";
const std::string REL_ASSET_TEXTURE_FILE_PATH = "Assets/Textures";
const std::string REL_SOUND_FILE_PATH = "Assets/Sounds";
const std::string REL_MUSIC_FILE_PATH = "Assets/Music";
const std::string REL_FONT_FILE_PATH = "Assets/Ttf";

SDL_Window *G_WINDOW = nullptr;
SDL_Renderer *G_RENDERER = nullptr;
