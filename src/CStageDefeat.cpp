#include "CStageDefeat.h"

int CStageDefeat::m_AvailableButtonID = 0;

CStageDefeat::CStageDefeat(MapPtr map, const DatabasePtr &database) :
	CStage(map),

	m_Text("Difficulty:",
		database->getFont(DATABASE_ID::FONT_ID::DIFFICULTY_END_SCREEN),
		{ 463, 100, 354, 200 }, 0, 0, EColor::BLACK),

	m_CurrentDifficultyTexture(
		toCharArray(m_Map->getDifficulty()),
		database->getFont(DATABASE_ID::FONT_ID::DIFFICULTY_END_SCREEN),
		{ 463, 160, 354, 200 }, 0, 0, getColorByDifficulty(m_Map->getDifficulty()))
{
	// shared textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU;

		m_Buttons.emplace(m_IDResetButton, CButton(463, 350, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "RESTART"));

		m_Buttons.emplace(m_IDLowerDiffButton, CButton(463, 440, 354, 78, database->getTexture(BTN_AVAILABLE), database->getTexture(BTN_NOT_AVAILABLE),
			m_ButtonsFont, "LOWER DIFF"));

		m_Buttons.emplace(m_IDLoadButton, CButton(463, 530, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "LOAD"));

		m_Buttons.emplace(m_IDExitButton, CButton(463, 620, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "EXIT"));

	}

	// only this stage textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::DEFEAT;

		m_BackgroundScreen = database->getTexture(SCREEN);
	}
}

void CStageDefeat::render() const {
	m_BackgroundScreen->render(0, 0);
	for (const auto &x : m_Buttons) {
		x.second.render();
	}
	m_Text.render();
	m_CurrentDifficultyTexture.render();
}

void CStageDefeat::handlePlayerInput(SDL_Event &eventHandler) {
	if (eventHandler.type == SDL_MOUSEBUTTONUP && eventHandler.button.button == SDL_BUTTON_LEFT) {

		SDL_Point mousePosition;
		mousePosition.x = eventHandler.button.x;
		mousePosition.y = eventHandler.button.y;


		if (getButton(m_IDResetButton).mouseInHitbox(mousePosition)) {
			if (m_Map->resetMap()) {
				CCustomEvents::sendNextStage(EStage::GAME);
			}
			else {
				CCustomEvents::sendNextStage(EStage::MENU);
				m_Map->m_GameRunning = false;
			}
		}
		if (getButton(m_IDLowerDiffButton).mouseInHitbox(mousePosition)) {
			m_Map->lowerDifficulty();
			update();
		}
		else if (getButton(m_IDLoadButton).mouseInHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::LOAD);
		}
		else if (getButton(m_IDExitButton).mouseInHitbox(mousePosition)) {
			CCustomEvents::sendNextStage(EStage::MENU);
			m_Map->m_GameRunning = false;
		}
	}
}

void CStageDefeat::update() {
	getButton(m_IDLowerDiffButton).setActive(m_Map->getDifficulty() != EDifficulty::EASY);
	m_CurrentDifficultyTexture.changeColor(getColorByDifficulty(m_Map->getDifficulty()));
	m_CurrentDifficultyTexture.changeText(toCharArray(m_Map->getDifficulty()));
}