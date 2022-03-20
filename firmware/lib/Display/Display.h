#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <Sensor.h>
#include <TFT_eSPI.h>

#include <map>

#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240

const std::map<uint16_t, uint16_t> colorBands = {{50, TFT_GREEN},
                                                 {100, TFT_YELLOW},
                                                 {200, TFT_ORANGE},
                                                 {300, TFT_PURPLE},
                                                 {500, TFT_MAROON}};

struct Display {
  Display();
  ~Display();

  void init();
  void draw(const SensorReadings readings, const uint16_t rpm);

 private:
  uint16_t getBgColor(const float iaq);

  TFT_eSPI* display;
};
