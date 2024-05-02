#pragma once
#include "CStage.h"
#include "CAI.h"

/// @brief handles all aspects of game stage (except some common methods form CStage)
class CStageGame : public CStage {
public:
	CStageGame(MapPtr map, const DatabasePtr &database);
	/// @brief handle attacks/retreats/supports
	void handlePlayerInput(SDL_Event &eventHandler) override;
	/// @brief object actions
	void doEvents() override;
	/// @brief render all objects
	void render() const override;
	/// @brief set up AI
	void update() override;

private:
	/// @brief player sends army from selectedHive to target hive
	void playerSendsArmy(const HivePtr &target);
	/// @brief check for win
	void checkForWin();
	/// @brief used for ai movement/actions
	void moveAI();
	/// @brief continous action for all clashes
	void actionClashes();
	/// @brief delete all useless clashes (=nobody attacks/support)
	void deleteClashes();
	/// @brief continous action for all hives
	void actionHives();
	/// @brief calculates highlight acording to mousePosition and locations of clashes and hives
	void highlight(const SDL_Point &mousePosition);
	/// @brief unhighlight selected hive
	void unselectHive();
	/// @brief action for clicking hive (select/attack/support)
	/// @return true if some hive has been clicked
	bool clickHive(const SDL_Point &mousePosition);
	/// @brief action for clicking clash (in this case only retreat)
	/// @return true if some clash has been clicked
	bool clickHiveClash(const SDL_Point &mousePosition);
	/// @return true if selected and this hive are same
	bool isSameAsSelected(const HivePtr &otherHive) const;

	/// @brief so one click only does one action
	bool m_Skip = false;
	/// @brief sound for invalid move (there is collision)
	Mix_Chunk *m_InvalidMoveSound = nullptr;
	/// @brief AIs
	CAI m_FirstEnemy, m_SecondEnemy;
	HivePtr m_SelectedHive;
};
