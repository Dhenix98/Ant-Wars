#include "CMap.h"

// PUBLIC

CMap::CMap(DatabasePtr database) :
	m_Database(database),
	m_Clashes(cmpSharedPtrClash),
	m_NumberOfPlayerHives(0),
	m_CurrLevel(0),
	m_Difficulty(EDifficulty::MEDIUM),
	m_PlayerColor(EColor::BLUE_HIVE),
	m_FirstEnemyColor(EColor::RED_HIVE),
	m_SecondEnemyColor(EColor::GREEN_HIVE),
	m_NeutralColor(EColor::GRAY_HIVE)
{}

bool CMap::newGameMap(EColor playerColor, EDifficulty difficulty) {
	setPlayerColor(playerColor);
	setHiveColors();
	setDifficulty(difficulty);
	m_CurrLevel = 0;
	m_GameRunning = true;

	return loadMap();
}

bool CMap::resetMap() {
	return loadMap();
}

bool CMap::loadMap() {
	std::ifstream loadedMap(CMap::levelPath(m_CurrLevel));
	if (loadedMap.fail()) {
		return false;
	}
	bool result = loadMapFromMapFile(loadedMap);
	loadedMap.close();
	return result;
}

bool CMap::loadMapFromMapFile(std::ifstream &dataStream) {
	bool result = true;
	std::map<unsigned int, std::pair<int, int>> objectLocations;
	clear();

	try {
		loadMapTiles(dataStream, objectLocations);
		loadHivesInfo(dataStream, objectLocations);
	}
	catch (CorruptedFileException &e) {
		std::cerr << e.what() << std::endl;
		result = false;
	}
	catch (NotImplementedException &e) {
		std::cerr << e.what() << std::endl;
		result = false;
	}
	return result;
}

bool CMap::loadMapFromSaveFile(std::ifstream &dataStream) {
	bool result = true;
	std::map<unsigned int, std::pair<int, int>> objectLocations;
	clear();
	try {
		trimOutSaveFileName(dataStream);
		loadMapTiles(dataStream, objectLocations);
		loadPlayerInfo(dataStream);
		loadHivesInfo(dataStream, objectLocations);
		loadClashInfo(dataStream);
	}
	catch (CorruptedFileException &e) {
		std::cerr << e.what() << std::endl;
		result = false;
	}
	catch (NotImplementedException &e) {
		std::cerr << e.what() << std::endl;
		result = false;
	}
	return result;
}


std::string CMap::toString() const {
	std::string result;
	result += saveMapTiles();
	result += '\n';
	result += std::string(toCharArray(m_PlayerColor)) +
		" " + toCharArray(m_Difficulty) +
		" " + std::to_string(m_CurrLevel) + "\n" + "\n";

	for (const auto &x : m_Hives) {
		result += x.second->toString(colorToCodeString(x.second->getColor()));
	}

	result += '\n';

	for (const auto &x : m_Clashes) {
		result += x->toString();
	}

	return result;
}


std::string CMap::saveMapTiles() const {
	std::string result;
	std::vector<std::vector<char>> tiles(32);
	for (auto &x : tiles) {
		x.resize(18, '-');
	}

	// write hives into tiles
	for (const auto &x : m_Hives) {
		unsigned int hiveX = x.second->getX() / 40;
		unsigned int hiveY = x.second->getY() / 40;

		for (unsigned int i = 0; i < 2; ++i) {
			for (unsigned int j = 0; j < 2; ++j) {
				tiles[hiveX + i][hiveY + j] = static_cast<char>('0' + x.first);
			}
		}
	}

	for (const auto &x : m_Walls) {
		unsigned int hiveX = x.getX() / 40;
		unsigned int hiveY = x.getY() / 40;
		tiles[hiveX][hiveY] = 'X';

	}

	for (unsigned int j = 0; j < 18; ++j) {
		for (unsigned int i = 0; i < 32; ++i) {
			result += tiles[i][j];
		}
		result += '\n';
	}
	return result;
}

bool CMap::playerOwnsAllHives() const {
	for (const auto &x : m_Hives) {
		if (x.second->getColor() != m_PlayerColor && x.second->getColor() != EColor::GRAY_HIVE) {
			return false;
		}
	}
	return true;
}

bool CMap::playerDoesntOwnAnyHives() const {
	for (const auto &x : m_Hives) {
		if (x.second->getColor() == m_PlayerColor) {
			return false;
		}
	}
	return true;
}

