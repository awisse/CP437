#pragma once

#include <stddef.h>
#include <stdint.h>
#include "defines.h"


typedef struct {
  uint16_t x, y;
} point;

class Platform
{
public:
  // General
  static uint8_t* getBuffer();

  // Buttons
  static uint8_t buttonsState();
  static bool pressed(uint8_t buttons);
  static void pollButtons(void);
  static bool justPressed(uint8_t button);
  static bool justReleased(uint8_t button);

  // Drawing Arduboy
  static void drawPixel(uint8_t x, uint8_t y, uint8_t colour=COLOUR_WHITE);
  static void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap,
    uint8_t w, uint8_t h, uint8_t colour=COLOUR_WHITE);
  static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    uint8_t colour=COLOUR_WHITE);
  static void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h);
  static void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h,
    uint8_t colour=COLOUR_WHITE);
  static void drawCircle(int16_t x0, int16_t y0, uint8_t r,
    uint8_t colour=COLOUR_WHITE);
  static void fillCircle(int16_t x0, int16_t y0, uint8_t r,
    uint8_t colour=COLOUR_WHITE);
  static void fillScreen(uint8_t colour);
  static void clear();
  static void display(bool clear);

  // Timer
  static unsigned long millis();

  // Text Functions
  static void setTextRawMode(bool raw);
  static void setCursor(int16_t x, int16_t y);
  static int16_t getCursorX(void);
  static int16_t getCursorY(void);

  // Print
  static size_t print(const char str[]);
  static size_t print(char c);
  static size_t print(unsigned char c);
  // Careful: `long` on Arduino is 16 bits, but 32 bits on a regular computer.
  static size_t print(int16_t x, uint8_t base=DEC);
  static size_t print(uint16_t x, uint8_t base=DEC);
  // Careful: `long` on Arduino is 32 bits, but 64 bits on a regular computer.
  static size_t print(int32_t x, uint8_t base=DEC);
  static size_t print(uint32_t x, uint8_t base=DEC);
  static size_t print(float x, uint8_t decimals=2);

  static size_t println(void);
  static size_t println(const char str[]);
  static size_t println(char c);
  static size_t println(unsigned char c);
  static size_t println(int16_t x, uint8_t base=DEC);
  static size_t println(uint16_t x, uint8_t base=DEC);
  static size_t println(int32_t x, uint8_t base=DEC);
  static size_t println(uint32_t x, uint8_t base=DEC);
  static size_t println(float x, uint8_t decimals=2);

  // EEPROM

  // Debug
#ifdef _DEBUG
  static void DebugPrint(uint16_t value);
  static void DebugPrint(unsigned long value);
  static void DebugPrint(float value);
  static void DebugPrint(const uint8_t* text);
#endif
	
#ifndef ARDUINO

#endif
};
// vim:ft=cpp
