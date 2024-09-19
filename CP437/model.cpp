// vim: ts=2:sts=2:expandtab

#include "model.h"
#include "platform.h"
#include "matrix.h"
#include "defines.h"

point rotate(point *p0, uint8_t step);

void init_game() {}

void step_model(uint16_t frame) {

  // point p, p0, center;
  // int16_t x, y; // Coordinates to be drawn
  // uint16_t step = frame % (2 * ANGLES - 1);

  // center = { SCREEN_WIDTH / 2, 0 };
  // p0 = { SCREEN_HEIGHT - 1, 0 };

  // p = rotate(&p0, step);

  // x = center.x + p.x;
  // y = center.y + p.y;

  // Platform::setCursor(0, 0);
  // Platform::print("p(x, y) = ");
  // Platform::print("(");
  // Platform::print(p.x);
  // Platform::print(", ");
  // Platform::print(p.y);
  // Platform::println(")");
  // Platform::print("step    = ");
  // Platform::println(step);

  // Platform::drawPixel(x, y);


  // while (!Platform::pressed(UP_BUTTON)) {
  //   Platform::idle();
  // }
  uint8_t buttons = Platform::buttonsState();
  static uint8_t row;
  static uint8_t counter = 0;

  if ((counter & 0x7F) == 0x7F) {
    if  (buttons & INPUT_A) {
      Platform::clear();
    } else {
      return;
    }
  }

  if (counter++ < 256) {
    Platform::setCursor((counter & 0x0F) * 6, ((counter & 0x7F) >> 4) << 3 );
    Platform::print((char)counter);
  } else {
    counter = 0;
  }

}

/*************** Rotation matrix *******************
 * | cos(\theta) -sin(\theta) |
 * | sin(\theta)  cos(\theta) |
 *
 * We need only two float values per angle: cos(\theta) and sin(\theta)
*/

point rotate(point *p0, uint8_t step) {
  /* Rotate point p0 by step * 6  degrees around the center of the screen.
   * Return the rotated point.
   * Between 0 and Pi/2 for now.
   */
  point p = {0, 0};
  rotn  r;
  float cos, sin; // Dependent on step.

  if (step < ANGLES) {
    r = omega[step];
    cos = r.cos;
    sin = r.sin;
  }  else {
    r = omega[2 * (ANGLES - 1) - step];
    cos = -r.cos;
    sin = r.sin;
  }

  // Matrix multiplication
  p.x = cos * p0->x - sin * p0->y;
  p.y = sin * p0->x + cos * p0->y;

  return p;
}

