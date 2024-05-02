#include "CAI.h"

CAI::CAI(MapPtr map) :
	m_Map(map),
	m_Color(EColor::RED_HIVE),
	m_Time(SDL_GetTicks()),
	m_Difficulty(EDifficulty::MEDIUM)
{}

void CAI::doMove() {
	switch (m_Difficulty) {
		case EDifficulty::EASY: {
			easyAI();
			break;
		}
		case EDifficulty::MEDIUM: {
			mediumAI();
			break;
		}
		case EDifficulty::HARD: {
			hardAI();
			break;
		}
		default: {
			throw NotImplementedException("This difficulty wasn't implemented in \"doMove()\"");
		}
	}
	// reset action
	m_ActionTaken = false;
}

EColor CAI::getColor() const {
	return m_Color;
}

void CAI::setColor(EColor color) {
	m_Color = color;
}

void CAI::setDifficulty(EDifficulty aiDifficulty) {
	m_Difficulty = aiDifficulty;
}

void CAI::resetTimer() {
	m_Time = SDL_GetTicks();
}

bool CAI::timer(Uint32 delay) {
	if ((SDL_GetTicks() - m_Time) > delay) {
		m_Time = SDL_GetTicks();
		return true;
	}
	return false;
}


void CAI::easyAI() {
	if (timer(5000)) {
		retreatSupport();
		defendSelf();
		attackSemiRandom();
	}
}

void CAI::mediumAI() {
	if (timer(4000)) {
		retreatSupport();
		defendSelf();
		reinforceWeakerHives();
		attackWeakerHives();
	}
}

void CAI::hardAI() {
	if (timer(2000)) {
		retreatSupport();
		defendSelf();
		reinforceWeakerHives();
		attackWeakerHives();
	}
}


void CAI::attackSemiRandom() {
	if (m_ActionTaken) {
		return;
	}
	std::vector<HivePtr> aiHives;
	std::vector<HivePtr> enemyHives;
	divideAllHives(aiHives, enemyHives);
	createRandomClash(aiHives, enemyHives);
}


void CAI::defendSelf() {
	if (m_ActionTaken) {
		return;
	}

	std::vector<std::pair<unsigned int, unsigned int>> possibleDefense;

	// find hives which are under attack and are not defending themselves
	for (const auto &x : m_Map->m_Clashes) {
		if (x->m_FirstHive->getColor() == x->m_SecondHive->getColor()) {
			continue;
		}
		else if (x->m_FirstHive->getColor() == m_Color && x->onlySecondIsActive()) {
			possibleDefense.push_back(std::pair<unsigned int, unsigned int>(x->m_FirstHive->getID(), x->m_SecondHive->getID()));
		}
		else if (x->m_SecondHive->getColor() == m_Color && x->onlyFirstIsActive()) {
			possibleDefense.push_back(std::pair<unsigned int, unsigned int>(x->m_SecondHive->getID(), x->m_FirstHive->getID()));
		}
	}

	if (possibleDefense.empty()) {
		return;
	}

	// select random hive which will protect itself
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	int choosenHive = std::rand() % possibleDefense.size();
	m_Map->createClash(m_Map->getHiveById(possibleDefense[choosenHive].first), m_Map->getHiveById(possibleDefense[choosenHive].second));

	m_ActionTaken = true;
}

void CAI::reinforceWeakerHives() {
	if (m_ActionTaken) {
		return;
	}
	std::vector<HivePtr> aiHives;
	std::vector<HivePtr> enemyHives;
	divideAllHives(aiHives, enemyHives);

	std::vector<HivePtr> receivers;
	std::vector<HivePtr> senders;

	//get senders and receivers
	for (const auto &x : aiHives) {
		if (x->isCapacityRatioGreaterThan(0.5)) {
			senders.push_back(x);
		}
		else if (x->isCapacityRatioLessThan(0.25)) {
			receivers.push_back(x);
		}
	}

	createRandomClash(senders, receivers);
}

void CAI::attackWeakerHives() {
	if (m_ActionTaken) {
		return;
	}
	std::vector<HivePtr> aiHives;
	std::vector<HivePtr> enemyHives;
	divideAllHives(aiHives, enemyHives);

	trimHivesByRatio(aiHives, 0.3);
	orderBySize(aiHives, true);
	orderBySize(enemyHives, false);

	strongestAttackWeakest(aiHives, enemyHives);
}

void CAI::divideAllHives(std::vector<HivePtr> &aiHives, std::vector<HivePtr> &enemyHives) const {
	for (auto &x : m_Map->m_Hives) {
		if (x.second->getColor() == m_Color) {
			aiHives.push_back(x.second);
		}
		else {
			enemyHives.push_back(x.second);
		}
	}
}


