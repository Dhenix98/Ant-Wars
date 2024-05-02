#pragma once
#include "CClickableObject.h"

/// @brief clickable "point", used in clicking of the line
class CPoint : public CClickableObject {
public:
	CPoint(int x, int y, unsigned int width = 1);
	bool mouseInHitbox(const SDL_Point &mousePosition) const override;
	void render() const override;
	const SDL_Rect &getHitBox() const;

	int m_X, m_Y;
};
