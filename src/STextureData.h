#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

struct STextureData {
	STextureData(SDL_Texture *texture, int width, int height);
	~STextureData();

	/// @brief The actual hardware texture
	SDL_Texture *m_Texture;

	int m_Width;
	int m_Height;
};
