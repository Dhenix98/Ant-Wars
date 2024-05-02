#include "CClash.h"

bool cmpSharedPtrClash(const std::shared_ptr<CClash> &lhs, const std::shared_ptr<CClash> &rhs) {
	return *lhs.get() < *rhs.get();
}

CClash::CClash(HivePtr firstHive, HivePtr secondHive) :
	m_AttackDelay(SDL_GetTicks()),
	m_FirstHive(firstHive),
	m_SecondHive(secondHive),
	m_FirstHiveLine(CLine(firstHive->getID(), firstHive->getCentre(), secondHive->getCentre(), firstHive->getColor())),
	m_SecondHiveLine(CLine(secondHive->getID(), secondHive->getCentre(), firstHive->getCentre(), secondHive->getColor()))
{}

CClash::CClash(HivePtr firstHive, HivePtr secondHive, int state,
	int currentXFirst, int currentYFirst, int currentXSecond, int currentYSecond) :
	m_AttackDelay(SDL_GetTicks()),
	m_FirstHive(firstHive),
	m_SecondHive(secondHive),
	m_FirstHiveLine(CLine(firstHive->getID(), firstHive->getCentre(), secondHive->getCentre(), firstHive->getColor())),
	m_SecondHiveLine(CLine(secondHive->getID(), secondHive->getCentre(), firstHive->getCentre(), secondHive->getColor()))
{
	setLineStatesByValue(state);
	// move the line to saved location
	setTargetsByCurrCoords(currentXFirst, currentYFirst, currentXSecond, currentYSecond);
	m_FirstHiveLine.instantMoveToTarget();
	m_SecondHiveLine.instantMoveToTarget();


	setTargetsByCurrClashState();
}

void CClash::contAction() {
	// movement
	m_FirstHiveLine.gradualMovement();
	m_SecondHiveLine.gradualMovement();

	// there is clash
	if (m_FirstHiveLine.m_XCurrent == m_SecondHiveLine.m_XCurrent && m_FirstHiveLine.m_YCurrent == m_SecondHiveLine.m_YCurrent) {
		//timer
		if (m_AttackRate < (SDL_GetTicks() - m_AttackDelay)) {
			m_AttackDelay = SDL_GetTicks();
			// attack or support
			if (m_FirstHive->getColor() != m_SecondHive->getColor()) {
				hiveAttacks();
			}
			else {
				hiveSupport();
			}
		}
	}
}

void CClash::render() const {
	m_FirstHiveLine.render();
	m_SecondHiveLine.render();
}

bool CClash::playerClickedLine(const SDL_Point &mousePosition, EColor playerColor) {
	if (m_FirstHiveLine.m_Color == playerColor && m_FirstHiveLine.mouseInHitbox(mousePosition)) {
		retreatArmy(m_FirstHive);
		return true;
	}
	if (m_SecondHiveLine.m_Color == playerColor && m_SecondHiveLine.mouseInHitbox(mousePosition)) {
		retreatArmy(m_SecondHive);
		return true;
	}
	return false;
}


void CClash::playerMovedOverLine(const SDL_Point &mousePosition, EColor playerColor) {

	if (m_FirstHiveLine.m_Color == playerColor && m_FirstHiveLine.mouseInHitbox(mousePosition)) {
		m_FirstHiveLine.setHighlight(true);
	}
	else {
		m_FirstHiveLine.setHighlight(false);
	}

	if (m_SecondHiveLine.m_Color == playerColor && m_SecondHiveLine.mouseInHitbox(mousePosition)) {
		m_SecondHiveLine.setHighlight(true);
	}
	else {
		m_SecondHiveLine.setHighlight(false);
	}
}

void CClash::setHighlight(bool highlight, EColor playerColor) {
	if (m_FirstHiveLine.m_Color == playerColor) {
		m_FirstHiveLine.setHighlight(highlight);
	}
	if (m_SecondHiveLine.m_Color == playerColor) {
		m_SecondHiveLine.setHighlight(highlight);
	}
}

