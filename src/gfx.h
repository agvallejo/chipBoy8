#pragma once

#include "defs.h"

#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

class gfx{
	public:
		gfx(const uint8_t*);
		void freeRes();
		void updateScreen();

	private:
		const uint8_t* screen;
		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_Texture *texture;
		SDL_Surface *surface;
		SDL_Rect renderTarget;
};