#include "CApplication.h"

/*! \mainpage Ant Wars
 * \section O_hře
 * Tato hra je válčení mezi mravenci, kteří se snaží ovládnout celý mravenčí svět!\n
 * Vyhraje pouze ten nejchytřejší a nejvytrvalejší.\n
 * \section Ovládání
 * \subsection step1 Menu
 * Všechna Menu se ovládá myší mačkáním tlačítek, popřípadě ESC, který slouží k vrácení se na minulou "stránku".
 * \subsection step2 Hra
 * Hra samotná se také ovládá myší a ESC slouží pouze na Pauzu/návrat do menu.\n
 * K útoku nebo podpoře musí hráč vybrat některé svoje hnízdo,\n
 * poté vybere cílové hnízdo a pokud nebude mít v cestě překážku,tak pošle armádu.\n
 * Pokud hnízdo není hrače, tak zautočí a pokud cílové hnízdo je hráče, tak ho podpoří.\n
 * Pokud chce hráč ukončit útok, tak zmáčkne svoji linii/armádu a ta ustoupí zpátky do svého hnízda.\n
 * Hráč vyhrává pokud dobije všechna hnízda (nemusí neutrální).
 * \section Mapy
 * ve složce examples/maps jsou dve složky testMaps a realMaps.\n
 * TestMaps slouží pouze k vyzkoušení funkcí hry, a pokud je chceme aktivovat, tak je zkopírujeme o složku níž.\n
 * RealMaps slouží ke hraní a pokud je chceme aktivovat, tak stejně jako testMaps, je zkopírujeme o složku níž.
 * \section Cheaty
 * výhra  = zmáčknutí 'w'\n
 * prohra = zmáčknutí 'e'
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

