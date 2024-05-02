#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include <vector>
#include <cmath>
#include "CClickableObject.h"
#include "EColor.h"
#include "globalVar.h"
#include "CPoint.h"

/// @brief line between two point(in this case hives)
class CLine : CClickableObject {
public:
	CLine(unsigned int id, int xBegin, int xTarget, int yBegin, int yTarget, EColor color);
	CLine(unsigned int id, std::pair<int, int> begin, std::pair<int, int> end, EColor color);

	void render() const override;
	bool mouseInHitbox(const SDL_Point &mousePosition) const override;

	/// @brief draw line between two points
	static void drawLine(const CPoint &a, const CPoint &b, SDL_Renderer *renderer, int offset = 0);

	/// @brief gradually move towards the target, movement speed is affected by m_LineDrawDelay
	void gradualMovement();
	/// @brief movs line by one tile towards the target (or none if the target is reached)
	void movement();
	/// @brief move instantly towards the target (used in loading)
	void instantMoveToTarget();

	/// @brief step horizontally towards the target
	void stepTowardsXTarget();
	/// @brief step vertically towards the target
	void stepTowardsYTarget();
	/// @brief step toward the target by 1 "length"
	void stepTowardsXAndYTarget();
	/// @return true if current coord equal target coords
	bool hasReachedTarget() const;

	/// @brief set target to source hive
	void setTargetToBegin();
	/// @brief set target to middle of source and target hive
	void setTargetToMiddle();
	/// @brief set target to target hive
	void setTargetToEnd();
	/// @brief set target manually
	void setTarget(int x, int y);
	/// @brief set line to be highligted/unhighlighted
	void setHighlight(bool choice);
	/// @return true if the line target equals to its beginning
	bool toBeDeleted() const;
	/// @return data string converted from this line info
	std::string toString() const;

	int getCurrCoordX() const;
	int getCurrCoordY() const;

	EColor getColor() const;
	void setColor(EColor color);

private:
	friend class CClash;

	/// @brief calculate middle of two values
	static int middle(int a, int b);

	unsigned int m_ID;

	const int m_XBegin, m_XTarget, m_YBegin, m_YTarget;
	int m_XCurrent, m_XCurrentTarget, m_YCurrent, m_YCurrentTarget;

	/// @brief true if target x is less than source x
	bool m_NegativeX;
	/// @brief true if target y is less than source y
	bool m_NegativeY;

	double m_CurrLength, m_CurrTargetLength, m_MaxTargetLength;

	bool m_VerticalLine;
	bool m_HorizontalLine;

	double m_SinAlpha;
	double m_CosAlpha;

	std::vector<CPoint> m_Points;
	EColor m_Color;
	bool m_Highlighted;
	const EColor m_HighlightedColor = EColor::WHITE;

	/// @brief drawing speed
	const Uint32 m_DrawSpeed = 10;
	/// @brief contains last time the line was drawn
	Uint32 m_LineDrawDelay;
	const int m_StepSize = 1;
	const int m_LineWidth = 2;
};
