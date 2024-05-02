#include "CDatabase.h"

CDatabase::CDatabase() {
	if (!loadTextures()) {
		throw NonExistingFileException();
	}
	if (!loadFonts()) {
		throw NonExistingFileException();
	}
	if (!loadSounds()) {
		throw NonExistingFileException();
	}
	if (!loadMusic()) {
		throw NonExistingFileException();
	}
}

CDatabase::~CDatabase() {
	m_Textures.clear();

	for (auto &x : m_Fonts) {
		TTF_CloseFont(x.second);
	}
	m_Fonts.clear();

	for (auto &x : m_Sounds) {
		Mix_FreeChunk(x.second);
	}
	m_Sounds.clear();

	for (auto &x : m_Music) {
		Mix_FreeMusic(x.second);
	}
	m_Music.clear();
}

CTexture *const CDatabase::getTexture(unsigned int id) {
	return &m_Textures[id];
}

TTF_Font *const CDatabase::getFont(unsigned int id) {
	return m_Fonts[id];
}

Mix_Chunk *const CDatabase::getSound(unsigned int id) {
	return m_Sounds[id];
}

Mix_Music *const CDatabase::getMusic(unsigned int id) {
	return m_Music[id];
}

//---------------------------- PRIVATE --------------------------------------//

bool CDatabase::loadTextures() {
	if (!loadMenuSharedTextures()) {
		return false;
	}
	if (!loadMainMenuTextures()) {
		return false;
	}
	if (!loadNewGameTextures()) {
		return false;
	}
	if (!loadGameTextures()) {
		return false;
	}
	if (!loadVictoryTextures()) {
		return false;
	}
	if (!loadDefeatTextures()) {
		return false;
	}
	if (!loadSaveTextures()) {
		return false;
	}

	return true;
}

bool CDatabase::loadMenuSharedTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::MENU;
	std::string path = "Menu/Shared/";

	if (!loadTexture(path + "Buttons/Available.png", BTN_AVAILABLE)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/NotAvailable.png", BTN_NOT_AVAILABLE)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/SaveFileAvailable.png", SAVE_FILE_AVAIL)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/SaveFileNotAvailable.png", SAVE_FILE_NOT_AVAIL)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/SaveFileSelected.png", SAVE_FILE_SELECTED)) {
		return false;
	}

	return true;
}

bool CDatabase::loadMainMenuTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::MENU::MAIN_MENU;
	std::string path = "Menu/MainMenu/";

	if (!loadTexture(path + "Background.png", SCREEN)) {
		return false;
	}

	return true;
}

bool CDatabase::loadNewGameTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::MENU::NEW_GAME;
	std::string path = "Menu/NewGame/";

	if (!loadTexture(path + "Background.png", SCREEN)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/ColorBlue.png", BLUE_BTN)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/ColorGreen.png", GREEN_BTN)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/ColorRed.png", RED_BTN)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/Difficulty.png", DIFF_BTN)) {
		return false;
	}

	if (!loadTexture(path + "Buttons/Selected.png", SEL_BTN)) {
		return false;
	}

	return true;
}

bool CDatabase::loadGameTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::GAME;
	std::string path = "Game/";

	if (!loadTexture(path + "Background.png", SCREEN)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Blue_Hive.png", BLUE_HIVE)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Green_Hive.png", GREEN_HIVE)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Red_Hive.png", RED_HIVE)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Gray_Hive.png", GRAY_HIVE)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Highlighted/Blue_Hive.png", HIGHLIGHTED_BLUE_HIVE)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Highlighted/Green_Hive.png", HIGHLIGHTED_GREEN_HIVE)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Highlighted/Red_Hive.png", HIGHLIGHTED_RED_HIVE)) {
		return false;
	}

	if (!loadTexture(path + "Hives/Highlighted/Gray_Hive.png", HIGHLIGHTED_GRAY_HIVE)) {
		return false;
	}

	return true;
}

bool CDatabase::loadVictoryTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::VICTORY;
	std::string path = "Victory/";

	if (!loadTexture(path + "Background.png", SCREEN)) {
		return false;
	}

	return true;
}

bool CDatabase::loadDefeatTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::DEFEAT;
	std::string path = "Defeat/";

	if (!loadTexture(path + "Background.png", SCREEN)) {
		return false;
	}

	return true;
}

