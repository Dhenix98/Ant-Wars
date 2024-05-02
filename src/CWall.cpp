#include "CWall.h"

CWall::CWall(int x, int y, int w, int h) :
	m_HitBox({ x, y, w, h })
{}

void CWall::render() const {
	SDL_SetRenderDrawColor(G_RENDERER, 127, 127, 127, 255);
	SDL_RenderFillRect(G_RENDERER, &m_HitBox);
}

bool CWall::collision(int x, int y) const {
	SDL_Point tmpPoint{ x, y };
	return SDL_PointInRect(&tmpPoint, &m_HitBox);
}


int CWall::getX() const {
	return m_HitBox.x;
}

int CWall::getY() const {
	return m_HitBox.y;
}