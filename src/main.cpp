#include <SDL.h>
#include <iostream>
#include "input.h"
#include "gfx.h"
#include "chip8.h"

int main(int argc, char **argv){
	if (argc != 2) {
		std::cout << "ERROR: Enter *.c8/*.ch8 as first and single argument or drag 'n drop it over the chipBoy8 bin" << std::endl;
		std::cout << std::endl << "Press ENTER to exit..." << std::endl;
		std::cin.ignore();
		return -1;
	}

	//SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		std::cout << std::endl << "Press ENTER to exit..." << std::endl;
		std::cin.ignore();
		return -1;
	}

	//Interpreter and IO classes initialization.
	input inputManager;
	chip8 chipBoy8(argv[1],inputManager.getKeypad());
	gfx gfxManager(chipBoy8.getScreen());

	//Variables used as simple timers.
	//slowTime is used to make a 60Hz timer [operate on internal timers and update keypad]
	//fastTime is used to make a 1kHz timer [interpret opcode]
	uint32_t slowTime = SDL_GetTicks();
	uint32_t fastTime = SDL_GetTicks();

	SDL_Event e;

	while(1){
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT)
			break;

		if (chipBoy8.mustDraw())
			gfxManager.updateScreen();

		if ((slowTime + 17) < SDL_GetTicks()) {
			chipBoy8.decrementTimers();
			inputManager.updateKeypad();
			slowTime = SDL_GetTicks();
		}

		if (fastTime < SDL_GetTicks()) {
			chipBoy8.emulateCycle();
			fastTime = SDL_GetTicks();
		}
	}

	//Frees allocated memory and exits SDL
	gfxManager.freeRes();
	SDL_Quit();
	return 0;
}