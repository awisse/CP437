#include <SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "EEPROM.h"
#include "../CP437/defines.h"
#include "../CP437/platform.h"
#include "../CP437/model.h"
#include "Font5x7.h"

#define ZOOM_SCALE 4

SDL_Window* AppWindow = nullptr;
SDL_Renderer* AppRenderer = nullptr;
SDL_Surface* AppSurface = nullptr;
uint8_t sBuffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];
EEPROM eeprom;
unsigned long StartTime;
int zoom_scale;
point cursor;
#ifdef _DEBUG
int counter;
#endif

uint8_t InputMask = 0;
void cleanup();
size_t write(uint8_t c); // Write one character at the cursor. Characters 
                         // from Font5x7.h

// Helper
void SetColour(uint8_t colour) {

  if (colour == COLOUR_WHITE) {
    SDL_SetRenderDrawColor(AppRenderer, 255, 255, 255, 255);
  } else {
    SDL_SetRenderDrawColor(AppRenderer, 0, 0, 0, 255);
  }
}

// From Platform.h
// General
uint8_t* Platform::getBuffer() {
  return sBuffer;
}

// Buttons
uint8_t Platform::buttonsState()
{
  return InputMask;
}

// Drawing
void Platform::drawPixel(uint8_t x, uint8_t y, uint8_t colour) {

  // If outside of screen nothing to do
  if ((x<0) || (x>=SCREEN_WIDTH) || (y<0) || (y>=SCREEN_HEIGHT)) {
    return;
  }
  SetColour(colour);
  SDL_RenderDrawPoint(AppRenderer, x, y);
}

void Platform::drawBitmap(const uint8_t* data, int16_t x, int16_t y,
    uint8_t w, uint8_t h, uint8_t colour)
{
  for (int j = 0; j < h; j++)
  {
    for (int i = 0; i < w; i++)
    {
      int blockX = i / 8;
      int blockY = j / 8;
      int blocksPerWidth = w / 8;
      int blockIndex = blockY * blocksPerWidth + blockX;
      uint8_t pixels = data[blockIndex * 8 + i % 8];
      uint8_t mask = 1 << (j % 8);
      if (pixels & mask)
      {
        drawPixel(x + i, y + j, colour);
      }
    }
  }
}

void Platform::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    uint8_t colour) {

  SetColour(colour);

  if (SDL_RenderDrawLine(AppRenderer, x0, y0, x1, y1)) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h) {
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SetColour(COLOUR_WHITE);

  if (SDL_RenderDrawRect(AppRenderer, &rect) < 0) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t colour) {
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SetColour(colour);

  if (SDL_RenderFillRect(AppRenderer, &rect) < 0) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0+r, colour);
  drawPixel(x0, y0-r, colour);
  drawPixel(x0+r, y0, colour);
  drawPixel(x0-r, y0, colour);

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, colour);
    drawPixel(x0 - x, y0 + y, colour);
    drawPixel(x0 + x, y0 - y, colour);
    drawPixel(x0 - x, y0 - y, colour);
    drawPixel(x0 + y, y0 + x, colour);
    drawPixel(x0 - y, y0 + x, colour);
    drawPixel(x0 + y, y0 - x, colour);
    drawPixel(x0 - y, y0 - x, colour);
  }
}

void Platform::fillCircle(int16_t x0, int16_t y0, uint8_t r,
    uint8_t colour) {

  SetColour(colour);

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  // Horizontal center line
  drawLine(x0-r, y0, x0+r, y0, colour);

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawLine(x0+x, y0-y, x0-x, y0-y, colour);
    drawLine(x0+y, y0-x, x0-y, y0-x, colour);
    drawLine(x0+x, y0+y, x0-x, y0+y, colour);
    drawLine(x0+y, y0+x, x0-y, y0+x, colour);
  }
}

