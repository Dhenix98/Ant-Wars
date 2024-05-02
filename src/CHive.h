#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include <vector>
#include <set>
#include "EColor.h"
#include "CClickableObject.h"
#include "CDatabase.h"

/// @brief hive contains ants, has capacity and regenerates its ants
class CHive : public CClickableObject {
public:
	CHive(unsigned int id, int x, int y, CTexture const *texture, CTexture const *highlightedTexture, EColor color, unsigned int numOfAnts, unsigned int capacity, TTF_Font *font);

	/// @return true if ids are equal
	bool operator==(const CHive &oth);

	/// @brief regenerate its ants and update size text
	void contAction();
	/// @brief render hive
	void render() const override;
	/// @brief un/highlight hive (changes active textures)
	void setHighlight(bool highlighted);
	/// @brief select hive (changes active texture)
	/// difference between select and highlight is that
	/// select can only be unselected by player and
	/// highlight "unhighlights" itself when the mouse isn't hovering over its location
	void setSelected(bool selected);
	bool isSelected() const;
	/// @return true when the hive is captured which means that attack wave has more ants than defending hive
	bool loseAnts(const unsigned int number);
	/// @return true when the hive is has more ant then it can store => ants over the limit are lost
	bool gainAnts(const unsigned int number);
	/// @return true if point with x and y values is in hitbox of the hive
	bool collision(int x, int y) const;
	/// @brief update textures according to color value
	void updateTexture(const DatabasePtr &database);
	/// @return data string converted from this hive info
	std::string toString(std::string player) const;

	EColor getColor() const;
	bool getUpdateOwner() const;
	std::pair<int, int> getCentre() const;
	unsigned int getNumOfAnts() const;
	unsigned int getCapacity() const;
	int getX() const;
	int getY() const;
	unsigned int getID() const;

	void setNewColor(EColor color);
	/// @brief assign to m_Color m_NewColor
	void applyNewColor();
	void setUpdateOwner(bool update);

	/// @param ratio should be between 0-1 to simulate 0%-100%
	/// @return true if size/capacity ratio is less than parameter ratio
	bool isCapacityRatioLessThan(double ratio) const;
	/// @param ratio should be between 0-1 to simulate 0%-100%
	/// @return true if size/capacity ratio is greater than parameter ratio
	bool isCapacityRatioGreaterThan(double ratio) const;

private:
	/// @brief render size text and calculates its offset (so it would be centered)
	void renderSizeInfo() const;

	unsigned int m_ID;
	EColor m_Color;
	EColor m_NewColor;
	unsigned int m_Size;
	unsigned int m_Capacity;

	CTexture const *m_HighlightedTexture;
	bool m_Highlighted = false;
	bool m_Selected = false;

	CTexture m_SizeTexture;
	TTF_Font *m_SizeFont;


	Uint32 m_HiveDelay;
	const Uint32 m_RegenerationRate = 1000;

	/// @brief if true then handled in higher logic layer
	bool m_updateOwner = false;
};

using HivePtr = std::shared_ptr<CHive>;
