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

  // Drawing Arduboy
  static void drawPixel(uint8_t x, uint8_t y, uint8_t colour=COLOUR_WHITE);
  static void drawBitmap(const uint8_t* bitmap, int16_t x, int16_t y,
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
  // Drawing Desktop
  static void display(bool clear);
  // Timer
  static unsigned long millis();

  // Print
  static void setCursor(int16_t, int16_t);
  static size_t print(const char str[]);
  static size_t print(char c);
  static size_t print(unsigned char c);
  static size_t print(int x, int=DEC);
  static size_t print(unsigned int x, int=DEC);
  static size_t print(long x, int=DEC);
  static size_t print(unsigned long x, int=DEC);
  static size_t print(float x, int=2);

  static size_t println(const char str[]);
  static size_t println(char c);
  static size_t println(unsigned char c);
  static size_t println(int x, int=DEC);
  static size_t println(unsigned int x, int=DEC);
  static size_t println(long x, int=DEC);
  static size_t println(unsigned long x, int=DEC);
  static size_t println(float x, int=2);

  // EEPROM

  // Debug
#ifdef _DEBUG
  static void DebugPrint(uint16_t value);
  static void DebugPrint(unsigned long value);
  static void DebugPrint(float value);
  static void DebugPrint(const uint8_t* text);
#endif
	
};
// vim:ft=cpp
