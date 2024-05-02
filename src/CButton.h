#pragma once
#include "CClickableObject.h"
#include "EColor.h"
#include "CTextureText.h"

/// @brief has two "modes" active and inactive both are affected differently by "mouseInActiveHitbox"/"mouseInHitbox" and render
class CButton : public CClickableObject {
public:
	CButton(int x, int y, int w, int h, CTexture const *textureActive, CTexture const *textureInactive = nullptr,
		TTF_Font *textFont = nullptr, const std::string &text = "", EColor activeColor = EColor::BLACK, EColor inactiveColor = EColor::GRAY);

	/// @return true if mouse is inside hitbox and CButton is active
	bool mouseInActiveHitbox(const SDL_Point &mousePosition) const;
	void render() const override;

	void setActive(bool active);
	bool isActive() const;

	int getX() const;
	int getY() const;
	int getW() const;
	int getH() const;
	const SDL_Rect &getHitBox() const;

	/// @brief add character into button text
	void addCharacter(char c);
	/// @brief remove character from button text
	void removeCharacter();
	/// @brief clear characters in button text
	void clearText();
	/// @brief put parameter text into button text
	void changeText(const std::string &text);
	const std::string &getText() const;

private:
	bool m_Active = true;
	CTexture const *m_TextureInactive;

	CTextureText m_Text;
	const int textOffsetX = 10;
	const int textOffsetY = 10;
	EColor m_TextActiveColor;
	EColor m_TextInactiveColor;
};