bool CDatabase::loadSaveTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::MENU::SAVE;
	std::string path = "Menu/Save/";

	if (!loadTexture(path + "Background.png", SCREEN)) {
		return false;
	}

	return true;
}

bool CDatabase::loadLoadTextures() {
	using namespace DATABASE_ID::TEXTURE_ID::MENU::LOAD;
	std::string path = "Menu/Load/";

	if (!loadTexture(path + "Background.png", SCREEN)) {
		return false;
	}

	return true;
}

bool CDatabase::loadFonts() {
	using namespace DATABASE_ID::FONT_ID;

	if (!loadFont("OpenSans-Bold.ttf", 20, HIVE_SIZE)) {
		return false;
	}

	if (!loadFont("OpenSans-Bold.ttf", 48, DIFFICULTY_END_SCREEN)) {
		return false;
	}

	if (!loadFont("OpenSans-Bold.ttf", 30, INPUT)) {
		return false;
	}

	if (!loadFont("OpenSans-Bold.ttf", 45, BUTTON)) {
		return false;
	}

	if (!loadFont("OpenSans-Bold.ttf", 85, DIFFICULTY_LETTER)) {
		return false;
	}

	if (!loadFont("OpenSans-Bold.ttf", 30, SAVE_FILE)) {
		return false;
	}

	return true;
}

bool CDatabase::loadSounds() {
	using namespace DATABASE_ID::SOUND_ID;

	if (!loadSound("suntemple__access-denied.wav", INVALID_MOVE)) {
		return false;
	}

	return true;
}

bool CDatabase::loadMusic() {
	using namespace DATABASE_ID::MUSIC_ID;

	//Stardust (Ziggy is coming) by Kraftamt (c) copyright 2020 Licensed under a Creative Commons Attribution Noncommercial  (3.0) license. 
	//http://dig.ccmixter.org/files/Karstenholymoly/62493 Ft: Platinum Butterfly
	//compressed by https://www.onlineconverter.com/compress-mp3
	if (!loadMusicFile("Karstenholymoly-Stardust-Ziggy-is_coming.mp3", MAIN)) {
		return false;
	}

	return true;
}

bool CDatabase::loadTexture(std::string path, unsigned int textureId) {

	CTexture tmpTexture;
	std::string realPath = REL_ASSET_TEXTURE_FILE_PATH + "/" + path;
	if (!tmpTexture.loadFromFile(realPath)) {
		std::cout << "Failed to load \"" << realPath << "\" texture" << std::endl;
		return false;
	}

	if (!m_Textures.insert(std::pair<unsigned int, CTexture>(textureId, tmpTexture)).second) {
		std::cout << "Texture id " << textureId << " is already in use!" << std::endl;
		return false;
	}

	return true;
}

bool CDatabase::loadFont(std::string fontFileName, int fontSize, unsigned int fontID) {

	std::string fontPath(REL_FONT_FILE_PATH + "/");
	TTF_Font *tmpFont = nullptr;
	tmpFont = TTF_OpenFont((fontPath + fontFileName).c_str(), fontSize);

	if (tmpFont == nullptr) {
		std::cout << "Failed to load \"" << fontFileName << "\"" << " font!" << std::endl;
		return false;
	}
	m_Fonts.emplace(fontID, tmpFont);
	return true;
}

bool CDatabase::loadSound(std::string soundFileName, unsigned int soundId) {
	std::string soundPath(REL_SOUND_FILE_PATH + "/");

	Mix_Chunk *tmpSound = Mix_LoadWAV((soundPath + soundFileName).c_str());
	if (tmpSound == nullptr)
	{
		std::cout << "Failed to load \"" << soundFileName << "\"" << " sound!" << std::endl;
		return false;
	}

	m_Sounds.emplace(soundId, tmpSound);
	return true;
}

bool CDatabase::loadMusicFile(std::string musicFileName, unsigned int musicId) {
	std::string musicPath(REL_MUSIC_FILE_PATH + "/");

	Mix_Music *tmpMusic = Mix_LoadMUS((musicPath + musicFileName).c_str());
	if (tmpMusic == nullptr)
	{
		std::cout << "Failed to load \"" << musicFileName << "\"" << " sound!" << std::endl;
		return false;
	}

	m_Music.emplace(musicId, tmpMusic);
	return true;
}
