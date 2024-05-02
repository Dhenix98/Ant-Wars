#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include "CClickableObject.h"
#include "CHive.h"
#include "CLine.h"
#include <math.h>
#include "CWall.h"

class CClash;
/// @brief used for CClash compare
bool cmpSharedPtrClash(const std::shared_ptr<CClash> &lhs, const std::shared_ptr<CClash> &rhs);

/// @brief CClash handles attacks/support between hives
/// controls 2 lines between two points(hives), sets their targets and handles ant death/gain rates
class CClash {
public:
	CClash(HivePtr firstHive, HivePtr secondHive);
	CClash(HivePtr firstHive, HivePtr secondHive, int state,
		int currentXFirst, int currentYFirst, int currentXSecond, int currentYSecond);

	/// @brief gradually move lines to its targets and if there is clash calls other methods
	void contAction();
	/// @brief render two lines(dots)
	void render() const;
	/// @return true if player clicked line and makes the line retreat
	bool playerClickedLine(const SDL_Point &mousePosition, EColor playerColor);
	/// @return true if player moved over line and highlights the lines if it belongs to player
	void playerMovedOverLine(const SDL_Point &mousePosition, EColor playerColor);
	/// @brief highlight value to line with equal color as playerColor
	void setHighlight(bool highlight, EColor playerColor);

	/// @brief hive sends its army
	void sendArmy(const HivePtr &hive);
	/// @brief hive retreats its army
	void retreatArmy(const HivePtr &hive);
	/// @brief hive's target retreats their army
	void retreatOtherArmy(const HivePtr &hive);
	/// @brief instant army retreat / "army death" (used when hive is captured)
	void instantRetreatArmy(const HivePtr &hive);
	/// @brief calculate losses on both sides when the lines/armies meet, when one hive has no more army then that hive is captured
	void hiveAttacks();
	/// @brief calculate losses/gains on both sides when the line meets hive
	void hiveSupport();
	/// @brief set line targets by current state of armies (hives or middle)
	void setTargetsByCurrClashState();
	/// @brief set line targets manually (used in loading)
	void setTargetsByCurrCoords(int currentXFirst, int currentYFirst, int currentXSecond, int currentYSecond);

	/// @brief update line color according its source hive (used when the hive changes its owner)
	void updateLineColor(const HivePtr &hive);
	/// @return true when both armies are at their source hives
	bool shouldBeDeleted() const;
	/// @return true this hive participates in this clash
	bool hiveParticipates(const HivePtr &hive) const;
	/// @return true if this hive is sending its army
	bool isHiveActive(const HivePtr &hive) const;
	/// @return true if both hives have the same color
	bool bothHiveHaveSameColor() const;
	/// @return data string converted from this clash info
	std::string toString() const;
	/// @brief convert state value into bool "XXXXXHiveLineActive" values
	void setLineStatesByValue(int state);

	/// @brief orders by lower id and then other id
	bool operator < (const CClash &oth);

	/// @brief attack speed of hives (not movement speed!)
	const Uint32 m_AttackRate = 400;
	/// @brief contains time of the last attack
	Uint32 m_AttackDelay;

	const unsigned int m_AdvantageAttackRate = 3;
	const unsigned int m_NormalAttackRate = 1;

	bool onlyFirstIsActive() const;
	bool onlySecondIsActive() const;
	bool noneIsActive() const;
	bool bothAreActive() const;

	HivePtr m_FirstHive;
	HivePtr m_SecondHive;

private:
	/// @return order number (1 or 2), if this hive is in this clash, if it isn't (which should not happen) then throws exception
	size_t getHiveNum(const HivePtr &hive) const;
	/// @brief convert 1 -> 2 or 2 -> 1 or if hiveNum isn't 1 or 2 then throw exception
	/// @return 1 or 2
	static size_t invertHiveNum(size_t hiveNum);

	HivePtr &getHive(size_t hiveNum);
	CLine &getHiveLine(size_t hiveNum);
	bool getHiveLineActive(size_t hiveNum) const;
	void setHiveLineActive(size_t hiveNum, bool choice);

	CLine m_FirstHiveLine;
	CLine m_SecondHiveLine;

	bool m_FirstHiveLineActive = false;
	bool m_SecondHiveLineActive = false;

	bool m_Highlighted = false;
};

using ClashPtr = std::shared_ptr<CClash>;