void CClash::sendArmy(const HivePtr &hive) {
	// so only one side sends army
	if (m_FirstHive->getColor() == m_SecondHive->getColor()) {
		m_FirstHiveLineActive = m_SecondHiveLineActive = false;
	}
	size_t hiveNum = getHiveNum(hive);
	setHiveLineActive(hiveNum, true);
	setTargetsByCurrClashState();
}

void CClash::retreatArmy(const HivePtr &hive) {
	size_t hiveNum = getHiveNum(hive);
	setHiveLineActive(hiveNum, false);
	setTargetsByCurrClashState();
}

void CClash::retreatOtherArmy(const HivePtr &hive) {
	size_t hiveNum = invertHiveNum(getHiveNum(hive));
	setHiveLineActive(hiveNum, false);
	setTargetsByCurrClashState();
}

void CClash::instantRetreatArmy(const HivePtr &hive) {
	size_t hiveNum = getHiveNum(hive);
	setHiveLineActive(hiveNum, false);
	setTargetsByCurrClashState();
	getHiveLine(hiveNum).instantMoveToTarget();
}

void CClash::hiveAttacks() {

	unsigned int firstHiveLosses, secondHiveLosses;
	if (m_FirstHiveLineActive && !m_SecondHiveLineActive) {
		firstHiveLosses = m_NormalAttackRate;
		secondHiveLosses = m_AdvantageAttackRate;
	}
	else if (!m_FirstHiveLineActive && m_SecondHiveLineActive) {
		firstHiveLosses = m_AdvantageAttackRate;
		secondHiveLosses = m_NormalAttackRate;
	}
	else {
		firstHiveLosses = m_NormalAttackRate;
		secondHiveLosses = m_NormalAttackRate;
	}
	if (m_FirstHive->loseAnts(firstHiveLosses)) {
		if (m_SecondHiveLineActive) {
			m_FirstHive->setNewColor(m_SecondHive->getColor());
			m_FirstHive->setUpdateOwner(true);
		}
	}
	if (m_SecondHive->loseAnts(secondHiveLosses)) {
		if (m_FirstHiveLineActive) {
			m_SecondHive->setNewColor(m_FirstHive->getColor());
			m_SecondHive->setUpdateOwner(true);
		}
	}
}

void CClash::hiveSupport() {
	if (m_FirstHiveLineActive) {
		m_FirstHive->loseAnts(m_AdvantageAttackRate);
		m_SecondHive->gainAnts(m_AdvantageAttackRate);

	}
	else if (m_SecondHiveLineActive) {
		m_FirstHive->gainAnts(m_AdvantageAttackRate);
		m_SecondHive->loseAnts(m_AdvantageAttackRate);
	}
}

void CClash::setTargetsByCurrClashState() {
	// both are active
	if (m_FirstHiveLineActive && m_SecondHiveLineActive) {
		m_FirstHiveLine.setTargetToMiddle();
		m_SecondHiveLine.setTargetToMiddle();
	}
	else {
		m_FirstHiveLineActive ? m_FirstHiveLine.setTargetToEnd() : m_FirstHiveLine.setTargetToBegin();
		m_SecondHiveLineActive ? m_SecondHiveLine.setTargetToEnd() : m_SecondHiveLine.setTargetToBegin();
	}
}

void CClash::setTargetsByCurrCoords(int currentXFirst, int currentYFirst, int currentXSecond, int currentYSecond) {
	m_FirstHiveLine.setTarget(currentXFirst, currentYFirst);
	m_SecondHiveLine.setTarget(currentXSecond, currentYSecond);
}

void CClash::updateLineColor(const HivePtr &hive) {
	if (hive->getID() == m_FirstHive->getID()) {
		m_FirstHiveLine.setColor(hive->getColor());
	}
	else if (hive->getID() == m_SecondHive->getID()) {
		m_SecondHiveLine.setColor(hive->getColor());
	}
}

