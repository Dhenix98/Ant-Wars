#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#include <stdio.h>
#include <string>
#include <memory>
#include "globalVar.h"
#include "STextureData.h"

extern SDL_Renderer *G_RENDERER;

/// @brief Texture wrapper class
class CTexture
{
public:
	/// @brief initialize variables
	CTexture();

	/// @brief initialize new texture and delete old
	void init();

	/// @brief load image at specified path
	/// @return success of loading
	bool loadFromFile(std::string path);

	/// @brief create image from font string
	/// @return success of loading
	bool loadFromRenderedText(const std::string &textureText, TTF_Font *font, int textLength, SDL_Color color);

	/// @brief render texture at coords
	void render(int x, int y, double angle = 0) const;
	/// @brief render texture at coords
	void render(int x, int y, int w, int h, double angle = 0) const;

	int getWidth() const;
	int getHeight() const;

private:
	std::shared_ptr<STextureData> m_Data;
};