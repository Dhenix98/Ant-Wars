#include "CStageGame.h"

CStageGame::CStageGame(MapPtr map, const DatabasePtr &database) :
	CStage(map),
	m_FirstEnemy(map),
	m_SecondEnemy(map)
{
	{
		using namespace DATABASE_ID::TEXTURE_ID::GAME;
		m_BackgroundScreen = database->getTexture(SCREEN);
	}
	{
		using namespace DATABASE_ID::SOUND_ID;
		m_InvalidMoveSound = database->getSound(INVALID_MOVE);
	}
}

void CStageGame::handlePlayerInput(SDL_Event &eventHandler) {
	if (eventHandler.type == SDL_KEYUP && eventHandler.key.repeat == 0) {
		if (eventHandler.key.keysym.sym == SDLK_ESCAPE) {
			CCustomEvents::sendNextStage(EStage::MENU);
		}
		// for debug purposes
		if (eventHandler.key.keysym.sym == SDLK_w) {
			CCustomEvents::sendNextStage(EStage::VICTORY);
			m_Map->incrCurrLevel();
		}
		// for debug purposes
		if (eventHandler.key.keysym.sym == SDLK_e) {
			CCustomEvents::sendNextStage(EStage::DEFEAT);
		}
	}
	// highlight
	else if (eventHandler.type == SDL_MOUSEMOTION) {
		SDL_Point mousePosition = { eventHandler.button.x, eventHandler.button.y };
		highlight(mousePosition);
	}
	// click
	else if (eventHandler.type == SDL_MOUSEBUTTONUP) {

		SDL_Point mousePosition = { eventHandler.button.x, eventHandler.button.y };
		// right click
		if (eventHandler.button.button == SDL_BUTTON_RIGHT) {
			unselectHive();
			return;
		}
		// left click
		else if (eventHandler.button.button == SDL_BUTTON_LEFT) {
			if (clickHive(mousePosition)) return;
			if (clickHiveClash(mousePosition)) return;
		}
	}
}

void CStageGame::doEvents() {
	moveAI();
	actionClashes();
	deleteClashes();
	actionHives();
	checkForWin();
}


void CStageGame::update() {
	m_FirstEnemy.setDifficulty(m_Map->m_Difficulty);
	m_SecondEnemy.setDifficulty(m_Map->m_Difficulty);
	m_FirstEnemy.setColor(m_Map->m_FirstEnemyColor);
	m_SecondEnemy.setColor(m_Map->m_SecondEnemyColor);
	m_FirstEnemy.resetTimer();
	m_SecondEnemy.resetTimer();
}

void CStageGame::render() const {
	m_BackgroundScreen->render(0, 0);

	for (const auto &x : m_Map->m_Clashes) {
		x->render();
	}

	for (const auto &x : m_Map->m_Hives) {
		x.second->render();
	}

	for (const auto &x : m_Map->m_Walls) {
		x.render();
	}
}

void CStageGame::checkForWin() {
	if (m_Map->playerOwnsAllHives()) {
		CCustomEvents::sendNextStage(EStage::VICTORY);
		m_Map->incrCurrLevel();
	}
	else if (m_Map->playerDoesntOwnAnyHives()) {
		CCustomEvents::sendNextStage(EStage::DEFEAT);
	}
}

void CStageGame::moveAI() {
	m_FirstEnemy.doMove();
	m_SecondEnemy.doMove();
}

void CStageGame::actionClashes() {
	for (auto &x : m_Map->m_Clashes) {
		x->contAction();
	}
}

void CStageGame::deleteClashes() {
	for (auto it = m_Map->m_Clashes.begin(); it != m_Map->m_Clashes.end();)
	{
		if ((*it)->shouldBeDeleted()) {
			it = m_Map->m_Clashes.erase(it);
		}
		else {
			++it;
		}
	}
}

void CStageGame::actionHives() {
	for (auto &x : m_Map->m_Hives) {
		x.second->contAction();
	}
	for (auto &x : m_Map->m_Hives) {
		if (x.second->getUpdateOwner()) {
			if (m_SelectedHive!=nullptr && x.second->getID() == m_SelectedHive->getID()) {
				unselectHive();
			}
			x.second->applyNewColor();
			m_Map->removeClashes(x.second);
			x.second->updateTexture(m_Map->m_Database);
		}
	}
}

void CStageGame::playerSendsArmy(const HivePtr &target) {
	if (m_Map->existObstacle(m_SelectedHive, target)) {
		Mix_PlayChannel(-1, m_InvalidMoveSound, 0);
	}
	else if (m_SelectedHive->getColor() == m_Map->getPlayerColor()) {
		m_Map->createClash(m_SelectedHive, target);
	}
}

void CStageGame::highlight(const SDL_Point &mousePosition) {
	bool foundHive = false;
	//highlight hive
	for (auto &x : m_Map->m_Hives) {
		if (x.second->mouseInHitbox(mousePosition)) {
			x.second->setHighlight(true);
			foundHive = true;
		}
		else if (!(x.second->isSelected())) {
			x.second->setHighlight(false);
		}
	}
	// highlight clashes
	if (!foundHive) {
		for (auto &x : m_Map->m_Clashes) {
			x->playerMovedOverLine(mousePosition, m_Map->getPlayerColor());
		}
	}
}


void CStageGame::unselectHive() {
	if (m_SelectedHive != nullptr) {
		m_SelectedHive->setHighlight(false);
		m_SelectedHive->setSelected(false);
		m_SelectedHive = nullptr;
	}
}

bool CStageGame::clickHive(const SDL_Point &mousePosition) {
	for (auto &x : m_Map->m_Hives) {
		if (x.second->mouseInHitbox(mousePosition)) {
			if (m_SelectedHive == nullptr && x.second->getColor() == m_Map->m_PlayerColor) {
				m_SelectedHive = x.second;
				m_SelectedHive->setSelected(true);
				return true;
			}
			else if (m_SelectedHive != nullptr && !isSameAsSelected(x.second)) {
				playerSendsArmy(x.second);
				unselectHive();
				return true;
			}
		}
	}
	return false;
}

bool CStageGame::clickHiveClash(const SDL_Point &mousePosition) {
	for (auto &x : m_Map->m_Clashes) {
		if (x->playerClickedLine(mousePosition, m_Map->getPlayerColor())) {
			return true;
		}
	}
	return false;
}


bool CStageGame::isSameAsSelected(const HivePtr &otherHive) const {
	return m_SelectedHive->getID() == otherHive->getID();
}
