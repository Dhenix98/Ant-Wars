#include "CButton.h"

CButton::CButton(int x, int y, int w, int h, CTexture const *textureActive, CTexture const *textureInactive,
	TTF_Font *textFont, const std::string &text, EColor activeColor, EColor inactiveColor) :
	CClickableObject(x, y, w, h, textureActive),
	m_TextureInactive(textureInactive),
	m_Text(text, textFont, { x, y, w, h }, 10, 10, activeColor),
	m_TextActiveColor(activeColor),
	m_TextInactiveColor(inactiveColor)
{}

bool CButton::mouseInActiveHitbox(const SDL_Point &mousePosition) const {
	return m_Active && mouseInHitbox(mousePosition);
}

void CButton::render() const {
	if (m_Active) {
		m_Texture->render(m_HitBox.x, m_HitBox.y);
		m_Text.render();
	}
	else if (m_TextureInactive != nullptr) {
		m_TextureInactive->render(m_HitBox.x, m_HitBox.y);
		m_Text.render();
	}
}

void CButton::setActive(bool active) {
	m_Active = active;

	if (active) {
		m_Text.changeColor(m_TextActiveColor);
	}
	else {
		m_Text.changeColor(m_TextInactiveColor);
	}
}

bool CButton::isActive() const {
	return m_Active;
}

int CButton::getX() const {
	return m_HitBox.x;
}

int CButton::getY() const {
	return m_HitBox.y;
}

int CButton::getW() const {
	return m_HitBox.w;
}

int CButton::getH() const {
	return m_HitBox.h;
}

const SDL_Rect &CButton::getHitBox() const {
	return m_HitBox;
}

void CButton::addCharacter(char c) {
	m_Text.addCharacter(c);
}

void CButton::removeCharacter() {
	m_Text.removeCharacter();
}

void CButton::clearText() {
	m_Text.changeText("");
}

void CButton::changeText(const std::string &text) {
	m_Text.changeText(text);
}

const std::string &CButton::getText() const {
	return m_Text.getText();
}
