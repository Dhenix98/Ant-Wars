#include "CPoint.h"

CPoint::CPoint(int x, int y, unsigned int width) :
	CClickableObject(x - width, y - width, 2 * width + 1, 2 * width + 1),
	m_X(x),
	m_Y(y)
{}

bool CPoint::mouseInHitbox(const SDL_Point &mousePosition) const {
	return SDL_PointInRect(&mousePosition, &m_HitBox);
}

void CPoint::render() const {
	SDL_RenderDrawRect(G_RENDERER, &m_HitBox);
}

const SDL_Rect &CPoint::getHitBox() const {
	return m_HitBox;
}
