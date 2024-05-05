#include "CStageSave.h"

int CStageSave::m_AvailableButtonID = 0;

CStageSave::CStageSave(MapPtr map, const DatabasePtr &database) :
	CStage(map),
	m_CurrTime(SDL_GetTicks())
{
	// shared textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU;

		m_Buttons.emplace(m_IDInputButton, CButton(780, 260, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			database->getFont(DATABASE_ID::FONT_ID::INPUT), defaultInput));

		m_Buttons.emplace(m_IDSaveButton, CButton(780, 350, 354, 78, database->getTexture(BTN_AVAILABLE), database->getTexture(BTN_NOT_AVAILABLE),
			m_ButtonsFont, "SAVE"));

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

void CStageSave::handlePlayerInput(SDL_Event &eventHandler) {
	SDL_Point mousePosition;
	mousePosition.x = eventHandler.button.x;
	mousePosition.y = eventHandler.button.y;

	// pressing ESC
	if (eventHandler.type == SDL_KEYUP && eventHandler.key.repeat == 0) {
		if (eventHandler.key.keysym.sym == SDLK_ESCAPE) {
			CCustomEvents::sendNextStage(CStage::prevStage);
			SDL_StopTextInput();
		}
	}
	// mouse
	if (eventHandler.type == SDL_MOUSEBUTTONUP && eventHandler.button.button == SDL_BUTTON_LEFT) {
		removeAwaitingLine();
		SDL_StopTextInput();
		m_EnabledText = false;
		m_ShowLine = false;

		// CHOICE BUTTONS
		if (getButton(m_IDInputButton).mouseInActiveHitbox(mousePosition)) {
			if (!m_EnabledText && getButton(m_IDInputButton).getText() == defaultInput) {
				getButton(m_IDInputButton).clearText();
			}
			m_EnabledText = true;
			SDL_StartTextInput();
		}

		else if (getButton(m_IDSaveButton).mouseInActiveHitbox(mousePosition)) {
			saveFile();
			update();
		}

		else if (getButton(m_IDDeleteButton).mouseInActiveHitbox(mousePosition)) {
			removeSelSaveFile();
			update();
		}

		else if (getButton(m_IDExitButton).mouseInActiveHitbox(mousePosition)) {
			SDL_StopTextInput();
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
	// keyboard input => special characters
	else if (eventHandler.type == SDL_KEYDOWN && m_EnabledText)
	{
		removeAwaitingLine();

		//Handle backspace
		if (eventHandler.key.keysym.sym == SDLK_BACKSPACE)
		{
			getButton(m_IDInputButton).removeCharacter();
		}
		updateInputFileName();
	}
	// keyboard input
	else if (eventHandler.type == SDL_TEXTINPUT && m_EnabledText) {
		removeAwaitingLine();
		getButton(m_IDInputButton).addCharacter(eventHandler.text.text[0]);
		if (getButton(m_IDInputButton).getText().size() > m_MaxFileName) {
			getButton(m_IDInputButton).removeCharacter();
		}
		updateInputFileName();
	}
}

void CStageSave::render() const {
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

void CStageSave::update() {
	verifySaveFiles();

	m_FileSelected = false;
	m_FileSelectedID = 0;
	getButton(m_IDInputButton).changeText(defaultInput);
	getButton(m_IDSaveButton).setActive(false);
	getButton(m_IDDeleteButton).setActive(false);
	SDL_StopTextInput();
	m_EnabledText = false;
	m_ShowLine = false;
}

void CStageSave::doEvents() {

	updateSaveButton();

	if (m_EnabledText && (SDL_GetTicks() - m_CurrTime > m_ChangeDelay)) {
		m_CurrTime = SDL_GetTicks();
		m_ShowLine = !m_ShowLine;
		if (m_ShowLine) {
			getButton(m_IDInputButton).addCharacter('|');
		}
		else {
			getButton(m_IDInputButton).removeCharacter();
		}
	}
}

bool CStageSave::correctFileName() {
	std::string text = getButton(m_IDInputButton).getText();
	if (m_ShowLine) {
		text.pop_back();
	}

	if (text.empty()) {
		return false;
	}
	if (text == "Empty") {
		return false;
	}
	if (containsOnlyWhitespace(text)) {
		return false;
	}
	if (text == defaultInput) {
		return false;
	}
	return true;
}

bool CStageSave::containsOnlyWhitespace(std::string text) {
	for (const auto &x : text) {
		if (!isspace(x)) {
			return false;
		}
	}
	return true;
}

bool CStageSave::removeAwaitingLine() {
	std::string text = getButton(m_IDInputButton).getText();
	if (!text.empty() && text[text.size() - 1] == '|') {
		getButton(m_IDInputButton).removeCharacter();
		return true;
	}
	return false;
}

void CStageSave::saveFile() {
	try {
		createDirectory();
		removeSelSaveFile();
		createSelSaveFile();
	}
	catch (CorruptedFileException &e) {
		std::cerr << e.what();
	}
}


void CStageSave::updateInputFileName() {
	m_CurrTime = SDL_GetTicks();
	m_ShowLine = true;
	getButton(m_IDInputButton).addCharacter('|');
}

void CStageSave::removeSelSaveFile() {
	std::string path = CMap::savePath(m_FileSelectedID);
	remove(path.c_str());
}


void CStageSave::createSelSaveFile() {
	std::string saveFileName = getButton(m_IDInputButton).getText();

	std::ofstream saveFile(CMap::savePath(m_FileSelectedID).c_str());
	if (saveFile.fail()) {
		throw CorruptedFileException("File couldn't be opened");
	}
	saveFile << saveFileName << std::endl << std::endl << m_Map->toString();
	saveFile.close();
}

void CStageSave::pressedFile(unsigned int choice) {
	m_FileSelected = true;
	m_FileSelectedID = choice;

	updateDeleteButton();
	updateSaveButton();
}

void CStageSave::updateSaveButton() {
	getButton(m_IDSaveButton).setActive(m_FileSelected && correctFileName());
}

void CStageSave::updateDeleteButton() {
	getButton(m_IDDeleteButton).setActive(m_FileSelected && m_SaveFileButtons[m_FileSelectedID].isActive());
}

void CStageSave::createDirectory() {
#ifdef __linux__
	mkdir(REL_SAVE_FILE_PATH.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#elif _WIN32
	_mkdir(REL_SAVE_FILE_PATH.c_str());
#endif
}
