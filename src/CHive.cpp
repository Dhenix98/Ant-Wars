#include "CHive.h"

CHive::CHive(unsigned int ID, int x, int y, CTexture const *texture, CTexture const *highlightedTexture, EColor color, unsigned int numOfAnts, unsigned int capacity, TTF_Font *font) :
	CClickableObject(x, y, 70, 70, texture),
	m_ID(ID),
	m_Color(color),
	m_NewColor(color),
	m_Size(numOfAnts),
	m_Capacity(capacity),
	m_HighlightedTexture(highlightedTexture),
	m_SizeFont(font),
	m_HiveDelay(SDL_GetTicks())
{
	m_SizeTexture.loadFromRenderedText(std::to_string(m_Size), m_SizeFont, 80, { 0, 0, 0, 255 });
}

bool CHive::operator==(const CHive &oth) {
	return m_ID == oth.m_ID;
}

void CHive::contAction() {
	if (m_RegenerationRate < (SDL_GetTicks() - m_HiveDelay)) {
		m_HiveDelay = SDL_GetTicks();
		m_Size = std::min(m_Size + 1, m_Capacity);
	}
	m_SizeTexture.loadFromRenderedText(std::to_string(m_Size), m_SizeFont, 80, { 0, 0, 0, 255 });
}

void CHive::render() const {
	if (m_Highlighted) {
		m_HighlightedTexture->render(m_HitBox.x, m_HitBox.y);
	}
	else {
		m_Texture->render(m_HitBox.x, m_HitBox.y);
	}
	renderSizeInfo();
}

void CHive::setHighlight(bool highlighted) {
	m_Highlighted = highlighted;
}

void CHive::setSelected(bool selected) {
	m_Selected = selected;
}

bool CHive::isSelected() const {
	return m_Selected;
}

bool CHive::loseAnts(const unsigned int number) {

	if (m_Size < number) {
		m_Size = 0;
		return true;
	}
	m_Size -= number;
	return false;
}

bool CHive::collision(int x, int y) const {
	SDL_Point tmpPoint{ x, y };
	return SDL_PointInRect(&tmpPoint, &m_HitBox);
}

bool CHive::gainAnts(const unsigned int number) {
	m_Size += number;

	if (m_Size > m_Capacity) {
		m_Size = m_Capacity;
		return true;
	}
	return false;
}

void CHive::updateTexture(const DatabasePtr &database) {
	m_updateOwner = false;
	using namespace DATABASE_ID::TEXTURE_ID::GAME;
	int wantedID, wantedHighlightedID;
	if (m_Color == EColor::BLUE_HIVE) {
		wantedID = BLUE_HIVE;
		wantedHighlightedID = HIGHLIGHTED_BLUE_HIVE;
	}
	else if (m_Color == EColor::RED_HIVE) {
		wantedID = RED_HIVE;
		wantedHighlightedID = HIGHLIGHTED_RED_HIVE;
	}
	else {
		wantedID = GREEN_HIVE;
		wantedHighlightedID = HIGHLIGHTED_GREEN_HIVE;
	}

	m_Texture = database->getTexture(wantedID);
	m_HighlightedTexture = database->getTexture(wantedHighlightedID);
}

void CHive::renderSizeInfo() const {
	if (m_Size >= 100) {
		m_SizeTexture.render(m_HitBox.x + 18, m_HitBox.y + 20);
	}
	else if (m_Size >= 10) {
		m_SizeTexture.render(m_HitBox.x + 25, m_HitBox.y + 20);
	}
	else {
		m_SizeTexture.render(m_HitBox.x + 32, m_HitBox.y + 20);
	}
}

std::string CHive::toString(std::string player) const {
	return std::to_string(m_ID) +
		" " + player +
		" " + std::to_string(m_Size) +
		" " + std::to_string(m_Capacity) + "\n";
}

std::pair<int, int> CHive::getCentre() const {
	return std::pair<int, int>(m_HitBox.x + m_HitBox.w / 2, m_HitBox.y + m_HitBox.h / 2);
}

EColor CHive::getColor() const {
	return m_Color;
}

bool CHive::getUpdateOwner() const {
	return m_updateOwner;
}

unsigned int CHive::getNumOfAnts() const {
	return m_Size;
}

unsigned int CHive::getCapacity() const {
	return m_Capacity;
}


int CHive::getX() const {
	return m_HitBox.x;
}

int CHive::getY() const {
	return m_HitBox.y;
}

unsigned int CHive::getID() const {
	return m_ID;
}

void CHive::setNewColor(EColor color) {
	m_NewColor = color;
}

void CHive::applyNewColor() {
	m_Color = m_NewColor;
}

void CHive::setUpdateOwner(bool update) {
	m_updateOwner = update;
}

bool CHive::isCapacityRatioLessThan(double ratio) const {
	return m_Size <= m_Capacity * ratio ? true : false;
}

bool CHive::isCapacityRatioGreaterThan(double ratio) const {
	return !isCapacityRatioLessThan(ratio);
}