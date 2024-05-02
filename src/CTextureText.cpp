#include "CTextureText.h"

CTextureText::CTextureText(std::string text, TTF_Font *font, const SDL_Rect &rect, int offsetX, int offsetY, EColor color) :
	m_Text(text),
	m_Font(font),
	m_Color(color),
	m_OffsetX(offsetX),
	m_OffsetY(offsetY),
	m_Rect(rect)
{
	updateText();
}


void CTextureText::render() const {
	m_Texture.render(m_X, m_Y);
}

void CTextureText::updateText() {
	m_Texture.loadFromRenderedText(m_Text, m_Font, m_Rect.w - 2 * m_OffsetX, toSDLColor(m_Color));
	// to make sure that textTexture X size is less than rect X size
	while (!m_Text.empty() && m_Texture.getWidth() > (m_Rect.w - 2 * m_OffsetX)) {
		m_Text.pop_back();
		m_Texture.loadFromRenderedText(m_Text, m_Font, m_Rect.w - 2 * m_OffsetX, toSDLColor(m_Color));
	}
	m_X = m_Rect.x + m_OffsetX + (m_Rect.w - 2 * m_OffsetX - m_Texture.getWidth()) / 2;
	m_Y = m_Rect.y + m_OffsetY + (m_Rect.h - 2 * m_OffsetY - m_Texture.getHeight()) / 2;

}

void CTextureText::changeColor(EColor color) {
	m_Color = color;
	updateText();
}

void CTextureText::changeText(const std::string &newText) {
	m_Text = newText;
	updateText();
}

void CTextureText::addCharacter(char c) {
	m_Text += c;
	updateText();
}

void CTextureText::removeCharacter() {
	if (!m_Text.empty()) {
		m_Text.pop_back();
	}
	updateText();
}

const std::string &CTextureText::getText() const {
	return m_Text;
}
