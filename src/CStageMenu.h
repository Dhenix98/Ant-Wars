#pragma once
#include "CStage.h"

/// @brief handles all aspects of menu stage (except some common methods form CStage)
class CStageMenu : public CStage {
public:
	CStageMenu(MapPtr map, const DatabasePtr &database);
	/// @brief handle change of stages and possible reset
	void handlePlayerInput(SDL_Event &eventHandler) override;
	void render() const override;
	/// @brief update button availability
	void update() override;

private:
	static int m_AvailableButtonID;
	const int m_IDResetButton = m_AvailableButtonID++;
	const int m_IDContinueButton = m_AvailableButtonID++;
	const int m_IDNewGameButton = m_AvailableButtonID++;
	const int m_IDSaveButton = m_AvailableButtonID++;
	const int m_IDLoadButton = m_AvailableButtonID++;
	const int m_IDExitButton = m_AvailableButtonID++;
};
