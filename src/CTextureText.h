#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include <string>
#include "CTexture.h"
#include "EColor.h"

/// @brief text texture
class CTextureText {
public:
	CTextureText(std::string text = "", TTF_Font *font = nullptr, const SDL_Rect &rect = { 0, 0, 0, 0 }, int offsetX = 10, int offsetY = 10, EColor color = EColor::BLACK);
	void render() const;
	/// @brief change color of the text
	void changeColor(EColor color);
	/// @brief change text inside the texture
	void changeText(const std::string &newText);
	/// @brief add character to the text
	void addCharacter(char c);
	/// @brief remove character from the text
	void removeCharacter();
	/// @return text inside the texture
	const std::string &getText() const;

private:
	/// @brief updates text and replaces texture for new one with the updated text
	void updateText();

	std::string m_Text;
	CTexture m_Texture;
	TTF_Font *m_Font;
	EColor m_Color;
	int m_OffsetX, m_OffsetY;
	int m_X, m_Y;
	const SDL_Rect m_Rect;
};
