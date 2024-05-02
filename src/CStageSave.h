#pragma once

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#elif _WIN32
#include <direct.h>
#endif

#include <cctype>
#include "CStage.h"

/// @brief handles all aspects of save stage (except some common methods form CStage)
class CStageSave : public CStage {
public:
	CStageSave(MapPtr map, const DatabasePtr &database);
	/// @brief handle saving and deletion of save files
	void handlePlayerInput(SDL_Event &eventHandler) override;
	void render() const override;
	/// @brief reset stage to init value
	void update() override;
	/// @brief if text writing is active then blinks available line
	void doEvents() override;

private:
	/// @return true if fileName is valid text
	bool correctFileName();
	/// @return true if text contains only whitespace
	static bool containsOnlyWhitespace(std::string text);
	/// @brief remove awaiting blinking line
	/// @return true if line existed and was removed
	bool removeAwaitingLine();
	/// @brief save file (saves map status into file with selected id)
	void saveFile();
	/// @brief add awaiting line at the end (used when writing or deleting fileName)
	void updateInputFileName();
	/// @brief remove selected file
	void removeSelSaveFile();
	/// @brief create on selected place save file
	void createSelSaveFile();
	/// @brief check for update when save file button is pressed
	void pressedFile(unsigned int choice);
	/// @brief update save button when clicking other buttons
	void updateSaveButton();
	/// @brief update delete button when clicking other buttons
	void updateDeleteButton();
	/// @brief create directory for save files if necessary
	void createDirectory();

	const unsigned int m_MaxFileName = 10;

	/// @brief true if text can be written
	bool m_EnabledText = false;
	/// @brief true if awaiting line is showing
	bool m_ShowLine = false;
	/// @brief blinking rate of awaiting line
	Uint32 m_ChangeDelay = 800;
	/// @brief last time when was awaiting line added/removed
	Uint32 m_CurrTime;

	const char *defaultInput = "Click and name file";

	static int m_AvailableButtonID;
	const int m_IDInputButton = m_AvailableButtonID++;
	const int m_IDSaveButton = m_AvailableButtonID++;
	const int m_IDDeleteButton = m_AvailableButtonID++;
	const int m_IDExitButton = m_AvailableButtonID++;
};

