#pragma once
#include "CDatabase.h"
#include "CGame.h"
#include "CTests.h"
#include "globalVar.h"
#include "Exceptions.h"

/// @brief handles all SDL libraries, database, window creation and calls CGame
class CApplication {

public:
	/// @brief initializes SDL libraries, database and game
	CApplication();
	/// @brief run application
	void run();
	/// @brief run application tests
	void runTests();

private:
	/// @brief handles all SDL libraries and renderer and window
	class CSDL {
	public:
		/// @brief initialize all SDL libraries and create window and renderer
		CSDL();
		/// @brief uninitialize all SDL libraries and destroy window and renderer
		~CSDL();
	};
	// must be before all other variables with constructors! (so SDL_Quit is called last)
	CSDL m_SDLConf;
	DatabasePtr m_Database;
	std::unique_ptr<CGame> m_Game;
};
