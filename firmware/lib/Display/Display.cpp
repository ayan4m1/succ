#include <Display.h>

Display::Display() {
  display = new TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT);
}

Display::~Display() {
  free(display);
}

void Display::init() {
  display->init();
  display->fillScreen(TFT_BLACK);
}

void Display::draw(const double voc, const uint16_t rpm) {
  // todo: choose a bg color based on voc value
  display->fillScreen(TFT_BLACK);
  display->setTextColor(TFT_WHITE);
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->printf("%f.2", voc);
  display->setCursor(30, 0);
  display->printf("%d rpm", rpm);
}