unsigned int CAI::randomNumberInRange(unsigned int mod) {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	return std::rand() % mod;
}

void CAI::retreatSupport() {
	if (m_ActionTaken) {
		return;
	}

	std::vector<HivePtr> aiHives;
	std::vector<HivePtr> enemyHives;
	divideAllHives(aiHives, enemyHives);

	std::vector<hiveRetreatInfo> candidateForSupportRetreat;

	// find candidates
	for (const auto &x : aiHives) {
		if (x->isCapacityRatioGreaterThan(0.5)) {
			getCandidate(candidateForSupportRetreat, x, false);
		}
		else if (x->isCapacityRatioLessThan(0.25)) {
			getCandidate(candidateForSupportRetreat, x, true);
		}
	}

	if (!candidateForSupportRetreat.empty()) {
		unsigned int choosenHiveID = randomNumberInRange(static_cast<unsigned int>(candidateForSupportRetreat.size()));
		hiveRetreatInfo &info = candidateForSupportRetreat[choosenHiveID];
		if (info.m_Sender) {
			candidateForSupportRetreat[choosenHiveID].m_Clash->retreatArmy(candidateForSupportRetreat[choosenHiveID].m_Hive);
		}
		else {
			candidateForSupportRetreat[choosenHiveID].m_Clash->retreatOtherArmy(candidateForSupportRetreat[choosenHiveID].m_Hive);
		}
		m_ActionTaken = true;
	}
}

void CAI::getCandidate(std::vector<hiveRetreatInfo> &candidates, HivePtr candidate, bool sender) const {
	for (const auto &y : m_Map->m_Clashes) {
		if (y->hiveParticipates(candidate) && y->isHiveActive(candidate) == sender && y->bothHiveHaveSameColor()) {
			candidates.push_back(hiveRetreatInfo(candidate, y, sender));
		}
	}
}

CAI::hiveRetreatInfo::hiveRetreatInfo(HivePtr hive, ClashPtr clash, bool sender) :
	m_Hive(hive),
	m_Clash(clash),
	m_Sender(sender)
{}

void CAI::createRandomClash(std::vector<HivePtr> &sources, std::vector<HivePtr> &targets, const unsigned int maxParticipations) {
	while (!sources.empty()) {
		// choose random ai hive
		unsigned int choosenHive = randomNumberInRange(static_cast<unsigned int>(sources.size()));

		// reset other hives
		std::vector<HivePtr> tmpTargets = targets;

		while (!tmpTargets.empty() && m_Map->howManyParticipations(sources[choosenHive]) < maxParticipations) {
			//choose random hive which doesn't belong to this ai
			unsigned int targetHive = randomNumberInRange(static_cast<unsigned int>(tmpTargets.size()));

			if (!m_Map->existObstacle(sources[choosenHive], tmpTargets[targetHive])) {

				m_Map->createClash(sources[choosenHive], tmpTargets[targetHive]);
				m_ActionTaken = true;
				return;
			}
			tmpTargets.erase(tmpTargets.begin() + targetHive);
		}
		sources.erase(sources.begin() + choosenHive);
	}
}

void CAI::trimHivesByRatio(std::vector<HivePtr> &hives, double ratio) {
	for (auto it = hives.begin(); it != hives.end();) {

		if ((*it)->isCapacityRatioLessThan(ratio)) {
			it = hives.erase(it);
		}
		else {
			++it;
		}
	}
}

void CAI::orderBySize(std::vector<HivePtr> &hives, bool descending) {
	if (descending) {
		std::sort(hives.begin(), hives.end(), [](const HivePtr &lhs, const HivePtr &rhs) {
			return rhs->getNumOfAnts() < lhs->getNumOfAnts();
			});
	}
	else {
		std::sort(hives.begin(), hives.end(), [](const HivePtr &lhs, const HivePtr &rhs) {
			return lhs->getNumOfAnts() < rhs->getNumOfAnts();
			});
	}
}

void CAI::strongestAttackWeakest(std::vector<HivePtr> &aiHives, std::vector<HivePtr> &enemyHives, unsigned int maxParticipations) {
	for (auto &y : enemyHives) {
		for (auto &x : aiHives) {
			if (m_Map->howManyParticipations(x) < maxParticipations && !m_Map->existObstacle(x, y)) {
				auto it = m_Map->findClash(x, y);
				if (it == m_Map->m_Clashes.end() || !(*it)->isHiveActive(x)) {

					m_Map->createClash(x, y);
					m_ActionTaken = true;
					return;
				}
			}
		}
	}
}
