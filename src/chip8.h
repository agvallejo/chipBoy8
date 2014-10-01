#pragma once

#include "defs.h"
#include "gfx.h"
#include "input.h"

#include <fstream>
#include <iostream>
#include <array>
#include <stack>

#include <stdlib.h>		
#include <stdint.h>
#include <string.h>
#include <time.h>

class chip8{
public:
	//In params: const char*	-> Path to game to be loaded
	//			 const uint8_t*	-> Keypad states given by the input class
	chip8(const char*, const uint8_t*);

	//Returns a const pointer to the screen for the gfx class to render it
	const uint8_t* getScreen();

	//Process opcodes
	void emulateCycle();

	//Decrement both DT and ST if they are non-zero
	void decrementTimers();

	//Returns drawFlag
	bool mustDraw();

private:
	//Determines whether a new render must be made or not
	bool drawFlag;

	uint8_t delayTimer;				//DT
	uint8_t soundTimer;				//ST

	uint16_t opcode;					//present opcode
	uint16_t I;						//Index register
	uint16_t pc;				//Program counter

	//The screen uses a 1D array for pointer economy
	uint8_t screen[CHIP8_WIDTH*CHIP8_HEIGHT];

	//Keypad pointer. It shows key states as shown by the input class
	const uint8_t* key;

	//CHIP-8 used a 12 levels stack, but many modern games asume bigger stacks,
	//so we are making it able to increase dynamically on the heap by using std::stack
	std::stack<uint16_t> stack;
	std::array<uint8_t, 16>		V;		//15 registers (16th is used for carries)
	std::array<uint8_t, 4096>	memory;

	void loadGame(const char*);
	void clearScreen();

};