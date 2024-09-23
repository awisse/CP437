/* vim: ts=2:sts=2:expandtab

 * Constants
 *
 * NOTE: AVOID USING `int`, `long`, `unsigned int` and `unsigned long` in code,
 * int is int16_t on Arduboy and int32_t on a Linux computer.
 * Use the corresponding stdint values instead.
 */
#pragma once

#define _DEBUG
// Platform dependant definitions
#ifndef ARDUINO
#include <stdint.h>
#include <string.h>
#define PROGMEM
#define PSTR
#define pgm_read_byte(x) *(x)
#define pgm_read_word(x) (*((uint16_t*)x))
#define pgm_read_dword(x) (*((uint32_t*)x))
#define pgm_read_float(x) (*((float*)x))
#define pgm_read_ptr(x) (*((uintptr_t*)x))
#define strlen_P(x) strlen(x)
// If you absolutely must use `long` and `int`,
// use these instead: `long_` and `int_`.
typedef int long_;
typedef short int_;
#else
//#define pgm_read_...:
#include <avr/pgmspace.h>
typedef long long_;
typedef int int_;
#endif

#define FRAME_DURATION 20
#define PI 3.141592653589793

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DEC 10
#define HEX 16
#define OCT 8
#define BINARY 2

// Colours
#define COLOUR_WHITE 1
#define COLOUR_BLACK 0

// Buttons
#define INPUT_LEFT 32u
#define INPUT_RIGHT 64u
#define INPUT_UP 128u
#define INPUT_DOWN 16u
#define INPUT_A 8u
#define INPUT_B 4u

// Typedefs
typedef struct {
  int16_t x, y;
} point;

