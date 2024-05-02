#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#include <cassert>
#include <map>
#include "EColor.h"
#include "CDatabase.h"
#include "CStageDefeat.h"
#include "CStageGame.h"
#include "CStageLoad.h"
#include "CStageMenu.h"
#include "CStageNewGame.h"
#include "CStageSave.h"
#include "CStageVictory.h"
#include "EStage.h"
#include "CCustomEvents.h"

/// @brief handles all stages of application
class CGame {
public:
	CGame(DatabasePtr database);
	/// @brief call right CStage doEvents method
	void doEvents();
	/// @brief call right CStage render method
	void render();
	/// @brief handle all SDL_Events
	void handleSDLEvents(SDL_Event &eventHandler, bool &quitApplication);

private:
	friend class CTests;
	/// @brief call right CStage handlePlayerInput method
	void handlePlayerInput(SDL_Event &eventHandler);
	/// @brief handle reply from stages called by static methods from CCustomEvents
	void handleReplyFromStage(SDL_Event &eventHandler);
	/// @brief used for registering custom SDL_Events
	void registerCustomEvents();
	EStage m_CurrStage = EStage::MENU;

	std::shared_ptr<CMap> m_Map;
	std::map<EStage, std::unique_ptr<CStage>> m_Stages;
	DatabasePtr m_Database;
};