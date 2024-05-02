#include "CTests.h"

CTests::CTests(const std::unique_ptr<CGame> &game, DatabasePtr database) :
	m_Map(game->m_Map),
	m_Database(database)
{}

void CTests::runAll() {
	loadTests();
	hiveTests();
	lineTests();
	textureTextTests();
	textureTests();
}

void CTests::loadTests() {
	// maps loads
	std::string testMapLoc = "examples/tests/maps/";

	// normal map
	assert(loadMap(testMapLoc + "level00.map"));
	// only player hive
	assert(loadMap(testMapLoc + "level01.map"));
	// only neutral hive
	assert(loadMap(testMapLoc + "level02.map"));
	// only enemy hive
	assert(loadMap(testMapLoc + "level03.map"));
	// empty map
	assert(!loadMap(testMapLoc + "level04.map"));
	// no spaces between info
	assert(!loadMap(testMapLoc + "level05.map"));
	// missing hive info => more hives on tile map than hive info
	assert(!loadMap(testMapLoc + "level06.map"));
	// missing tile id => less hives on tile map than hive info
	assert(!loadMap(testMapLoc + "level07.map"));
	// one hive is not 2*2
	assert(!loadMap(testMapLoc + "level08.map"));
	// one hive id is there 2 times
	assert(!loadMap(testMapLoc + "level09.map"));
	// two hive infos with the same id
	assert(!loadMap(testMapLoc + "level10.map"));
	// missing new line at the end
	assert(!loadMap(testMapLoc + "level11.map"));
	// maps doesnt exist
	assert(!loadMap(testMapLoc + "DONT_EXIST"));

	// save loads
	std::string testSaveLoc = "examples/tests/save/";

	// normal save
	assert(loadSaveFile(testSaveLoc + "save00.save"));
	// missing file name
	assert(!loadSaveFile(testSaveLoc + "save01.save"));
	// missing new line file name and tile map
	assert(!loadSaveFile(testSaveLoc + "save02.save"));
	// missing file name and new line file name and tile map
	assert(!loadSaveFile(testSaveLoc + "save03.save"));

	// missing new line between tile map and player info
	assert(!loadSaveFile(testSaveLoc + "save04.save"));
	// missing new line between player info and hive info
	assert(!loadSaveFile(testSaveLoc + "save05.save"));
	// missing new line between hive info and clash info
	assert(!loadSaveFile(testSaveLoc + "save06.save"));
	// missing player info
	assert(!loadSaveFile(testSaveLoc + "save07.save"));
	// missing hive info
	assert(!loadSaveFile(testSaveLoc + "save08.save"));
	// only new lines
	assert(!loadSaveFile(testSaveLoc + "save09.save"));
	// file doesnt exist
	assert(!loadSaveFile(testSaveLoc + "DONT_EXIST"));
}

void CTests::hiveTests() {
	// width and height are 70
	CHive testHive(1, 50, 50, nullptr, nullptr, EColor::BLUE_HIVE, 50, 100, nullptr);

	// test mouseInHitbox
	assert(!testHive.mouseInHitbox({ 2,0 }));
	assert(testHive.mouseInHitbox({ 50,50 }));
	assert(!testHive.mouseInHitbox({ 121,121 }));

	// test collision
	assert(!testHive.collision(2, 0));
	assert(testHive.collision(50, 50));
	assert(!testHive.collision(121, 121));

	// test gain/lose ants
	assert(!testHive.gainAnts(50));
	assert(testHive.gainAnts(1));

	assert(!testHive.loseAnts(100));
	assert(testHive.loseAnts(1));
	assert(testHive.loseAnts(100));
	assert(testHive.loseAnts(1));
	assert(!testHive.gainAnts(50));

	// test ratios
	assert(!testHive.isCapacityRatioLessThan(0.2));
	assert(!testHive.isCapacityRatioLessThan(0.4));
	assert(testHive.isCapacityRatioLessThan(0.6));
	assert(testHive.isCapacityRatioLessThan(0.8));
	assert(testHive.isCapacityRatioLessThan(1));

	assert(testHive.isCapacityRatioGreaterThan(0.2));
	assert(testHive.isCapacityRatioGreaterThan(0.4));
	assert(!testHive.isCapacityRatioGreaterThan(0.6));
	assert(!testHive.isCapacityRatioGreaterThan(0.8));
	assert(!testHive.isCapacityRatioGreaterThan(1));
}

void CTests::lineTests() {
	CLine testLine(1, std::make_pair(50, 50), std::make_pair(100, 100), EColor::BLUE_HIVE);
	assert(testLine.toBeDeleted());
	assert(testLine.hasReachedTarget());

	testLine.setTargetToEnd();
	assert(!testLine.toBeDeleted());
	assert(!testLine.hasReachedTarget());

	testLine.instantMoveToTarget();
	assert(!testLine.toBeDeleted());
	assert(testLine.hasReachedTarget());

	testLine.setTargetToBegin();
	testLine.instantMoveToTarget();
	assert(testLine.toBeDeleted());
	assert(testLine.hasReachedTarget());
}

void CTests::textureTextTests() {
	CTextureText testTextureText;
	for (unsigned int i = 0; i < 50; ++i) {
		testTextureText.addCharacter('c');
	}
	for (unsigned int i = 0; i < 60; ++i) {
		testTextureText.removeCharacter();
	}
	testTextureText.changeText("JAJASUPERNOVYTEXTTTTTTTTTT");
	testTextureText.changeText("");
}

void CTests::textureTests() {
	CTexture testTexture;
	assert(!testTexture.loadFromFile("NONEXISTANT"));
	assert(!testTexture.loadFromFile("NONEXISTANT"));
	assert(testTexture.loadFromFile(REL_ASSET_TEXTURE_FILE_PATH + "/Game/Background.png"));
	assert(testTexture.loadFromFile(REL_ASSET_TEXTURE_FILE_PATH + "/Game/Background.png"));
	assert(testTexture.loadFromFile(REL_ASSET_TEXTURE_FILE_PATH + "/Game/Background.png"));
	assert(!testTexture.loadFromRenderedText("TEST", nullptr, 30, toSDLColor(EColor::BLACK)));
	assert(testTexture.loadFromRenderedText("TEST", m_Map->m_Database->getFont(DATABASE_ID::FONT_ID::BUTTON), 30, toSDLColor(EColor::BLACK)));
	assert(testTexture.loadFromRenderedText("TEST", m_Map->m_Database->getFont(DATABASE_ID::FONT_ID::BUTTON), 30, toSDLColor(EColor::BLACK)));
}

bool CTests::loadMap(std::string path) {
	std::ifstream loadedMap(path);
	if (loadedMap.fail()) {
		return false;
	}
	bool result = m_Map->loadMapFromMapFile(loadedMap);
	loadedMap.close();
	return result;
}

bool CTests::loadSaveFile(std::string path) {
	std::ifstream loadedMap(path);
	if (loadedMap.fail()) {
		return false;
	}
	bool result = m_Map->loadMapFromSaveFile(loadedMap);
	loadedMap.close();
	return result;
}