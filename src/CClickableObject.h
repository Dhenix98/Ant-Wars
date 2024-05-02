#pragma once
#include "CTexture.h"

/// @brief object which can be clicked and renders itself
class CClickableObject {
public:
	CClickableObject(int x = 0, int y = 0, int w = 0, int h = 0, CTexture const *texture = nullptr);

	/// @return true if mouse is in hitbox
	virtual bool mouseInHitbox(const SDL_Point &mousePosition) const;
	/// @brief render texture based on its hitbox values
	virtual void render() const;
	/// @brief move object to different coordinations
	void move(int x, int y);
protected:

	SDL_Rect m_HitBox;
	CTexture const *m_Texture;
};
