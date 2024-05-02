#include <iostream>
#include <stdexcept>
#include "CTexture.h"

CTexture::CTexture()
{
	init();
}

void CTexture::init() {
	m_Data = std::make_shared<STextureData>(nullptr, 0, 0);
}

bool CTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	init();

	//The final texture
	SDL_Texture *newTexture = nullptr;

	//Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface != nullptr)
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(G_RENDERER, loadedSurface);
		if (newTexture == nullptr)
		{
			std::cout << "Unable to create texture from \"" << path << "\"!" << " SDL Error : " << SDL_GetError() << std::endl;;
		}
		else
		{
			//Get image dimensions
			m_Data->m_Width = loadedSurface->w;
			m_Data->m_Height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	m_Data->m_Texture = newTexture;
	return m_Data->m_Texture != nullptr;
}

bool CTexture::loadFromRenderedText(const std::string &textureText, TTF_Font *font, int textLength, SDL_Color color)
{
	if (font == nullptr) {
		return false;
	}

	init();

	SDL_Surface *textSurface = nullptr;
	if (textureText.empty()) {
		textSurface = TTF_RenderText_Blended(font, (textureText + " ").c_str(), color);
	}
	else {
		textSurface = TTF_RenderText_Blended(font, textureText.c_str(), color);
	}

	if (textSurface != nullptr) {
		m_Data->m_Texture = SDL_CreateTextureFromSurface(G_RENDERER, textSurface);
		if (m_Data->m_Texture == nullptr) {
			std::cout << "Unable to create texture from rendered text!" << std::endl;
		}
		else {
			m_Data->m_Width = textSurface->w;
			m_Data->m_Height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	else {
		std::cout << "Unable to render text surface!" << std::endl;
	}
	return m_Data->m_Texture != nullptr;
}

void CTexture::render(int x, int y, double angle) const {
	render(x, y, m_Data->m_Width, m_Data->m_Height, angle);
}

void CTexture::render(int x, int y, int w, int h, double angle) const {
	if (w < 0 || h < 0) {
		throw std::invalid_argument("Width or height cannot be negative!");
	}
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, w, h };

	//Render to screen
	SDL_RenderCopyEx(G_RENDERER, m_Data->m_Texture, nullptr, &renderQuad, angle, nullptr, SDL_FLIP_NONE);
}

int CTexture::getWidth() const
{
	return m_Data->m_Width;
}

int CTexture::getHeight() const
{
	return m_Data->m_Height;
}