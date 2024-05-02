#include "CStageVictory.h"

int CStageVictory::m_AvailableButtonID = 0;

CStageVictory::CStageVictory(MapPtr map, const DatabasePtr &database) :
	CStage(map),

	m_Text("Difficulty:",
		database->getFont(DATABASE_ID::FONT_ID::DIFFICULTY_END_SCREEN),
		{ 463, 160, 354, 200 }, 0, 0, EColor::BLACK),

	m_CurrentDifficultyTexture(
		toCharArray(m_Map->getDifficulty()),
		database->getFont(DATABASE_ID::FONT_ID::DIFFICULTY_END_SCREEN),
		{ 463, 220, 354, 200 }, 0, 0, getColorByDifficulty(m_Map->getDifficulty()))
{

	// shared textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU;

		m_Buttons.emplace(m_IDContinueButton, CButton(463, 380, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "CONTINUE"));

		m_Buttons.emplace(m_IDSaveButton, CButton(463, 500, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "SAVE"));

		m_Buttons.emplace(m_IDExitButton, CButton(463, 620, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "EXIT"));

	}

	// only this stage textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::VICTORY;

		m_BackgroundScreen = database->getTexture(SCREEN);
	}
}

void CStageVictory::render() const {
	m_BackgroundScreen->render(0, 0);
	for (const auto &x : m_Buttons) {
		x.second.render();
	}
	m_Text.render();
	m_CurrentDifficultyTexture.render();
}

void CStageVictory::handlePlayerInput(SDL_Event &eventHandler) {
	if (eventHandler.type == SDL_MOUSEBUTTONUP && eventHandler.button.button == SDL_BUTTON_LEFT) {

		SDL_Point mousePosition;
		mousePosition.x = eventHandler.button.x;
		mousePosition.y = eventHandler.button.y;


		if (getButton(m_IDContinueButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::GAME);
		}

		else if (getButton(m_IDSaveButton).mouseInActiveHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::SAVE);
		}

		else if (getButton(m_IDExitButton).mouseInActiveHitbox(mousePosition)) {
			m_Map->m_GameRunning = false;
			CCustomEvents::sendNextStage(EStage::MENU);
		}
	}
}

void CStageVictory::update() {
	m_CurrentDifficultyTexture.changeColor(getColorByDifficulty(m_Map->getDifficulty()));
	m_CurrentDifficultyTexture.changeText(toCharArray(m_Map->getDifficulty()));

	bool result = m_Map->loadMap();
	getButton(m_IDContinueButton).setActive(result);
	getButton(m_IDSaveButton).setActive(result);
}