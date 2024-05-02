#include "CLine.h"

CLine::CLine(unsigned int id, int xBegin, int xTarget, int yBegin, int yTarget, EColor color) :
	m_ID(id),

	m_XBegin(xBegin),
	m_XTarget(xTarget),
	m_YBegin(yBegin),
	m_YTarget(yTarget),

	m_XCurrent(xBegin),
	m_XCurrentTarget(xBegin),
	m_YCurrent(yBegin),
	m_YCurrentTarget(yBegin),

	m_NegativeX(false),
	m_NegativeY(false),
	m_VerticalLine(false),
	m_HorizontalLine(false),
	m_SinAlpha(0),
	m_CosAlpha(0),

	m_Highlighted(false)
{
	int sizeX = xTarget - xBegin;
	int sizeY = yTarget - yBegin;

	if (sizeX < 0) {
		m_NegativeX = true;
		sizeX *= -1;
	}
	if (sizeY < 0) {
		m_NegativeY = true;
		sizeY *= -1;
	}
	m_CurrLength = 0;
	m_CurrTargetLength = 0;
	m_MaxTargetLength = std::sqrt(sizeX * sizeX + sizeY * sizeY);

	if (sizeX == 0) {
		m_VerticalLine = true;
	}
	else if (sizeY == 0) {
		m_HorizontalLine = true;
	}
	else {
		m_SinAlpha = sizeY / m_MaxTargetLength;
		m_CosAlpha = sizeX / m_MaxTargetLength;
	}

	m_LineDrawDelay = SDL_GetTicks();
	m_Color = color;
}

CLine::CLine(unsigned int id, std::pair<int, int> begin, std::pair<int, int> end, EColor color) :
	CLine(id, begin.first, end.first, begin.second, end.second, color)
{}

void CLine::render() const {
	if (m_Points.size() < 3) {
		return;
	}
	if (m_Highlighted) {
		SDL_Color tmpColor = toSDLColor(m_HighlightedColor);
		SDL_SetRenderDrawColor(G_RENDERER, tmpColor.r, tmpColor.g, tmpColor.b, tmpColor.a);
		drawLine(m_Points[0], m_Points[m_Points.size() - 1], G_RENDERER, 1);
	}
	SDL_Color tmpColor = toSDLColor(m_Color);
	SDL_SetRenderDrawColor(G_RENDERER, tmpColor.r, tmpColor.g, tmpColor.b, tmpColor.a);
	drawLine(m_Points[0], m_Points[m_Points.size() - 1], G_RENDERER);
}


void CLine::drawLine(const CPoint &a, const CPoint &b, SDL_Renderer *renderer, int offset) {
	int fromX = a.getHitBox().x - offset;
	int fromY = a.getHitBox().y - offset;
	int fromXWidth = a.getHitBox().x + a.getHitBox().w + offset;
	int fromYWidth = a.getHitBox().y + a.getHitBox().h + offset;

	int xDistance = a.getHitBox().x - b.getHitBox().x;
	int yDistance = a.getHitBox().y - b.getHitBox().y;

	for (int i = fromX; i <= fromXWidth; ++i) {
		for (int j = fromY; j <= fromYWidth; ++j) {
			if (i == fromX || i == fromXWidth || j == fromY || j == fromYWidth) {
				SDL_RenderDrawLine(G_RENDERER, i, j, i - xDistance, j - yDistance);
			}
		}
	}
}

bool CLine::mouseInHitbox(const SDL_Point &mousePosition) const {
	for (const auto &x : m_Points) {
		if (x.mouseInHitbox(mousePosition)) {
			return true;
		}
	}
	return false;
}

void CLine::gradualMovement() {
	// at least 1 second has passed
	if (m_DrawSpeed < (SDL_GetTicks() - m_LineDrawDelay)) {
		m_LineDrawDelay = SDL_GetTicks();
		movement();
	}
}

