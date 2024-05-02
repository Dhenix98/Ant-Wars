#include "CStageLoad.h"

int CStageLoad::m_AvailableButtonID = 0;

CStageLoad::CStageLoad(MapPtr map, const DatabasePtr &database) :
	CStage(map)
{
	// shared textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU;


		m_Buttons.emplace(m_IDLoadButton, CButton(780, 350, 354, 78, database->getTexture(BTN_AVAILABLE), database->getTexture(BTN_NOT_AVAILABLE),
			m_ButtonsFont, "LOAD"));

		m_Buttons.emplace(m_IDDeleteButton, CButton(780, 440, 354, 78, database->getTexture(BTN_AVAILABLE), database->getTexture(BTN_NOT_AVAILABLE),
			m_ButtonsFont, "DELETE"));

		m_Buttons.emplace(m_IDExitButton, CButton(780, 530, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "EXIT"));

		createSaveFileButtons(database);
		verifySaveFiles();

		m_SelectedButtonTexture = database->getTexture(SAVE_FILE_SELECTED);
	}

	// only this stage textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU::SAVE;

		m_BackgroundScreen = database->getTexture(SCREEN);
	}

	m_FileSelected = false;
	m_FileSelectedID = 0;
}

void CStageLoad::handlePlayerInput(SDL_Event &eventHandler) {
	SDL_Point mousePosition;
	mousePosition.x = eventHandler.button.x;
	mousePosition.y = eventHandler.button.y;

	// pressing ESC
	if (eventHandler.type == SDL_KEYUP && eventHandler.key.repeat == 0) {
		if (eventHandler.key.keysym.sym == SDLK_ESCAPE) {
			CCustomEvents::sendNextStage(CStage::prevStage);
			update();
		}
	}
	// left click
	if (eventHandler.type == SDL_MOUSEBUTTONUP && eventHandler.button.button == SDL_BUTTON_LEFT) {

		// CHOICE BUTTONS

		if (getButton(m_IDLoadButton).mouseInActiveHitbox(mousePosition)) {
			loadFile();
		}

		else if (getButton(m_IDDeleteButton).mouseInActiveHitbox(mousePosition)) {
			removeSelSaveFile();
			update();
		}

		else if (getButton(m_IDExitButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(prevStage);
		}

		// SAVE FILE BUTTON
		else {
			for (size_t i = 0; i < m_SaveFileButtons.size(); ++i) {
				if (m_SaveFileButtons[i].mouseInHitbox(mousePosition)) {
					pressedFile(static_cast<unsigned int>(i));
				}
			}

		}
	}
}

void CStageLoad::render() const {
	m_BackgroundScreen->render(0, 0);
	for (const auto &x : m_Buttons) {
		x.second.render();
	}
	for (size_t i = 0; i < m_SaveFileButtons.size(); ++i) {
		m_SaveFileButtons[i].render();
	}
	if (m_FileSelected) {
		m_SelectedButtonTexture->render(m_SaveFileButtons[m_FileSelectedID].getX(), m_SaveFileButtons[m_FileSelectedID].getY());
	}
}

void CStageLoad::update() {
	verifySaveFiles();

	m_FileSelected = false;
	m_FileSelectedID = 0;
	getButton(m_IDLoadButton).setActive(false);
	getButton(m_IDDeleteButton).setActive(false);
}

void CStageLoad::removeSelSaveFile() {
	std::string path = CMap::savePath(m_FileSelectedID);
	remove(path.c_str());
}

void CStageLoad::loadFile() {

	std::ifstream saveFile(CMap::savePath(m_FileSelectedID));
	if (saveFile.fail()) {
		throw CorruptedFileException("File couldn't be opened");
	}
	if (m_Map->loadMapFromSaveFile(saveFile)) {
		m_Map->m_GameRunning = true;
		CCustomEvents::sendNextStage(EStage::GAME);
	}
	else {
		m_Map->m_GameRunning = false;
	}
	saveFile.close();
}

void CStageLoad::pressedFile(unsigned int choice) {
	m_FileSelected = true;
	m_FileSelectedID = choice;
	bool result = m_SaveFileButtons[m_FileSelectedID].isActive();
	getButton(m_IDLoadButton).setActive(result);
	getButton(m_IDDeleteButton).setActive(result);
}

