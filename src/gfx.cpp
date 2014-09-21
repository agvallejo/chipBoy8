#include "gfx.h"

gfx::gfx(const uint8_t* screenPtr){
	//The screen "chipBoy8" draws to is observed by gfx::screen*
	screen = screenPtr;

	window = SDL_CreateWindow(	PROG_NAME,
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								SCREEN_WIDTH,
								SCREEN_HEIGHT,
								SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(	window,
									-1,
									SDL_RENDERER_ACCELERATED);

	texture = SDL_CreateTexture(renderer,
								SDL_GetWindowPixelFormat(window),
								SDL_TEXTUREACCESS_STREAMING,
								CHIP8_WIDTH,
								CHIP8_HEIGHT);

	surface = SDL_CreateRGBSurface(	0,
									CHIP8_WIDTH,
									CHIP8_HEIGHT,
									32,
									0,0,0,0);

	//If there was any error upon initialization, show it
	if (!window || !renderer || !texture || !surface)
		std::cout << "Error initialiing graphics" << std::endl;

	renderTarget = { 0, 0, 1, 1 };
}

void gfx::updateScreen(){
	for (int i = 0; i < CHIP8_WIDTH*CHIP8_WIDTH; i++){
		renderTarget.x = i%CHIP8_WIDTH;
		renderTarget.y = i/CHIP8_WIDTH;

		if (screen[i])
			SDL_FillRect(surface, &renderTarget, 0xFFFFFFFF);
		else
			SDL_FillRect(surface, &renderTarget, 0x00000000);
	}

	SDL_UpdateTexture(texture, NULL, surface->pixels, CHIP8_WIDTH * sizeof (Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	return;
}

void gfx::freeRes(){
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}