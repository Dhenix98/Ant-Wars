#include "CApplication.h"

CApplication::CApplication() :
	m_Database(std::make_shared<CDatabase>()),
	m_Game(std::make_unique<CGame>(m_Database))
{}

// was mostly taken over from https://lazyfoo.net/tutorials/SDL/
CApplication::CSDL::CSDL() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		throw AppInitException(std::string("SDL could not initialize! SDL Error: ") + SDL_GetError());
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering is not enabled!" << std::endl;
	}

	//Create window
	G_WINDOW = SDL_CreateWindow("Ant Wars", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (G_WINDOW == nullptr) {
		throw AppInitException(std::string("Window could not be created! SDL Error: ") + SDL_GetError());
	}

	//Create renderer for window
	G_RENDERER = SDL_CreateRenderer(G_WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (G_RENDERER == nullptr) {
		throw AppInitException(std::string("Renderer could not be created! SDL Error: ") + SDL_GetError());
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(G_RENDERER, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		throw AppInitException(std::string("SDL_image could not initialize! SDL_image Error: ") + IMG_GetError());
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		throw AppInitException(std::string("SDL_ttf could not initialize! SDL_ttf Error: %s\n") + TTF_GetError());
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		throw AppInitException(std::string("SDL_mixer could not initialize! SDL_mixer Error: %s\n") + Mix_GetError());
	}

	//Load support for the MP3 sample/music format
	int mixFlags = MIX_INIT_MP3;
	int mixInit = Mix_Init(mixFlags);

	if((mixInit & mixFlags) != mixFlags) {
		throw AppInitException(std::string("Mix_Init: Failed to init required mp3 support!\nMix_Init: %s\n") + Mix_GetError());
	}

	SDL_SetWindowResizable(G_WINDOW, SDL_FALSE);
}

CApplication::CSDL::~CSDL() {
	SDL_DestroyRenderer(G_RENDERER);
	SDL_DestroyWindow(G_WINDOW);

	//Quit ALL SDL subsystems
	SDL_Quit();
}

void CApplication::run() {

	bool quitApplication = false;

	//Event handler
	SDL_Event eventHandler;

	//50% music volume
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	//Play music
	Mix_PlayMusic(m_Database->getMusic(DATABASE_ID::MUSIC_ID::MAIN), -1);

	//While application is running
	while (!quitApplication)
	{
		//handle SDL events on queue
		while (SDL_PollEvent(&eventHandler)) {
			m_Game->handleSDLEvents(eventHandler, quitApplication);
		}
		m_Game->doEvents();

		//Clear screen
		SDL_SetRenderDrawColor(G_RENDERER, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(G_RENDERER);

		//Render level/menu
		m_Game->render();

		//Update screen
		SDL_RenderPresent(G_RENDERER);

	}
	//Stop music
	Mix_HaltMusic();

	// cleanup
	while (SDL_PollEvent(&eventHandler)) {
		if (eventHandler.type == CCustomEvents::nextStageEvent) {
			delete static_cast<EStage *>(eventHandler.user.data1);
		}
	}
}

void CApplication::runTests() {
	SDL_HideWindow(G_WINDOW);
	CTests tmpTest(m_Game, m_Database);
	tmpTest.runAll();
}
