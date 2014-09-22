#include "chip8.h"

chip8::chip8(const char* gameName, const uint8_t* keyPtr){
	//---Keypad introduced by the input class. <Read only!>
	key = keyPtr;

	//---PC's default state
	pc = 0x200;

	//---Reset of system variables
	I = 0;
	memory.fill(0);
	V.fill(0);
	clearScreen();

	//---CHIP-8 font sprites
	uint8_t chip8_fontset[80] =	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	for (auto i : memory)
		i = chip8_fontset[i];

	//---Random seed initialization
	srand((unsigned int) time(NULL));

	//---Load <gameName> into memory from 0x200 onwards
	loadGame(gameName);
}

const uint8_t* chip8::getScreen() {
	return screen;
}

void chip8::emulateCycle(){
	//At first, drawFlag is unset. If at some time
	//the screen needs to be updated, it'll be set.
	drawFlag = false;

	//---Extract opcode and increase PC
	opcode = (memory[pc] << 8) | memory[pc + 1];
	pc += 2;

	//---Opcode decoding
	switch ((opcode & 0xF000) >> 12){	//Most significant bits are extracted
	case 0:
		if (opcode == 0x00E0)			//CLS: clears the screen
			clearScreen();
		else if (opcode == 0x00EE){		//RET:	Returns from subroutine
			pc = stack.top();
			stack.pop();
//			sp--;
//			pc = stack[sp];
		}
		break;
	case 1:								//JP: Jumps to addr NNN [0x1NNN]
		pc = opcode & 0x0FFF;
		break;
	case 2:								//CALL: Calls subroutine at NNN [0x2NNN]
//		stack[sp] = pc;
//		sp++;
		stack.push(pc);
		pc = opcode & 0x0FFF;
		break;
	case 3:								//SE: Skips next inst. if VX==NN [0x3XNN]
		if (V.at((opcode & 0x0F00) >> 8) == (opcode & 0x00FF))
			pc += 2;
		break;
	case 4:								//SNE: Skips next inst. if VX!=NN [0x4XNN]
		if (V.at((opcode & 0x0F00) >> 8) != (opcode & 0x00FF))
			pc += 2;
		break;
	case 5:								//SE: Skip next inst. if VX==VY [0x5XY0]
		if (V.at((opcode & 0x0F00) >> 8) == V.at((opcode & 0x00F0) >> 4))
			pc += 2;
		break;
	case 6:								//LD: VX=NN [0x6XNN]
		V.at((opcode & 0x0F00) >> 8) = (opcode & 0x00FF);
		break;
	case 7:								//ADD: VX+=NN [0x7XNN]
		V.at((opcode & 0x0F00) >> 8) += (opcode & 0x00FF);
		break;
	case 8:
		switch (opcode & 0xF00F){
		case 0x8000:					//LD: VX=VY  [0x8XY0]
			V.at((opcode & 0x0F00) >> 8) = V.at((opcode & 0x00F0) >> 4);
			break;
		case 0x8001:					//OR: VX|=VY [0x8XY1]
			V.at((opcode & 0x0F00) >> 8) |= V.at((opcode & 0x00F0) >> 4);
			break;
		case 0x8002:					//AND: VX&=VY [0x8XY2]
			V.at((opcode & 0x0F00) >> 8) &= V.at((opcode & 0x00F0) >> 4);
			break;
		case 0x8003:					//XOR: VX^=VY [0x8XY3]
			V.at((opcode & 0x0F00) >> 8) ^= V.at((opcode & 0x00F0) >> 4);
			break;
		case 0x8004:					//ADD: VX+=VY [0x8XY4]
			if ((15 - V.at((opcode & 0x0F00) >> 8)) < V.at((opcode & 0x00F0) >> 4))
				V.at(0xF) = 1;		//V[F] is set in case of overflow
			else
				V.at(0xF) = 0;
			V.at((opcode & 0x0F00) >> 8) += V.at((opcode & 0x00F0) >> 4);
			break;
		case 0x8005:					//SUB: VX-=VY [0x8XY5]
			if (V.at((opcode & 0x0F00) >> 8) < V.at((opcode & 0x00F0) >> 4))
				V.at(0xF) = 0;		//V[F] is unset if there's a borrow
			else
				V.at(0xF) = 1;
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			break;
		case 0x8006:					//SHR: VX>>=1 [0x8X06]
			if (V[(opcode & 0x0F00) >> 8] & 0x01)
				V.at(0xF) = 1; //if VX is odd, VF is set
			else
				V.at(0xF) = 0;
			V[(opcode & 0x0F00) >> 8] >>= 1;
			break;
		case 0x8007:					//SUBN: VX=VY-VX [0x8XY7]
			if (V.at((opcode & 0x0F00) >> 8) > V.at((opcode & 0x00F0) >> 4))
				V.at(0xF) = 0;		//V[F] is unset if there's a borrow
			else
				V.at(0xF) = 1;
			V.at((opcode & 0x0F00) >> 8) = V.at((opcode & 0x00F0) >> 4) - V.at((opcode & 0x0F00) >> 8);
			break;
		case 0X800E:					//SHL: VX<<=1 [0x8XYE]
			if (V.at((opcode & 0x0F00) >> 8) & 0x80)
				V.at(0xF) = 1;
			else
				V.at(0xF) = 0;
			V.at((opcode & 0x0F00) >> 8) <<= 1;
			break;
		default:
			std::cout << "Unrecognised opcode:" << opcode << std::endl;
		} //0x8000 switch end
		break;
	case 9:							//SNE: Skips next inst. if VX!=VY [0x9XY0]
		if (V.at((opcode & 0x0F00) >> 8) != V.at((opcode & 0x00F0) >> 4))
			pc += 2;
		break;
	case 0xA:						//LD: I=NNN [0xANNN]
		I = opcode & 0x0FFF;
		break;
	case 0xB:						//JP: Jumps to addr V0+NNN [0xBNNN]
		pc = V.at(0) + (opcode & 0x0FFF);
		break;
	case 0xC:						//RND: VX=8bit random AND NN [0xCXNN]
		V.at((opcode & 0x0F00) >> 8) = (rand() % 256) & (opcode & 0x00FF);
		break;
	case 0xD:						//DRW: Draws N-bytes height sprite to screen at (VX,VY) XORed [0xDXYN]
		uint8_t sprite, x, y;
		V.at(0xF) = 0;

		for (int i = 0; i < (opcode & 0x000F); i++){
			sprite = memory[I + i];
			for (int j = 0; j < 8; j++){
				if (sprite & (0x80 >> j)){
					x = V[(opcode & 0x0F00) >> 8] + j;		// This particular variables are needed
					y = V[(opcode & 0x00F0) >> 4] + i;		// so that overflow happens

					if (x>(CHIP8_WIDTH-1))
						x %= CHIP8_WIDTH;
					if (y>(CHIP8_HEIGHT-1))
						y %= CHIP8_HEIGHT;

					screen[x + y*CHIP8_WIDTH] ^= 1;
					if (screen[x + y*CHIP8_WIDTH] == 0)
						V.at(0xF) = 1;
				}//end if sprite
			}//end for j
		}//end for i
		drawFlag = true;
		break;
	case 0xE:
		switch (opcode & 0xF0FF){
		case 0xE09E:				 //SKP: Skips next inst. if key[VX] is pressed [0xEX9E]
			if (key[V.at((opcode & 0x0F00) >> 8)])
				pc += 2;
			break;
		case 0xE0A1:					//SKNP: Skips next inst. if key[VX] isn't pressed [0xEXA1]
			if (!key[V.at((opcode & 0x0F00) >> 8)])
				pc += 2;
			break;
		default:
			std::cout << "Opcode de tipo 0xE000 no reconocido" << std::endl;
		}//Fin del switch de 0xE000
		break;
	case 0xF:
		switch (opcode & 0xF0FF){
		case 0xF007:				//LD: VX=DT [0xFX07]
			V.at((opcode & 0x0F00) >> 8) = delayTimer;
			break;
		case 0xF00A:				//LD: Waits for keypress and VX=key [0xFX0A]
			//If no key is pressed, then pc-=2 overrides pc+=2 at the beginning
			//of the method and a new check is made.
			pc -= 2;
			for (int i = 0; i < 16; i++){
				if (key[i]){
					V.at((opcode & 0x0F00) >> 8) = i;
					pc += 2;
					break;
				}
			}
			break;
		case 0xF015:				//LD: DT=VX [0xFX15]
			delayTimer = V.at((opcode & 0x0F00) >> 8);
			break;
		case 0xF018:				//LD: ST=VX [0xFX18]
			soundTimer = V.at((opcode & 0x0F00) >> 8);
			break;
		case 0xF01E:				//ADD: I+=VX [0xFX1E]
			I += V.at((opcode & 0x0F00) >> 8);
			break;
		case 0xF029:				//LD: I=location of sprite for digit VX [0xFX29]
			I = 5 * V.at((opcode & 0x0F00) >> 8);
			break;
		case 0xF033:				//LD: Store the BCD representation of VX in locations I, I+1 and I+2. [0xFX33]
			memory.at(I) = V.at((opcode & 0x0F00) >> 8) / 100;
			memory.at(I + 1) = (V.at((opcode & 0x0F00) >> 8) / 10) - memory.at(I) * 10;
			memory.at(I + 2) = V.at((opcode & 0x0F00) >> 8) - ((memory.at(I) * 10 + memory.at(I + 1)) * 10);
			break;
		case 0xF055:				//LD: Store registers V0 through Vx in memory starting at location I. [0xFX55]
			for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
				memory.at(I + i) = V.at(i);
			break;
		case 0xF065:				//LD: Read registers V0 through Vx from memory starting at location I. [0xFX65]
			for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
				V.at(i) = memory.at(I + i);
			break;
		default:
			std::cout << "Unrecognised opcode:" << opcode << std::endl;
			break;
		}//0xF000 switch end
		break;
	default:
		std::cout << "Unrecognised opcode:" << opcode << std::endl;
		break;
	}//Opcodes switch end
}

bool chip8::loadGame(const char* fname) {
	FILE *inFile;
	size_t binSize;

	if (fopen_s(&inFile, fname, "rb")) {
		printf("Error al abrir el fichero '%s'.\n", fname);
		return false;
	}



	fseek(inFile, 0, SEEK_END);
	binSize = ftell(inFile);
	rewind(inFile);

	fread(memory.data() + 0x200, 1, binSize, inFile);
	fclose(inFile);

	return true;

}

void chip8::clearScreen() {
	memset(screen, 0, CHIP8_HEIGHT*CHIP8_WIDTH);
}

void chip8::decrementTimers() {
	if (delayTimer)
		delayTimer--;
	if (soundTimer)
		soundTimer--;
	return;
}

bool chip8::mustDraw() {
	return drawFlag;
}