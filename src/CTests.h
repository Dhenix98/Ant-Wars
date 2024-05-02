#pragma once
#include <memory>
#include "CGame.h"

class CTests {
public:
	CTests(const std::unique_ptr<CGame> &game, DatabasePtr database);
	void runAll();

	void loadTests();
	void hiveTests();
	void lineTests();
	void textureTextTests();
	void textureTests();
private:
	bool loadMap(std::string path);
	bool loadSaveFile(std::string path);

	MapPtr m_Map;
	DatabasePtr m_Database;
};