void Platform::fillScreen(uint8_t colour) {

  SetColour(colour);

  if (SDL_RenderClear(AppRenderer)) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::clear() {
  fillScreen(COLOUR_BLACK);
}

// Drawing desktop
void Platform::display(bool clear_screen) {
  // Transpose sBuffer to screen. Faster than drawPixel one by one
  int i, bit;
  SDL_Rect sq;

  if (clear_screen) clear();

  sq.w = sq.h = zoom_scale;

  for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT / 8; i++) {
    // 1 byte = 8 vertical pixels
    for (bit=0; bit<8; bit++) {
      if ((sBuffer[i] >> bit) & 0x01) {
        sq.y = zoom_scale * (i / SCREEN_WIDTH * 8 + 7 - bit);
        sq.x = zoom_scale * (i % SCREEN_WIDTH);
        if (SDL_FillRect(AppSurface, &sq, 0xFFFFFFFF)) {
          std::cerr << SDL_GetError() << "\n";
        }
      }
    }
  }
}

// Timer
unsigned long Platform::millis() {
  struct timespec ts;
  unsigned long ms;

  if (clock_gettime(CLOCK_REALTIME, &ts)) {
    std::cerr << "Can't get clock_gettime" << "\n";
  }
  ms = (1000 * ts.tv_sec  + ts.tv_nsec / 1000000) - StartTime;
  return ms;
}

// Print
void Platform::setCursor(int16_t x, int16_t y) {
  cursor.x = x;
  cursor.y = y;
}

size_t Platform::print(const char str[]) {
  return 1;
}

size_t Platform::print(char c) {
  return write(c);
}

size_t Platform::print(unsigned char c) {
  return write(c);
}

size_t Platform::print(int x, int fmt) {
  return 1;
}

size_t Platform::print(unsigned int x, int fmt) {
  return 1;
}

size_t Platform::print(long x, int fmt) {
  return 1;
}

size_t Platform::print(unsigned long x, int fmt) {
  return 1;
}

size_t Platform::print(float x, int decimals) {
  return 1;
}


size_t Platform::println(const char str[]) {
  return 1;
}

size_t Platform::println(char c) {
  return 1;
}

size_t Platform::println(unsigned char c) {
  return 1;
}

size_t Platform::println(int x, int fmt) {
  return 1;
}

size_t Platform::println(unsigned int x, int fmt) {
  return 1;
}

size_t Platform::println(long x, int fmt) {
  return 1;
}

size_t Platform::println(unsigned long x, int fmt) {
  return 1;
}

size_t Platform::println(float x, int decimals) {
  return 1;
}


// 
#ifdef _DEBUG
void Platform::DebugPrint(uint16_t value) {
  std::cout << value << ":";
  std::cout.flush();
  if (++counter % 8 == 0) {
    std::cout << "\n";
  }
}

void Platform::DebugPrint(unsigned long value) {
  std::cout << value << ":";
  std::cout.flush();
  if (++counter % 8 == 0) {
    std::cout << "\n";
  }
}

void Platform::DebugPrint(float value) {
  std::cout << value << ":";
  std::cout.flush();
  if (++counter % 8 == 0) {
    std::cout << "\n";
  }
}

void Platform::DebugPrint(const uint8_t* text) {
  std::cout << text << "\n";
}
#endif


// TODO: EEPROM
/* SavedState Platform::ToEEPROM(const uint8_t *bytes, */
/*     const uint16_t offset, const uint16_t sz) { */

/*   if (offset < 0) { */
/*     return WrongOffset; */
/*   } */

/*   int insertAt = offset + EEPROM_STORAGE_SPACE_START; */

/*   if (insertAt + sz > eeprom.length()) { */
/*     return TooBig; */
/*   } */

/*   eeprom.write(bytes, insertAt, sz); */
/*   return Saved; */
/* } */

/* SavedState Platform::FromEEPROM(uint8_t *bytes, const uint16_t offset, */
/*     const uint16_t sz) { */
/*   int getFrom = offset + EEPROM_STORAGE_SPACE_START; */

/*   if (getFrom < 0) { */
/*     return WrongOffset; */
/*   } */
/*   if (getFrom + sz > eeprom.length()) { */
/*     return TooBig; */
/*   } */

/*   eeprom.read(bytes, getFrom, sz); */
/*   return Saved; */
/* } */

// From Game.h
int Random(int i0, int i1) {
  int r;

  if (i0 == i1) return i0;

  r = ((int)random() & 0xFFFFFFFF) % (i1 - i0);

  return i0 + r;
}

