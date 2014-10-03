chipBoy8
========
> A CHIP-8 interpreter written in C++ using SDL2.

Even though there are still some bugs left, the code is pretty much functional already by making a good number of games playable. The code has only been tested on Windows and ArchLinux, but Mac OS X should be supported, as well as the different *BSD flavours.


## Usage
As of now, you can execute a CHIP-8 program by:
  - Dragging and dropping the CHIP-8 binary over the interpreter executable
  - Executing the interpreter in the command line with the CHIP-8 program path as its first argument


## Dependencies and building instructions

In order to build **chipBoy8**, [SDL2](https://www.libsdl.org/download-2.0.php) and [CMake](http://www.cmake.org/download/) are needed. **SDL2**'s development libraries for compilation and its runtime libraries for execution of the binary.

This project uses **CMake** to ensure portability across different platforms and/or compilers.

> NOTE FOR WINDOWS: As Windows lacks default *include* or *lib* folders, the development libraries must be extracted somewhere else. It doesn't really matter, but an environment variable named **SDL2** must be created showing their path (e.g: `$(SDL2)=C:\SDL`), so that **CMake** knows where to look for them.

  1. `cd <repository root>`
  2. `mkdir build`
  3. `cd build`
  4. `cmake ..`

The **build** directory should be populated now with whatever files your compiler needs. In case of Visual Studio on Windows those are *.sln* and *.vcxproj* files (among others). CMake can be instructed to populate **build** with files for other compilers (Projects for XCode or Makefiles, for instance), as explained in [the documentation of CMake](http://www.cmake.org/cmake/help/v3.0/manual/cmake.1.html)

The project must be compiled now. In Visual Studio that is pressing *F5* in the IDE, while for *NIX systems most likely `./configure && make` is the way to go.

## TODO
  - Fix bugs (known and unknown)
  - Check if timing is consistent across different machines
  - Make use of SDL's sound API to emulate the buzzer
  - Remake the gigantic opcode-decoding switch as function pointer arrays
  - Introduce wxWidgets in order to load games easily
