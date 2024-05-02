#include "CStageMenu.h"

int CStageMenu::m_AvailableButtonID = 0;

CStageMenu::CStageMenu(MapPtr map, const DatabasePtr &database) :
	CStage(map)
{
	// shared textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU;
		m_Buttons.emplace(m_IDResetButton, CButton(463, 170, 354, 78, database->getTexture(BTN_AVAILABLE), database->getTexture(BTN_NOT_AVAILABLE),
			m_ButtonsFont, "RESET"));

		m_Buttons.emplace(m_IDContinueButton, CButton(463, 260, 354, 78, database->getTexture(BTN_AVAILABLE), database->getTexture(BTN_NOT_AVAILABLE),
			m_ButtonsFont, "CONTINUE"));

		m_Buttons.emplace(m_IDNewGameButton, CButton(463, 350, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "NEW GAME"));

		m_Buttons.emplace(m_IDSaveButton, CButton(463, 440, 354, 78, database->getTexture(BTN_AVAILABLE), database->getTexture(BTN_NOT_AVAILABLE),
			m_ButtonsFont, "SAVE"));

		m_Buttons.emplace(m_IDLoadButton, CButton(463, 530, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "LOAD"));

		m_Buttons.emplace(m_IDExitButton, CButton(463, 620, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "EXIT"));
	}

	// only this stage textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU::MAIN_MENU;
		m_BackgroundScreen = database->getTexture(SCREEN);
	}

	update();
}

void CStageMenu::handlePlayerInput(SDL_Event &eventHandler) {
	// pressed ESC => resume game if the game is running
	if (eventHandler.type == SDL_KEYUP && eventHandler.key.keysym.sym == SDLK_ESCAPE && eventHandler.key.repeat == 0 && m_Map->m_GameRunning) {
		CCustomEvents::sendNextStage(EStage::GAME);
	}
	else if (eventHandler.type == SDL_MOUSEBUTTONUP && eventHandler.button.button == SDL_BUTTON_LEFT) {

		SDL_Point mousePosition;
		mousePosition.x = eventHandler.button.x;
		mousePosition.y = eventHandler.button.y;

		if (getButton(m_IDResetButton).mouseInActiveHitbox(mousePosition)) {
			resetMap();
			CCustomEvents::sendNextStage(EStage::GAME);
		}

		if (getButton(m_IDContinueButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::GAME);
		}

		else if (getButton(m_IDNewGameButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::NEWGAME);
		}

		else if (getButton(m_IDSaveButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::SAVE);
		}

		else if (getButton(m_IDLoadButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::LOAD);
		}

		else if (getButton(m_IDExitButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendQuitApp();
		}
	}
}

void CStageMenu::render() const {
	m_BackgroundScreen->render(0, 0);
	for (const auto &x : m_Buttons) {
		x.second.render();
	}
}

void CStageMenu::update() {
	bool result = m_Map->m_GameRunning;
	getButton(m_IDResetButton).setActive(result);
	getButton(m_IDContinueButton).setActive(result);
	getButton(m_IDSaveButton).setActive(result);
}