#include "CGame.h"

CGame::CGame(DatabasePtr database) :
	m_Map(std::make_shared<CMap>(CMap(database)))
{
	// add stages
	m_Stages.insert(std::make_pair(EStage::MENU, std::unique_ptr<CStage>(new CStageMenu(m_Map, database))));
	m_Stages.insert(std::make_pair(EStage::NEWGAME, std::unique_ptr<CStage>(new CStageNewGame(m_Map, database))));
	m_Stages.insert(std::make_pair(EStage::SAVE, std::unique_ptr<CStage>(new CStageSave(m_Map, database))));
	m_Stages.insert(std::make_pair(EStage::LOAD, std::unique_ptr<CStage>(new CStageLoad(m_Map, database))));
	m_Stages.insert(std::make_pair(EStage::GAME, std::unique_ptr<CStage>(new CStageGame(m_Map, database))));
	m_Stages.insert(std::make_pair(EStage::VICTORY, std::unique_ptr<CStage>(new CStageVictory(m_Map, database))));
	m_Stages.insert(std::make_pair(EStage::DEFEAT, std::unique_ptr<CStage>(new CStageDefeat(m_Map, database))));

	m_Database = database;

	registerCustomEvents();
}

void CGame::doEvents() {
	m_Stages[m_CurrStage]->doEvents();
}

void CGame::render() {
	m_Stages[m_CurrStage]->render();
}

void CGame::handleSDLEvents(SDL_Event &eventHandler, bool &quitApplication) {
	if (eventHandler.type == SDL_QUIT) {
		quitApplication = true;
	}
	handlePlayerInput(eventHandler);
	handleReplyFromStage(eventHandler);
}

void CGame::handlePlayerInput(SDL_Event &eventHandler) {
	m_Stages[m_CurrStage]->handlePlayerInput(eventHandler);
}

void CGame::registerCustomEvents() {
	uint32_t tmpEventID = 0;
	CCustomEvents::nextStageEvent = tmpEventID++;
	uint32_t eventID = SDL_RegisterEvents(tmpEventID);
	CCustomEvents::nextStageEvent += eventID;
}

void CGame::handleReplyFromStage(SDL_Event &eventHandler) {
	if (eventHandler.type == CCustomEvents::nextStageEvent) {
		CStage::prevStage = m_CurrStage;
		EStage * tmpStage = static_cast<EStage *>(eventHandler.user.data1);
		m_CurrStage = *tmpStage;
		m_Stages[m_CurrStage]->update();
		delete tmpStage;
	}
}