bool CClash::shouldBeDeleted() const {
	return m_FirstHiveLine.toBeDeleted() && m_SecondHiveLine.toBeDeleted();
}

bool CClash::hiveParticipates(const HivePtr &hive) const {
	return m_FirstHive->getID() == hive->getID() || m_SecondHive->getID() == hive->getID();
}

bool CClash::isHiveActive(const HivePtr &hive) const {
	size_t hiveNum = getHiveNum(hive);
	return getHiveLineActive(hiveNum);
}

bool CClash::bothHiveHaveSameColor() const {
	return m_FirstHive->getColor() == m_SecondHive->getColor();
}

std::string CClash::toString() const {
	return std::to_string(m_FirstHive->getID()) +
		" " + std::to_string(m_SecondHive->getID()) +
		" " + std::to_string(m_FirstHiveLineActive + m_SecondHiveLineActive * 2) +
		" " + m_FirstHiveLine.toString() +
		" " + m_SecondHiveLine.toString() + "\n";
}

void CClash::setLineStatesByValue(int state) {
	m_FirstHiveLineActive = state % 2 == 1 ? true : false;
	m_SecondHiveLineActive = state / 2 == 1 ? true : false;
}

bool CClash::operator < (const CClash &oth) {
	if (m_FirstHive->getID() == oth.m_FirstHive->getID()) {
		return m_SecondHive->getID() < oth.m_SecondHive->getID();
	}
	return m_FirstHive->getID() < oth.m_FirstHive->getID();
}

bool CClash::onlyFirstIsActive() const {
	return m_FirstHiveLineActive && !m_SecondHiveLineActive;
}

bool CClash::onlySecondIsActive() const {
	return !m_FirstHiveLineActive && m_SecondHiveLineActive;
}

bool CClash::noneIsActive() const {
	return !m_FirstHiveLineActive && !m_SecondHiveLineActive;
}

bool CClash::bothAreActive() const {
	return m_FirstHiveLineActive && m_SecondHiveLineActive;
}

//------------------- PRIVATE ------------------------//

size_t CClash::getHiveNum(const HivePtr &hive) const {
	if (hive->getID() == m_FirstHive->getID()) {
		return 1;
	}
	else if (hive->getID() == m_SecondHive->getID()) {
		return 2;
	}
	return 0;
}

size_t CClash::invertHiveNum(size_t hiveNum) {
	if (hiveNum == 1) {
		return 2;
	}
	if (hiveNum == 2) {
		return 1;
	}
	throw std::invalid_argument("Only 1 and 2 are allowed!");
}

HivePtr &CClash::getHive(size_t hiveNum) {
	if (hiveNum == 1) {
		return m_FirstHive;
	}
	if (hiveNum == 2) {
		return m_SecondHive;
	}
	throw std::invalid_argument("Only 1 and 2 are allowed!");
}

CLine &CClash::getHiveLine(size_t hiveNum) {
	if (hiveNum == 1) {
		return m_FirstHiveLine;
	}
	if (hiveNum == 2) {
		return m_SecondHiveLine;
	}
	throw std::invalid_argument("Only 1 and 2 are allowed!");
}

bool CClash::getHiveLineActive(size_t hiveNum) const {
	if (hiveNum == 1) {
		return m_FirstHiveLineActive;
	}
	if (hiveNum == 2) {
		return m_SecondHiveLineActive;
	}
	throw std::invalid_argument("Only 1 and 2 are allowed!");
}

void CClash::setHiveLineActive(size_t hiveNum, bool choice) {
	if (hiveNum == 1) {
		m_FirstHiveLineActive = choice;
	}
	else if (hiveNum == 2) {
		m_SecondHiveLineActive = choice;
	}
	else {
		throw std::invalid_argument("Only 1 and 2 are allowed!");
	}
}
