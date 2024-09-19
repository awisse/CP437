/* vim: ts=2:sts=2:expandtab
 *
 * Drawing and Rotating an arrow
 * 
 *
 *
 */

#include <Arduboy2.h>
#include <stdint.h>
#include "model.h"
#include "defines.h"
#include "platform.h"

Arduboy2 arduboy;

void setup() {
  // put your setup code here, to run once:

  arduboy.begin();

  while (!arduboy.pressed(A_BUTTON)) {
    arduboy.idle();
  }
  arduboy.clear();
  arduboy.setFrameDuration(FRAME_DURATION);
}

void loop() {

  while (!arduboy.nextFrame()) {
  };

  step_model(arduboy.frameCount);

  Platform::display(false);

}

/******* Begin Platform Class Function Implementations ********/

/******* Buttons **********************************************/
uint8_t Platform::buttonsState() {
  return arduboy.buttonsState();
}
/******* Drawing **********************************************/
static void Platform::drawPixel(uint8_t x, uint8_t y, uint8_t colour) {
  arduboy.drawPixel(x, y, colour);
}

static void Platform::drawBitmap(const uint8_t *bitmap, int16_t x, int16_t y,
  uint8_t w, uint8_t h, uint8_t colour) {
  arduboy.drawBitmap(bitmap, x, y, w, h, colour);
}

static void Platform::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour)
{
  arduboy.drawLine(x0, y0, x1, y1, colour);
}

static void Platform::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h)
{
  arduboy.drawRect(x, y, w, h);
}

static void Platform::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t colour)
{
  arduboy.fillRect(x, y, w, h, colour);
}

static void Platform::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour)
{
  arduboy.drawCircle(x0, y0, r, colour);
}

static void Platform::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour)
{
  arduboy.fillCircle(x0, y0, r, colour);
}

static void Platform::fillScreen(uint8_t colour)
{
  arduboy.fillScreen(colour);
}

static void Platform::clear()
{
  arduboy.clear();
}

static void Platform::display(bool clear)
{
  arduboy.display(clear);
}
/************* Text *************************************************/
static void Platform::setCursor(int16_t x, int16_t y)
{
  arduboy.setCursor(x, y);
}
// *** print ***
static size_t Platform::print(const char str[])
{
  arduboy.print(str);
}

static size_t Platform::print(char c)
{
  arduboy.print(c);
}

static size_t Platform::print(unsigned char c)
{
  arduboy.print(c);
}

static size_t Platform::print(int x, int fmt)
{
  arduboy.print(x, fmt);
}

static size_t Platform::print(unsigned int x, int fmt)
{
  arduboy.print(x, fmt);
}

static size_t Platform::print(long x, int fmt)
{
  arduboy.print(x, fmt);
}

static size_t Platform::print(unsigned long x, int fmt)
{
  arduboy.print(x, fmt);
}

static size_t Platform::print(float x, int digits)
{
  arduboy.print(x, digits);
}

// *** println ***
static size_t Platform::println(const char str[])
{
  arduboy.println(str);
}

static size_t Platform::println(char c)
{
  arduboy.println(c);
}

static size_t Platform::println(unsigned char c)
{
  arduboy.println(c);
}

static size_t Platform::println(int x, int fmt)
{
  arduboy.println(x, fmt);
}

static size_t Platform::println(unsigned int x, int fmt)
{
  arduboy.println(x, fmt);
}

static size_t Platform::println(long x, int fmt)
{
  arduboy.println(x, fmt);
}

static size_t Platform::println(unsigned long x, int fmt)
{
  arduboy.println(x, fmt);
}

static size_t Platform::println(float x, int digits)
{
  arduboy.println(x, digits);
}