// Local Functions
//
void Initialize() {
  struct timespec ts;
  memset(sBuffer, 0, sizeof(sBuffer));
  // Initialize timer from start of program
  if (clock_gettime(CLOCK_REALTIME, &ts)) {
    std::cerr << "Can't get clock_gettime" << "\n";
  }
  StartTime = 1000 * ts.tv_sec + ts.tv_nsec / 1000000;
  // Initialize game
  init_game();
  // Initialize random number generator.
  srandom(StartTime);
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
  uint32_t i; // For displaying surface values
  uint16_t pixel; // Setting a random pixel in sBuffer
#endif
  uint16_t frameCount = 0;
  zoom_scale = ZOOM_SCALE;
  if (argc == 2) {
    zoom_scale = atoi(argv[1]);
    if ((zoom_scale < 1) || (zoom_scale > 8)) {
      zoom_scale = ZOOM_SCALE;
      std::cerr << "Zoom must be between 1 and 8" << "\n";
    }
  }

  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer(SCREEN_WIDTH * zoom_scale, SCREEN_HEIGHT * zoom_scale,
      SDL_WINDOW_RESIZABLE, &AppWindow, &AppRenderer);
  SDL_RenderSetLogicalSize(AppRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  AppSurface = SDL_GetWindowSurface(AppWindow);

  Initialize();

  bool running = true;
  InputMask = 0;

  while (running)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {


        case SDL_QUIT:
          running = false;
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {

            case SDLK_a:
              InputMask |= INPUT_A;
              break;

            case SDLK_b:
              InputMask |= INPUT_B;
              break;

            case SDLK_LEFT:
              InputMask |= INPUT_LEFT;
              break;

            case SDLK_RIGHT:
              InputMask |= INPUT_RIGHT;
              break;

            case SDLK_UP:
              InputMask |= INPUT_UP;
              break;

            case SDLK_DOWN:
              InputMask |= INPUT_DOWN;
              break;

            case SDLK_ESCAPE:
              running = false;
              break;

#ifdef _DEBUG
            case SDLK_t:
              for (i=0; i<64; i++) {
                pixel = Random(0, sizeof(sBuffer) * 8);
                sBuffer[pixel >> 3] |= 1 << (pixel & 0x07);
              }
              Platform::DrawBuffer();
              SDL_UpdateWindowSurface(AppWindow);
              SDL_Delay(4000);
              break;
#endif

          }
          break;

        case SDL_KEYUP:
          switch (event.key.keysym.sym) {

            case SDLK_a:
              InputMask &= ~INPUT_A;
              break;

            case SDLK_b:
              InputMask &= ~INPUT_B;
              break;

            case SDLK_LEFT:
              InputMask &= ~INPUT_LEFT;
              break;

            case SDLK_RIGHT:
              InputMask &= ~INPUT_RIGHT;
              break;

            case SDLK_UP:
              InputMask &= ~INPUT_UP;
              break;

            case SDLK_DOWN:
              InputMask &= ~INPUT_DOWN;
              break;
          }
          break;
        }
      }

    step_model(frameCount);

    /* if (!eeprom.isSaved()) { */
    /*   eeprom.save(); */
    /* } */

    SDL_RenderPresent(AppRenderer);

    // FrameRate
    SDL_Delay(FRAME_DURATION);

  }

  cleanup();
  return 0;
}

void cleanup() {
  SDL_DestroyRenderer(AppRenderer);
  SDL_DestroyWindow(AppWindow);
  AppRenderer = nullptr;
  AppWindow = nullptr;
  SDL_Quit();
}

size_t write(uint8_t c) {
  Platform::drawBitmap(&font5x7[FONT_WIDTH * c], cursor.x, cursor.y, 5, 7);
  cursor.x += FONT_WIDTH + 1;
  if (cursor.x >= SCREEN_WIDTH) {
    cursor.x = 0;
    cursor.y += FONT_HEIGHT + 1;
    if (cursor.y >= SCREEN_HEIGHT) {
      cursor.y = 0;
    }
  }
  return 1;
}
  
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
