#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32

#include <SDL.h>
#endif
#include "EStage.h"
#include "EColor.h"

/// @brief custom SDL_Events for communication between CGame and CStage childs
class CCustomEvents {
public:
	/// @brief push SDL_Event for exiting application
	static void sendQuitApp();
	/// @brief push SDL_Event for entering next stage
	static void sendNextStage(EStage nextStage);

	/// @brief used for right initialization of custom events
	static uint32_t nextStageEvent;
};
