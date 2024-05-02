#include "CStageNewGame.h"

int CStageNewGame::m_AvailableButtonID = 0;


CStageNewGame::CStageNewGame(MapPtr map, const DatabasePtr &database) :
	CStage(map)
{
	// default values 
	const EColor defaultColor = EColor::BLUE_HIVE;
	const EDifficulty defaultDifficulty = EDifficulty::MEDIUM;

	// shared textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU;

		m_Buttons.emplace(m_IDContinueButton, CButton(463, 560, 354, 78, database->getTexture(BTN_AVAILABLE), nullptr,
			m_ButtonsFont, "CONTINUE"));
	}

	// only this stage textures
	{
		using namespace DATABASE_ID::TEXTURE_ID::MENU::NEW_GAME;
		using namespace DATABASE_ID::FONT_ID;

		m_Buttons.emplace(m_IDBlueButton, CButton(450, 120, 100, 100, database->getTexture(BLUE_BTN)));
		m_Buttons.emplace(m_IDGreenButton, CButton(590, 120, 100, 100, database->getTexture(GREEN_BTN)));
		m_Buttons.emplace(m_IDRedButton, CButton(730, 120, 100, 100, database->getTexture(RED_BTN)));
		m_Buttons.emplace(m_IDSelectedColorButton, CButton(0, 0, 100, 100, database->getTexture(SEL_BTN)));

		m_Buttons.emplace(m_IDEasyButton, CButton(450, 360, 100, 100, database->getTexture(DIFF_BTN), nullptr,
			database->getFont(DIFFICULTY_LETTER), "E", EColor::GREEN_DIFFICULTY));

		m_Buttons.emplace(m_IDMediumButton, CButton(590, 360, 100, 100, database->getTexture(DIFF_BTN), nullptr,
			database->getFont(DIFFICULTY_LETTER), "M", EColor::ORANGE_DIFFICULTY));

		m_Buttons.emplace(m_IDHardButton, CButton(730, 360, 100, 100, database->getTexture(DIFF_BTN), nullptr,
			database->getFont(DIFFICULTY_LETTER), "H", EColor::RED_DIFFICULTY));

		m_Buttons.emplace(m_IDSelectedDiffButton, CButton(0, 0, 100, 100, database->getTexture(SEL_BTN)));

		m_BackgroundScreen = database->getTexture(SCREEN);
	}

	// move selected buttons to the right default value
	updateSelectedColor(defaultColor);
	updateSelectedDiff(defaultDifficulty);
}



void CStageNewGame::handlePlayerInput(SDL_Event &eventHandler) {
	SDL_Point mousePosition;
	mousePosition.x = eventHandler.button.x;
	mousePosition.y = eventHandler.button.y;

	// pressing ESC
	if (eventHandler.type == SDL_KEYUP && eventHandler.key.repeat == 0) {
		if (eventHandler.key.keysym.sym == SDLK_ESCAPE) {
			CCustomEvents::sendNextStage(EStage::MENU);
		}
	}

	// left click 
	if (eventHandler.type == SDL_MOUSEBUTTONUP && eventHandler.button.button == SDL_BUTTON_LEFT) {

		if (getButton(m_IDContinueButton).mouseInActiveHitbox(mousePosition)) {
			if (m_Map->newGameMap(m_SelectedColor, m_SelectedDifficulty)) {
				CCustomEvents::sendNextStage(EStage::GAME);

			}
			else {
				CCustomEvents::sendNextStage(EStage::MENU);
				m_Map->m_GameRunning = false;
			}
		}

		else if (getButton(m_IDBlueButton).mouseInHitbox(mousePosition)) {
			updateSelectedColor(EColor::BLUE_HIVE);
		}

		else if (getButton(m_IDGreenButton).mouseInHitbox(mousePosition)) {
			updateSelectedColor(EColor::GREEN_HIVE);
		}

		else if (getButton(m_IDRedButton).mouseInHitbox(mousePosition)) {
			updateSelectedColor(EColor::RED_HIVE);
		}
		else if (getButton(m_IDEasyButton).mouseInHitbox(mousePosition)) {
			updateSelectedDiff(EDifficulty::EASY);
		}

		else if (getButton(m_IDMediumButton).mouseInHitbox(mousePosition)) {
			updateSelectedDiff(EDifficulty::MEDIUM);
		}

		else if (getButton(m_IDHardButton).mouseInHitbox(mousePosition)) {
			updateSelectedDiff(EDifficulty::HARD);
		}
	}
}

void CStageNewGame::render() const {
	m_BackgroundScreen->render(0, 0);
	for (auto &x : m_Buttons) {
		x.second.render();
	}
}

void CStageNewGame::updateSelectedColor(EColor newColor) {
	m_SelectedColor = newColor;

	if (newColor == EColor::BLUE_HIVE) {
		getButton(m_IDSelectedColorButton).move(getButton(m_IDBlueButton).getX(), getButton(m_IDBlueButton).getY());
		return;
	}

	if (newColor == EColor::GREEN_HIVE) {
		getButton(m_IDSelectedColorButton).move(getButton(m_IDGreenButton).getX(), getButton(m_IDGreenButton).getY());
		return;
	}

	if (newColor == EColor::RED_HIVE) {
		getButton(m_IDSelectedColorButton).move(getButton(m_IDRedButton).getX(), getButton(m_IDRedButton).getY());
		return;
	}

	throw NotImplementedException("This color has not been implemented in \"updateSelectedColor(EColor newColor)\"");
}

void CStageNewGame::updateSelectedDiff(EDifficulty newDifficulty) {
	m_SelectedDifficulty = newDifficulty;

	if (newDifficulty == EDifficulty::EASY) {
		getButton(m_IDSelectedDiffButton).move(getButton(m_IDEasyButton).getX(), getButton(m_IDEasyButton).getY());
		return;
	}

	if (newDifficulty == EDifficulty::MEDIUM) {
		getButton(m_IDSelectedDiffButton).move(getButton(m_IDMediumButton).getX(), getButton(m_IDMediumButton).getY());
		return;
	}

	if (newDifficulty == EDifficulty::HARD) {
		getButton(m_IDSelectedDiffButton).move(getButton(m_IDHardButton).getX(), getButton(m_IDHardButton).getY());
		return;
	}
	throw NotImplementedException("This difficulty has not been implemented in \"updateSelectedDiff(EDifficulty newDifficulty)\"");
}
