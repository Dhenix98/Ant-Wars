#pragma once
#include "CStage.h"
#include "EColor.h"

/// @brief handles all aspects of new game stage (except some common methods form CStage)
class CStageNewGame : public CStage {
public:
	CStageNewGame(MapPtr map, const DatabasePtr &database);
	/// @brief handle selection of color and difficulty
	void handlePlayerInput(SDL_Event &eventHandler) override;
	void render() const override;

private:
	/// @brief update button availability updates selected color and moves selected texture to right color
	void updateSelectedColor(EColor newColor);
	/// @brief update button availability updates selected difficulty and moves selected texture to right difficulty
	void updateSelectedDiff(EDifficulty newDifficulty);

	static int m_AvailableButtonID;
	const int m_IDContinueButton = m_AvailableButtonID++;
	const int m_IDBlueButton = m_AvailableButtonID++;
	const int m_IDGreenButton = m_AvailableButtonID++;
	const int m_IDRedButton = m_AvailableButtonID++;
	const int m_IDSelectedColorButton = m_AvailableButtonID++;
	const int m_IDEasyButton = m_AvailableButtonID++;
	const int m_IDMediumButton = m_AvailableButtonID++;
	const int m_IDHardButton = m_AvailableButtonID++;
	const int m_IDSelectedDiffButton = m_AvailableButtonID++;

	EColor m_SelectedColor;
	EDifficulty m_SelectedDifficulty;
};

