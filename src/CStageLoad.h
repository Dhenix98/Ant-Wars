#pragma once
#include "CStage.h"

/// @brief handles all aspects of load stage (except some common methods form CStage)
class CStageLoad : public CStage {
public:
	CStageLoad(MapPtr map, const DatabasePtr &database);
	/// @brief handle loading and deleting of save files
	void handlePlayerInput(SDL_Event &eventHandler) override;
	/// @brief render all buttons
	void render() const override;
	/// @brief set to initial state
	void update() override;

private:
	/// @brief try to remove files by selected id
	void removeSelSaveFile();
	/// @brief load file and updates map with its content
	void loadFile();
	/// @brief use for activation of buttons
	void pressedFile(unsigned int choice);

	static int m_AvailableButtonID;
	const int m_IDLoadButton = m_AvailableButtonID++;
	const int m_IDDeleteButton = m_AvailableButtonID++;
	const int m_IDExitButton = m_AvailableButtonID++;
};
