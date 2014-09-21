chipBoy8
========

> A CHIP-8 interpreter written in C++ using SDL2.

Even though there are still some bugs left, the code is pretty much functional already by making a good number of games playable.

## Usage

As of now, you can execute a CHIP-8 program by:
  - Dragging and dropping the CHIP-8 binary over the interpreter executable
  - Executing the interpreter in the command line with the CHIP-8 program path as its first argument

## TODO
  - Fix bugs (known and unknown)
  - Check if timing is consistent across different machines
  - Make use of SDL's sound API to emulate the buzzer
  - Remake the gigantic opcode-decoding switch as function pointer arrays
  - Introduce wxWidgets in order to load games easily