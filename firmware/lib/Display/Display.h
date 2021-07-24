#include <Arduino.h>
#include <TFT_eSPI.h>

#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240

struct Display {
  Display();
  ~Display();

  void init();
  void draw(const double voc, const uint16_t rpm);

private:
  TFT_eSPI* display;
};
