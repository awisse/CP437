# Description

## Template 

This is a template for developing an Arduboy2 program in parallel with a 
version for a desktop (or portable) computer.

This example contains a partial implementation of functions of 
the `Arduboy2` class in the `Platform` class which is useful for testing 
and debugging the code on a computer. It may serve as a template for 
new games.

In order to develop in parallel on a computer and the Arduboy, never use
the `Arduboy2.*` functions directly, but the `Platform::*` versions instead.

Use the *CP437* directory with the Arduino IDE. Execute `make` in the Desktop
directory. **Required**: The **SDL2** library must be installed.

## The example
Displays all 256 characters of IBM Code Page 437 on the screen using
the builtin 5x7 font.

Characters `0x0a` and `0x0d` have special treatment on the Arduboy. The
first is used as newline with the `print` statement and the second is 
ignored. Both are shown as the corresponding CP437 characters on the screen by setting
`arduboy.setTextRawMode(true)`.


## Instructions
Press **A** to cycle between the next 128 characters and a 
few floating point numbers.

# Future Improvements
Do away with platform.h and implement the `Arduboy2` class directly in 
*Desktop/ardusdl.cpp*. Adapt a version of the *Arduboy2.h* header for this purpose and 
`#include` it in *ardusdl.cpp*.

## Credits
Some pieces of printing code copied directly from [MLXXXp/Arduboy2](https://github.com/MLXXXp/Arduboy2.git).
