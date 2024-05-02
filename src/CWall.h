#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

#include "globalVar.h"

/// @brief impassable object
class CWall {
public:
	CWall(int x, int y, int w = 40, int h = 40);
	void render() const;
	bool collision(int x, int y) const;
	int getX() const;
	int getY() const;

private:
	SDL_Rect m_HitBox;
};
