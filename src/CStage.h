#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include <map>
#include "CClash.h"
#include "CHive.h"
#include "CButton.h"
#include "CDatabase.h"
#include "CCustomEvents.h"
#include "CMap.h"
#include "EStage.h"
#include "Exceptions.h"

/// @brief common stage for all
class CStage {
public:
	CStage(MapPtr map);
	virtual ~CStage();
	/// @brief handle players input
	virtual void handlePlayerInput(SDL_Event &eventHandler) = 0;
	/// @brief continous events => object movement, regenerations belong here
	virtual void doEvents();
	/// @brief render stage
	virtual void render() const = 0;
	/// @brief update stage
	virtual void update();
	/// @return button with this id
	CButton &getButton(int id);

	static SDL_Color getSDLColorByDifficulty(EDifficulty difficulty);
	static EColor getColorByDifficulty(EDifficulty difficulty);
	/// @brief used for returning to prevStage (mainly used in ESC)
	static EStage prevStage;

protected:
	/// @brief verify if save files exists, and if they do, than they are also named
	void verifySaveFiles();
	void createSaveFileButtons(const DatabasePtr &database);
	/// @brief find this save file button name
	/// @return true if the save file exists
	static bool findAndInsertSaveFileName(unsigned int num, std::string &fileName);
	/// @brief used for calculating offsets between save file buttons
	/// @return offset
	static int getSaveFileCoordY(unsigned int i);
	/// @brief resets map
	void resetMap();

	MapPtr m_Map;

	CTexture const *m_BackgroundScreen = nullptr;

	std::map<int, CButton> m_Buttons;
	TTF_Font *m_ButtonsFont;

	const unsigned int m_NumOfSaves = 7;
	std::vector<CButton> m_SaveFileButtons;
	TTF_Font *m_SaveFileButtonsFont;
	bool m_FileSelected = false;
	unsigned int m_FileSelectedID = 0;
	CTexture const *m_SelectedButtonTexture = nullptr;
};