// PRIVATE

void CMap::clear() {
	m_Clashes.clear();
	m_Hives.clear();
	m_Walls.clear();
	m_NumberOfPlayerHives = 0;
}

std::string CMap::levelPath(unsigned int levelNum) {
	std::string filePath = REL_MAP_FILE_PATH;
	filePath += "/level";
	if (levelNum < 10) {
		filePath += "0";
	}
	filePath += std::to_string(levelNum);
	filePath += ".map";
	return filePath;
}

std::string CMap::savePath(unsigned int levelNum) {
	return REL_SAVE_FILE_PATH + "/" + saveName(levelNum);
}

std::string CMap::saveName(unsigned int levelNum) {
	std::string fileName = "save";
	if (levelNum < 10) {
		fileName += "0";
	}
	fileName += std::to_string(levelNum);
	fileName += ".save";
	return fileName;
}


void CMap::trimOutSaveFileName(std::ifstream &loadedMap) {
	std::string data;
	// get rid of file name
	std::getline(loadedMap, data);
	// get rid of new line
	std::getline(loadedMap, data);
}

void CMap::loadPlayerInfo(std::ifstream &loadedMap) {
	// file variables
	std::string data;
	std::getline(loadedMap, data);
	if (loadedMap.fail() || loadedMap.eof()) {
		throw CorruptedFileException("File corruption at playerInfo section");
	}
	std::istringstream rowStream(data);

	std::string color, difficulty;

	rowStream >> color >> difficulty >> m_CurrLevel;
	if (rowStream.fail() || !rowStream.eof()) {
		throw CorruptedFileException("File corruption at playerInfo section");
	}

	m_PlayerColor = toColor(color);
	m_Difficulty = toDifficulty(difficulty);
	setHiveColors();
	// get rid off new line
	std::getline(loadedMap, data);
}

void CMap::loadHivesInfo(std::ifstream &loadedMap, const std::map<unsigned int, std::pair<int, int>> &objectLocations) {
	std::string data;
	while (true) {
		std::getline(loadedMap, data);
		std::istringstream rowStream(data);

		if (rowStream.eof() || data.empty()) {
			if (objectLocations.size() != m_Hives.size()) {
				throw CorruptedFileException("File corruption at hive info section");
			}
			break;
		}

		unsigned int id, size, capacity;
		std::string type;

		rowStream >> id >> type >> size >> capacity;
		if (rowStream.fail() || !rowStream.eof()) {
			throw CorruptedFileException("File corruption at hive info section");
		}

		EColor objColor = codeStringToColor(type);
		if (objColor == m_PlayerColor) {
			++m_NumberOfPlayerHives;
		}
		std::pair<CTexture const *, CTexture const *> textures = getHiveTexturesByColor(objColor);
		{
			using namespace DATABASE_ID::FONT_ID;
			if (objectLocations.find(id) == objectLocations.end()) {
				throw CorruptedFileException("File corruption at finding hives");
			}
			int x = objectLocations.find(id)->second.first;
			int y = objectLocations.find(id)->second.second;

			// to make sure that hive are centered
			int offset = 5;
			if (m_Hives.find(id) != m_Hives.end()) {
				throw CorruptedFileException("File corruption at too many hives with the same ID");
			}
			m_Hives.emplace(id, std::make_shared<CHive>(CHive(id, x * 40 + offset, y * 40 + offset,
				textures.first, textures.second, objColor, size, capacity, m_Database->getFont(HIVE_SIZE))));
		}
	}
}

void CMap::loadClashInfo(std::ifstream &loadedMap) {
	std::string data;
	while (true) {
		std::getline(loadedMap, data);
		std::istringstream rowStream(data);

		if (data.empty()) {
			break;
		}

		int firstHiveID, secondHiveID, clashState, firstHiveCurrX, firstHiveCurrY, secondHiveCurrX, secondHiveCurrY;
		std::string type;

		rowStream >> firstHiveID >> secondHiveID >> clashState >> firstHiveCurrX >> firstHiveCurrY >> secondHiveCurrX >> secondHiveCurrY;
		if (rowStream.fail() || !rowStream.eof() || clashState < 0 || clashState > 3) {
			throw CorruptedFileException("File corruption at clash section");
		}
		std::shared_ptr<CClash> tmpClash = std::make_shared<CClash>(CClash(getHiveById(firstHiveID), getHiveById(secondHiveID), clashState,
			firstHiveCurrX, firstHiveCurrY, secondHiveCurrX, secondHiveCurrY));

		m_Clashes.insert(tmpClash);
	}
}

