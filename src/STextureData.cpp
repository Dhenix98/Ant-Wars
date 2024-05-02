#include "STextureData.h"

STextureData::STextureData(SDL_Texture *texture, int width, int height) :
	m_Texture(texture),
	m_Width(width),
	m_Height(height)
{}

STextureData::~STextureData() {
	if (m_Texture != nullptr) {
		SDL_DestroyTexture(m_Texture);
	}
}