void CLine::movement() {
	if (hasReachedTarget()) {
		return;
	}
	if (m_VerticalLine) {
		stepTowardsYTarget();
	}
	else if (m_HorizontalLine) {
		stepTowardsXTarget();
	}
	else {
		stepTowardsXAndYTarget();
	}
	// detect for turn... turning can happen when using new target
	if (m_Points.size() >= 2 && m_Points[m_Points.size() - 2].m_X == m_XCurrent && m_Points[m_Points.size() - 2].m_Y == m_YCurrent) {
		m_Points.pop_back();
	}

	else if (m_Points.empty() || m_Points[m_Points.size() - 1].m_X != m_XCurrent || m_Points[m_Points.size() - 1].m_Y != m_YCurrent) {
		m_Points.push_back(CPoint(m_XCurrent, m_YCurrent, m_LineWidth));
	}
}

void CLine::stepTowardsXTarget() {
	if (m_XCurrentTarget < m_XCurrent) {
		m_XCurrent = std::max(m_XCurrent - m_StepSize, m_XCurrentTarget);
	}
	if (m_XCurrentTarget > m_XCurrent) {
		m_XCurrent = std::min(m_XCurrent + m_StepSize, m_XCurrentTarget);
	}
}

void CLine::stepTowardsYTarget() {
	if (m_YCurrentTarget < m_YCurrent) {
		m_YCurrent = std::max(m_YCurrent - m_StepSize, m_YCurrentTarget);
	}
	if (m_YCurrentTarget > m_YCurrent) {
		m_YCurrent = std::min(m_YCurrent + m_StepSize, m_YCurrentTarget);
	}
}

void CLine::stepTowardsXAndYTarget() {
	if (m_CurrTargetLength > m_CurrLength) {
		m_CurrLength = std::min(m_CurrLength + m_StepSize, m_CurrTargetLength);
	}
	else if (m_CurrTargetLength < m_CurrLength) {
		m_CurrLength = std::max(m_CurrLength - m_StepSize, m_CurrTargetLength);
	}

	int xOffset = static_cast<int>(std::round(m_CosAlpha * m_CurrLength));
	int yOffset = static_cast<int>(std::round(m_SinAlpha * m_CurrLength));

	m_NegativeX ? m_XCurrent = m_XBegin - xOffset : m_XCurrent = m_XBegin + xOffset;
	m_NegativeY ? m_YCurrent = m_YBegin - yOffset : m_YCurrent = m_YBegin + yOffset;
}

bool CLine::hasReachedTarget() const {
	return m_XCurrent == m_XCurrentTarget && m_YCurrent == m_YCurrentTarget;
}

void CLine::setTargetToBegin() {
	m_CurrTargetLength = 0;
	m_XCurrentTarget = m_XBegin;
	m_YCurrentTarget = m_YBegin;
}

void CLine::setTargetToMiddle() {
	m_CurrTargetLength = m_MaxTargetLength / 2;
	m_XCurrentTarget = middle(m_XBegin, m_XTarget);
	m_YCurrentTarget = middle(m_YBegin, m_YTarget);
}

void CLine::setTargetToEnd() {
	m_CurrTargetLength = m_MaxTargetLength;
	m_XCurrentTarget = m_XTarget;
	m_YCurrentTarget = m_YTarget;
}

void CLine::setTarget(int x, int y) {
	m_CurrTargetLength = sqrt(x * x + y * y);
	m_XCurrentTarget = x;
	m_YCurrentTarget = y;
}

int CLine::middle(int a, int b) {
	double result = (a + b) / 2.0;
	return static_cast<int>(std::round(result));
}

void CLine::setHighlight(bool choice) {
	m_Highlighted = choice;
}

bool CLine::toBeDeleted() const {
	return m_XBegin == m_XCurrent && m_XCurrent == m_XCurrentTarget && m_YBegin == m_YCurrent && m_YCurrent == m_YCurrentTarget;
}


std::string CLine::toString() const {
	return std::to_string(m_XCurrent) +
		" " + std::to_string(m_YCurrent);
}

int CLine::getCurrCoordX() const {
	return m_XCurrent;
}

int CLine::getCurrCoordY() const {
	return m_YCurrent;
}

void CLine::instantMoveToTarget() {
	while (m_XCurrent != m_XCurrentTarget || m_YCurrent != m_YCurrentTarget) {
		movement();
	}
}

EColor CLine::getColor() const {
	return m_Color;
}

void CLine::setColor(EColor color) {
	m_Color = color;
}
