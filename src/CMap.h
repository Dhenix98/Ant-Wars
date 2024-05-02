#pragma once
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cassert>
#include "globalVar.h"
#include "CHive.h"
#include "CClash.h"
#include "CDatabase.h"
#include "EColor.h"
#include "EDifficulty.h"
#include "CCustomEvents.h"
#include "Exceptions.h"
#include "CWall.h"

/// @brief represents map
/// stores all objects (hives, clashes, walls)
class CMap {
public:
	CMap(DatabasePtr database);

	/// @brief load level00.map and sets colors and difficulty
	/// @return true if the map was succesfully loaded
	bool newGameMap(EColor playerColor, EDifficulty difficulty);

	/// @brief load map based on current level
	/// @return true if map was sucessfully loaded
	bool loadMap();
	/// @brief reload map
	/// @return true if map was sucessfully loaded
	bool resetMap();

	/// @return data string converted from this map
	std::string toString() const;

	/// @brief take data from dataStream and create from it map objects
	/// @return true if map was sucessfully loaded
	bool loadMapFromMapFile(std::ifstream &dataStream);
	/// @brief take data from dataStream and create from it map objects
	/// @return true if save was sucessfully loaded
	bool loadMapFromSaveFile(std::ifstream &dataStream);

	/// @brief increment current level
	void incrCurrLevel();

	/// @return true if player owns all non-neutral hives
	bool playerOwnsAllHives() const;
	/// @return true if player doesn't own any hives
	bool playerDoesntOwnAnyHives() const;

	/// @return true if there aren't any collisions between hives
	bool existObstacle(const HivePtr &firstHive, const HivePtr &secondHive);
	/// @return number of clashes that this hive participates in
	unsigned int howManyParticipations(const HivePtr &hive) const;
	/// @brief create clash(attack/support) between two hives
	void createClash(HivePtr firstHive, HivePtr secondHive);
	/// @brief remove clash(attack/support) between two hives
	void removeClashes(const HivePtr &hive);
	/// @return iterator of the clash in which these two hives participates in
	std::set<ClashPtr, decltype(cmpSharedPtrClash) *>::iterator findClash(const HivePtr &firstHive, const HivePtr &secondHive);
	/// @return hive with id equal to parameter id
	const HivePtr &getHiveById(unsigned int id) const;

	EColor getPlayerColor() const;
	void setPlayerColor(EColor playerColor);

	EColor getFirstEnemyColor() const;
	EColor getSecondEnemyColor() const;

	EDifficulty getDifficulty() const;
	void setDifficulty(EDifficulty difficulty);

	unsigned int getCurrLevel() const;
	void lowerDifficulty();

	/// @brief set all hive colors according to playerColor
	void setHiveColors();

	/// @return unhighlighted and highlighter texture of requested color
	std::pair<CTexture const *, CTexture const *> getHiveTexturesByColor(EColor color);

	DatabasePtr m_Database;
	bool m_GameRunning = false;

	/// @return path and name of this levelNum
	static std::string levelPath(unsigned int levelNum);
	/// @return path and name of this savePath
	static std::string savePath(unsigned int levelNum);
	/// @return name of this savePath
	static std::string saveName(unsigned int levelNum);

private:
	friend class CAI;
	friend class CStageGame;

	std::map<unsigned int, HivePtr> m_Hives;
	std::set<ClashPtr, decltype(cmpSharedPtrClash) *> m_Clashes;
	std::vector<CWall> m_Walls;
	unsigned int m_NumberOfPlayerHives;

	unsigned int m_CurrLevel;
	EDifficulty m_Difficulty;

	EColor m_PlayerColor;
	EColor m_FirstEnemyColor;
	EColor m_SecondEnemyColor;
	EColor m_NeutralColor;

	/// @brief delete all hives, clashes and walls
	void clear();
	/// @brief remove saveFileName from loadedMap
	void trimOutSaveFileName(std::ifstream &loadedMap);
	/// @brief load playerColor, difficulty and levelNum
	void loadPlayerInfo(std::ifstream &loadedMap);
	/// @brief load hive objects into map
	void loadHivesInfo(std::ifstream &loadedMap, const std::map<unsigned int, std::pair<int, int>> &objectLocations);
	/// @brief load clashes into map
	void loadClashInfo(std::ifstream &loadedMap);
	/// @brief load walls and hive locations into map
	void loadMapTiles(std::ifstream &loadedMap, std::map<unsigned int, std::pair<int, int>> &objectLocations);

	/// @return data string containing tiles of map
	std::string saveMapTiles() const;
	/// @brief decode players code to color
	/// @return color
	EColor codeStringToColor(std::string type) const;
	/// @brief encode color to players code
	/// @return player code
	std::string colorToCodeString(EColor color) const;
};

using MapPtr = std::shared_ptr<CMap>;
