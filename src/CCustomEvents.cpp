#include "CCustomEvents.h"

uint32_t CCustomEvents::nextStageEvent = 0;

void CCustomEvents::sendQuitApp() {
	SDL_Event myEvent;
	myEvent.type = SDL_QUIT;
	SDL_PushEvent(&myEvent);
}

void CCustomEvents::sendNextStage(EStage nextStage) {
	SDL_Event myEvent;
	myEvent.type = nextStageEvent;
	myEvent.user.data1 = new EStage(nextStage);
	SDL_PushEvent(&myEvent);
}