void CMap::loadMapTiles(std::ifstream &loadedMap, std::map<unsigned int, std::pair<int, int>> &objectLocations) {
	const int tileSize = 40;
	const int maxRowCount = SCREEN_HEIGHT / tileSize;
	const int maxColumnCount = SCREEN_WIDTH / tileSize;

	std::string data;
	int rowNum = 0;
	std::multiset<unsigned int> numOfIds;

	while (true) {
		std::getline(loadedMap, data);

		// column length is not expected number or end of tiles
		if (rowNum == maxRowCount) {
			if (!data.empty()) {
				throw CorruptedFileException("File corruption at incorrect number of rows in map tiles");
			}
			break;
		}

		// row length is not expected number
		if (data.size() != static_cast<unsigned int>(maxColumnCount)) {
			throw CorruptedFileException("File corruption at incorrect length of the row in map tiles");
		}

		for (int i = 0; i < maxColumnCount; ++i) {
			if (data[i] == '-') {
				continue;
			}
			else if (data[i] <= '9' && data[i] >= '0') {
				unsigned int id = data[i] - '0';
				// wasn't already in map
				if (objectLocations.emplace(id, std::pair<int, int>(i, rowNum)).second) {
					numOfIds.insert(id);
				}
				else {
					if ((i != objectLocations[id].first) && (i != (objectLocations[id].first + 1))) {
						throw CorruptedFileException("File corruption at hive is not 2*2");
					}
					else if ((rowNum != objectLocations[id].second) && (rowNum != (objectLocations[id].second + 1))) {
						throw CorruptedFileException("File corruption at hive is not 2*2");
					}
					numOfIds.insert(id);
				}
			}
			else if (data[i] == 'X') {
				m_Walls.push_back(CWall(static_cast<int>(i * tileSize), static_cast<int>(rowNum * tileSize)));
			}
		}
		rowNum++;
	}
	// check if all hives are 2*2
	for (const auto &x : objectLocations) {
		if (numOfIds.count(x.first) != 4) {
			throw CorruptedFileException("File corruption at hive not being 2*2");
		}
	}
	// check if at least some hives exists
	if (objectLocations.empty()) {
		throw CorruptedFileException("File corruption at no hive existence");
	}
}

std::pair<CTexture const *, CTexture const *> CMap::getHiveTexturesByColor(EColor color) {
	using namespace DATABASE_ID::TEXTURE_ID::GAME;
	std::pair<CTexture const *, CTexture const *> result;
	switch (color) {
		case EColor::BLUE_HIVE: {
			result = std::make_pair(m_Database->getTexture(BLUE_HIVE), m_Database->getTexture(HIGHLIGHTED_BLUE_HIVE));
			break;
		}
		case EColor::GREEN_HIVE: {
			result = std::make_pair(m_Database->getTexture(GREEN_HIVE), m_Database->getTexture(HIGHLIGHTED_GREEN_HIVE));
			break;
		}
		case EColor::RED_HIVE: {
			result = std::make_pair(m_Database->getTexture(RED_HIVE), m_Database->getTexture(HIGHLIGHTED_RED_HIVE));
			break;
		}
		default: {
			result = std::make_pair(m_Database->getTexture(GRAY_HIVE), m_Database->getTexture(HIGHLIGHTED_GRAY_HIVE));
		}
	}
	return result;
}

void CMap::setHiveColors() {
	if (m_PlayerColor == EColor::BLUE_HIVE) {
		m_FirstEnemyColor = EColor::RED_HIVE;
		m_SecondEnemyColor = EColor::GREEN_HIVE;
	}
	else if (m_PlayerColor == EColor::GREEN_HIVE) {
		m_FirstEnemyColor = EColor::RED_HIVE;
		m_SecondEnemyColor = EColor::BLUE_HIVE;
	}
	else {
		m_FirstEnemyColor = EColor::BLUE_HIVE;
		m_SecondEnemyColor = EColor::GREEN_HIVE;
	}
}

void CMap::setPlayerColor(EColor playerColor) {
	m_PlayerColor = playerColor;
}

EColor CMap::getPlayerColor() const {
	return m_PlayerColor;
}

