# Description

Displays all 256 characters of IBM Code Page 437 on the screen using
the builtin 5x7 font.

Characters `0x0a` and `0x0d` have special treatment on the Arduboy. The
first is used as newline with the `print` statement and the second is 
ignored. Both are shown as the CP437 characters on the screen by setting
`arduboy.setTextRawMode(true)`.

This example contains a fairly complete implementation of functions of 
the `Arduboy2` class in the `Platform` class which is useful for testing 
and debugging the code on a computer. It may serve as a template for 
new games.

## Instructions
Press **A** to cycle between the next 128 characters and a 
few floating point numbers.

## Future Improvements
Do away with platform.h and implement the `Arduboy2` class directly in 
*Desktop/ardusdl.cpp*. Adapt the *Arduboy2.h* header for this purpose and 
import it in *ardusdl.cpp*.
