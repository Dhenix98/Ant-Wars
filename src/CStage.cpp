#include "CStage.h"

CStage::~CStage() {}

void CStage::update() {}

void CStage::doEvents() {}

EStage CStage::prevStage = EStage::MENU;

CStage::CStage(MapPtr map) :
	m_Map(map),
	m_ButtonsFont(map->m_Database->getFont(DATABASE_ID::FONT_ID::BUTTON)),
	m_SaveFileButtonsFont(map->m_Database->getFont(DATABASE_ID::FONT_ID::SAVE_FILE))
{}


CButton &CStage::getButton(int id) {
	return m_Buttons.find(id)->second;
}

SDL_Color CStage::getSDLColorByDifficulty(EDifficulty difficulty) {
	// returns green
	if (difficulty == EDifficulty::EASY) {
		return toSDLColor(EColor::GREEN_DIFFICULTY);
	}
	// returns orange
	if (difficulty == EDifficulty::MEDIUM) {
		return toSDLColor(EColor::ORANGE_DIFFICULTY);
	}
	// returns red
	if (difficulty == EDifficulty::HARD) {
		return toSDLColor(EColor::RED_DIFFICULTY);
	}
	throw NotImplementedException("This difficulty is not implemented in \"getSDLColorByDifficulty(EDifficulty difficulty)\"");
}

EColor CStage::getColorByDifficulty(EDifficulty difficulty) {
	// returns green
	if (difficulty == EDifficulty::EASY) {
		return EColor::GREEN_DIFFICULTY;
	}
	// returns orange
	if (difficulty == EDifficulty::MEDIUM) {
		return EColor::ORANGE_DIFFICULTY;
	}
	// returns red
	if (difficulty == EDifficulty::HARD) {
		return EColor::RED_DIFFICULTY;
	}
	throw NotImplementedException("This difficulty is not implemented in \"getColorByDifficulty(EDifficulty difficulty)\"");
}

bool CStage::findAndInsertSaveFileName(unsigned int num, std::string &fileName) {
	std::ifstream loadedSave(CMap::savePath(num));
	if (loadedSave.fail()) {
		fileName = "Empty";
		loadedSave.close();
		return false;
	}
	std::string tmpFileName;
	std::getline(loadedSave, tmpFileName);
	if (tmpFileName == "" || tmpFileName.size() > 20) {
		fileName = "Empty";
		loadedSave.close();
		return false;
	}
	fileName = tmpFileName;
	loadedSave.close();
	return true;
}

void CStage::createSaveFileButtons(const DatabasePtr &database) {
	using namespace DATABASE_ID::TEXTURE_ID::MENU;

	for (unsigned int i = 0; i < m_NumOfSaves; ++i) {
		m_SaveFileButtons.push_back(CButton(50, getSaveFileCoordY(i), 571, 78, database->getTexture(SAVE_FILE_AVAIL),
			database->getTexture(SAVE_FILE_NOT_AVAIL), m_SaveFileButtonsFont, "EMPTY"));
	}
}

void CStage::verifySaveFiles() {

	for (unsigned int i = 0; i < m_NumOfSaves; ++i) {
		std::string tmpFileName;

		m_SaveFileButtons[i].setActive(findAndInsertSaveFileName(i, tmpFileName));
		m_SaveFileButtons[i].changeText(tmpFileName);
	}
}

int CStage::getSaveFileCoordY(unsigned int i) {
	return 50 + static_cast<int>(i) * 90;
}

void CStage::resetMap() {
	std::ifstream loadedMap(CMap::levelPath(m_Map->getCurrLevel()));
	m_Map->loadMapFromMapFile(loadedMap);
	loadedMap.close();
}