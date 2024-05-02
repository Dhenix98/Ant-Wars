#pragma once
#include <memory>
#include <algorithm>
#include <ctime>
#include "CMap.h"
#include "EColor.h"
#include "EDifficulty.h"
#include "CHive.h"

/// @brief AI controls movement of hives with AI's color
class CAI {
public:
	CAI(MapPtr map);
	/// @brief do action/move if enough time has passed
	void doMove();

	EColor getColor() const;
	void setColor(EColor color);
	void setDifficulty(EDifficulty aiDifficulty);
	void resetTimer();

private:
	/// @brief true if enough time has passed
	bool timer(Uint32 delay);
	/// @brief actions for easy AI
	void easyAI();
	/// @brief actions for medium AI
	void mediumAI();
	/// @brief actions for hard AI
	void hardAI();

	/// @brief if action wasn't already taken then
	/// attacks random(and available) enemy hive
	void attackSemiRandom();
	/// @brief if action wasn't already taken then
	/// defend itself if under attack
	void defendSelf();
	/// @brief if action wasn't already taken then
	/// reinforce its weaker hives
	void reinforceWeakerHives();
	/// @brief if action wasn't already taken then
	/// ai attacks with its strongest hive the most weak enemy hive
	void attackWeakerHives();
	/// @brief if action wasn't already taken then
	/// retreats army from targeted hive
	void retreatSupport();
	/// @brief remove from vector hives which have lesser ratio than parameter ratio
	static void trimHivesByRatio(std::vector<HivePtr> &hives, double ratio);
	static void orderBySize(std::vector<HivePtr> &hives, bool descending);
	/// @brief select strongest(=biggest size) hive and attack the weakest enemy hive, if not available than attack the second weakest,
	/// if the hive has 2 clash participations then second strongest hive is selected and so on...
	void strongestAttackWeakest(std::vector<HivePtr> &aiHives, std::vector<HivePtr> &enemyHives, unsigned int maxParticipations = 2);
	/// @brief create random clash between hives from sources and targets
	void createRandomClash(std::vector<HivePtr> &sources, std::vector<HivePtr> &targets, const unsigned int maxParticipations = 2);
	/// @brief divide all hives between ai hives and other hives
	void divideAllHives(std::vector<HivePtr> &aiHives, std::vector<HivePtr> &enemyHives) const;
	/// @return random number between 0,1...(mod-1), mod must not be 0!
	static unsigned int randomNumberInRange(unsigned int mod);
	struct hiveRetreatInfo;
	/// @brief select candidates for retreat and put them into hiveRetreatInfo struct
	void getCandidate(std::vector<hiveRetreatInfo> &candidates, HivePtr candidate, bool receiver) const;

	MapPtr m_Map;
	bool m_ActionTaken = false;
	EColor m_Color;
	Uint32 m_Time;
	EDifficulty m_Difficulty;

	/// @brief structure used in getCandidate
	struct hiveRetreatInfo {
		hiveRetreatInfo(HivePtr hive, ClashPtr clash, bool sender);
		HivePtr m_Hive;
		ClashPtr m_Clash;
		bool m_Sender;
	};
};
