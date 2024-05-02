#include "CClickableObject.h"

CClickableObject::CClickableObject(int x, int y, int w, int h, CTexture const *texture) :
	m_HitBox({ x,y,w,h }),
	m_Texture(texture)
{}

bool CClickableObject::mouseInHitbox(const SDL_Point &mousePosition) const {
	return SDL_PointInRect(&mousePosition, &m_HitBox);
}

void CClickableObject::render() const {
	m_Texture->render(m_HitBox.x, m_HitBox.y);
}

void CClickableObject::move(int x, int y) {
	m_HitBox.x = x;
	m_HitBox.y = y;
}