EColor CMap::getFirstEnemyColor() const {
	return m_FirstEnemyColor;
}

EColor CMap::getSecondEnemyColor() const {
	return m_SecondEnemyColor;
}

EDifficulty CMap::getDifficulty() const {
	return m_Difficulty;
}

void CMap::setDifficulty(EDifficulty difficulty) {
	m_Difficulty = difficulty;
}

unsigned int CMap::getCurrLevel() const {
	return m_CurrLevel;
}

void CMap::lowerDifficulty() {
	int newDifficultyValue = static_cast<int>(m_Difficulty) - 1;
	newDifficultyValue = std::max(0, newDifficultyValue);

	m_Difficulty = static_cast<EDifficulty>(newDifficultyValue);
}


std::string CMap::colorToCodeString(EColor color) const {
	if (color == m_PlayerColor) {
		return "CP";
	}
	if (color == m_FirstEnemyColor) {
		return "CE1";
	}
	if (color == m_SecondEnemyColor) {
		return "CE2";
	}
	if (color == m_NeutralColor) {
		return "CN";
	}
	throw CorruptedFileException("Should not happen! => colorToCodeString()");
}

EColor CMap::codeStringToColor(std::string type) const {
	EColor result;

	if (type.compare("CP") == 0) {
		result = m_PlayerColor;
	}
	else if (type.compare("CE1") == 0) {
		result = m_FirstEnemyColor;
	}
	else if (type.compare("CE2") == 0) {
		result = m_SecondEnemyColor;
	}
	else if (type.compare("CN") == 0) {
		result = m_NeutralColor;
	}
	else {
		throw CorruptedFileException("File corruption at hive/player color");
	}
	return result;
}


const HivePtr &CMap::getHiveById(unsigned int id) const {
	return m_Hives.find(id)->second;
}

void CMap::incrCurrLevel() {
	++m_CurrLevel;
}

bool CMap::existObstacle(const HivePtr &firstHive, const HivePtr &secondHive) {
	CLine testLine(0, firstHive->getCentre(), secondHive->getCentre(), EColor::BLUE_HIVE);
	testLine.setTargetToEnd();
	bool foundObstacle = false;

	while (!foundObstacle && !testLine.hasReachedTarget()) {
		testLine.movement();
		for (const auto &x : m_Walls) {
			if (x.collision(testLine.getCurrCoordX(), testLine.getCurrCoordY())) {
				foundObstacle = true;
				break;
			}
		}
		for (const auto &x : m_Hives) {
			if (x.second->getID() == firstHive->getID() || x.second->getID() == secondHive->getID()) {
				continue;
			}

			if (x.second->collision(testLine.getCurrCoordX(), testLine.getCurrCoordY())) {
				foundObstacle = true;
				break;
			}
		}
	}

	return foundObstacle;
}

unsigned int CMap::howManyParticipations(const HivePtr &hive) const {
	unsigned int result = 0;
	for (const auto &x : m_Clashes) {
		if (x->hiveParticipates(hive)) {
			++result;
		}
	}


	return result;
}

void CMap::createClash(HivePtr firstHive, HivePtr secondHive) {
	auto it = findClash(firstHive, secondHive);
	if (it != m_Clashes.end()) {
		it->get()->sendArmy(firstHive);
	}
	else {
		HivePtr lowerIdHive = firstHive->getID() < secondHive->getID() ? firstHive : secondHive;
		HivePtr higherIdHive = firstHive->getID() < secondHive->getID() ? secondHive : firstHive;

		auto tmp = std::make_shared<CClash>(CClash(lowerIdHive, higherIdHive));
		tmp->sendArmy(firstHive);
		m_Clashes.insert(tmp);
	}
}

void CMap::removeClashes(const HivePtr &hive) {
	for (auto &x : m_Clashes) {
		if (x->hiveParticipates(hive)) {
			x->instantRetreatArmy(hive);
			x->updateLineColor(hive);
		}
	}
}

std::set<ClashPtr, decltype(cmpSharedPtrClash) * >::iterator CMap::findClash(const HivePtr &firstHive, const HivePtr &secondHive) {
	const HivePtr &lowerIdHive = firstHive->getID() < secondHive->getID() ? firstHive : secondHive;
	const HivePtr &higherIdHive = firstHive->getID() > secondHive->getID() ? firstHive : secondHive;
	return m_Clashes.find(std::make_shared<CClash>(lowerIdHive, higherIdHive));
}
