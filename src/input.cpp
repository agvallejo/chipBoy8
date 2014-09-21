#include "input.h"
#include <stdio.h>

input::input() {
	currentKeyStates = SDL_GetKeyboardState(NULL);
}

const uint8_t* input::getKeypad() {
	return key;
}

void input::updateKeypad(){
	key[0x00] = currentKeyStates[SDL_SCANCODE_X];
	key[0x01] = currentKeyStates[SDL_SCANCODE_1];
	key[0x02] = currentKeyStates[SDL_SCANCODE_2];
	key[0x03] = currentKeyStates[SDL_SCANCODE_3];
	key[0x04] = currentKeyStates[SDL_SCANCODE_Q];
	key[0x05] = currentKeyStates[SDL_SCANCODE_W];
	key[0x06] = currentKeyStates[SDL_SCANCODE_E];
	key[0x07] = currentKeyStates[SDL_SCANCODE_A];
	key[0x08] = currentKeyStates[SDL_SCANCODE_S];
	key[0x09] = currentKeyStates[SDL_SCANCODE_D];
	key[0x0A] = currentKeyStates[SDL_SCANCODE_Z];
	key[0x0B] = currentKeyStates[SDL_SCANCODE_C];
	key[0x0C] = currentKeyStates[SDL_SCANCODE_4];
	key[0x0D] = currentKeyStates[SDL_SCANCODE_R];
	key[0x0E] = currentKeyStates[SDL_SCANCODE_F];
	key[0x0F] = currentKeyStates[SDL_SCANCODE_V];
}