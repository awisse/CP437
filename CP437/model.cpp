// vim: ts=2:sts=2:expandtab

#include "model.h"
#include "platform.h"
#include "defines.h"

typedef enum {
  LOWER = 0,
  HIGHER = 128
} charset;

void show_characters(charset);
void show_float(void);

void initialize() {
  Platform::setTextRawMode(true);
}

void step_model(uint16_t frame) {

  uint8_t buttons = Platform::buttonsState();
  static uint8_t state = 0;
  bool done = false;

  Platform::pollButtons();
  if  (Platform::justPressed(INPUT_A)) {
    Platform::clear();
    state = (state + 1) % 3;
    done = false;
  }

  if (!done) {
    switch (state) {
      case 0:
        show_characters(LOWER);
        break;
      case 1:
        show_characters(HIGHER);
        break;
      case 2:
        show_float();
      default:
        break;
    }
    done = true;
  }
}

void show_characters(charset from) {
  uint16_t counter;

  for (counter = from; counter < from + 128; counter++) {
    Platform::setCursor((counter & 0x0F) * 6, ((counter & 0x7F) >> 4) << 3 );
    Platform::print((char)counter);
  }
}

void show_float(void) {

	float x, y, z, xzz, zz;
  uint32_t *ix, *iy, *iz, *izz, *ixzz;

  x = 2.;
  y = 0.34;
  z = 2.34;
  zz = z - (uint32_t)z;
  xzz = x + zz;
  ix = (uint32_t*)&x;
  iy = (uint32_t*)&y;
  iz = (uint32_t*)&z;
  izz = (uint32_t*)&zz;
  ixzz = (uint32_t*)&xzz;

  // Show the samples
  Platform::setTextRawMode(false);
  Platform::setCursor(0, 0);

  // x
  Platform::print("x   ");
  Platform::print(x, 6);
  Platform::print(" ");
  Platform::println(*ix, HEX);
  // y
  Platform::print("y   ");
  Platform::print(y, 6);
  Platform::print(" ");
  Platform::println(*iy, HEX);
  // z
  Platform::print("z   ");
  Platform::print(z, 6);
  Platform::print(" ");
  Platform::println(*iz, HEX);
  // zz
  Platform::print("zz  ");
  Platform::print(zz, 6);
  Platform::print(" ");
  Platform::println(*izz, HEX);
  // xzz
  Platform::print("xzz ");
  Platform::print(xzz, 6);
  Platform::print(" ");
  Platform::println(*ixzz, HEX);

  Platform::setTextRawMode(true);
}
