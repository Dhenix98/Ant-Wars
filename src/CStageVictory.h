#pragma once
#include "CStage.h"

/// @brief handles all aspects of victory stage (except some common methods form CStage)
class CStageVictory : public CStage {
public:
	CStageVictory(MapPtr map, const DatabasePtr &database);
	/// @brief handle entering next map, saving or exiting
	void handlePlayerInput(SDL_Event &eventHandler) override;
	void render() const override;
	/// @brief update difficulty text, difficulty color and check if another map is available
	void update() override;

private:
	static int m_AvailableButtonID;
	const int m_IDContinueButton = m_AvailableButtonID++;
	const int m_IDSaveButton = m_AvailableButtonID++;
	const int m_IDExitButton = m_AvailableButtonID++;

	CTextureText m_Text;
	CTextureText m_CurrentDifficultyTexture;
};
