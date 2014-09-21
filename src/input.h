#pragma once

#include <stdint.h>
#include <SDL.h>

class input{
public:
	input();
	const uint8_t* getKeypad();
	void updateKeypad();

private:
	uint8_t key[16];
	const uint8_t* currentKeyStates;
};