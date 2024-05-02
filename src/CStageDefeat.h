#pragma once
#include "CStage.h"

/// @brief handles all aspects of defeat stage (except some common methods form CStage)
class CStageDefeat : public CStage {
public:
	CStageDefeat(MapPtr map, const DatabasePtr &database);
	/// @brief handle reset and changing stages
	void handlePlayerInput(SDL_Event &eventHandler) override;
	void render() const override;
	void update() override;

private:
	static int m_AvailableButtonID;
	const int m_IDResetButton = m_AvailableButtonID++;
	const int m_IDLowerDiffButton = m_AvailableButtonID++;
	const int m_IDLoadButton = m_AvailableButtonID++;
	const int m_IDExitButton = m_AvailableButtonID++;

	CTextureText m_Text;
	CTextureText m_CurrentDifficultyTexture;
};
