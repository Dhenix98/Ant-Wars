#include "CApplication.h"

/*! \mainpage Ant Wars
 * \section About_game
 * This game is a war between ants, who are trying to conquer the whole ant world!\n
 * Only the smartest and the tenacious will win.\n
 * \section Controls
 * \subsection step1 Menu
 * All menu pages are controlled by mouse-pressing or pressing the ESC key, which is used to return to the previous state.\n
 * \subsection step2 Game
 * The game is also controlled by the mouse and ESC key, which are used to pause or go to the menu.\n
 * To attack or support, the player has to select any of his hives,\n
 * then he has to select the target hive, and if there isn't an obstacle in any way, the source hive will send the army.\n
 * If the targeted hive is not owned by the player, then it will be attacked; if the hive is owned by the player, then it will be supported.\n
 * If the player wishes to end the attack or support, then he can press the army or line of ants, and the army will retreat to its hive.\n
 * The player wins if he conquers all the hives (not including neutral ones).\n
 * \section Maps
 * All levels are saved in the directory "maps".\n
 * If you wish, you can add new maps if they have the correct structure and name.\n
 * \section Cheats
 * Win  = by pressing 'w'\n
 * Defeat = by pressing 'e'\n
 */

int main(int argc, char *args[])
{
	try {
		CApplication application;
		if (argc == 2 && std::string(args[1]) == "test") {
			application.runTests();
		}
		else {
			application.run();
		}
	}
	catch (AppInitException &e) {
		std::cout << e.what() << std::endl;
		std::cout << "Failed to initialize!" << std::endl;
	}
	catch (NonExistingFileException &e) {
		std::cout << e.what() << std::endl;
		std::cout << "Failed to load media!" << std::endl;
	}
	catch (CorruptedFileException &e) {
		std::cout << e.what() << std::endl;
		std::cout << "Failed to load map file!" << std::endl;
	}
	return 0;
}

