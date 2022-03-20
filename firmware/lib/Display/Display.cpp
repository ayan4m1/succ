#include <Display.h>

Display::Display() { display = new TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT); }

Display::~Display() { free(display); }

void Display::init() {
  display->init();
  display->fillScreen(TFT_BLACK);
  display->setTextFont(0);
}

// uint16_t getBgColor(float iaq) {
//   for (auto iterator = colorBands.begin(); iterator != colorBands.end();
//        iterator++) {
//     if (iaq < iterator->first) {
//       return iterator->second;
//     }
//   }

//   return TFT_RED;
// }

uint16_t Display::getBgColor(const float iaq) {
  auto iterator = colorBands.begin();

  // handle case where IAQ is very low
  if (iaq < iterator->first) {
    return iterator->second;
  }

  // linear interpolation of the two colors we are sitting between
  for (iterator = colorBands.begin(); iterator != colorBands.end();
       iterator++) {
    const uint16_t startThreshold = iterator->first;
    const uint16_t startColor = iterator->second;
    uint16_t endColor = TFT_BLACK;
    uint16_t endThreshold = iterator->first;

    if (iterator != colorBands.end()) {
      iterator++;
      endColor = iterator->second;
      endThreshold = iterator->first;
    }

    if (iaq < endThreshold && iaq >= startThreshold) {
      const float theta =
          ((iaq - startThreshold) / (endThreshold - startThreshold)) * 255;
      return lerp16by8(startColor, endColor, theta);
    }
  }

  // handle cases where IAQ is > 500
  return TFT_BLACK;
}

void Display::draw(const SensorReadings readings, const uint16_t rpm) {
  // todo: choose a bg color based on voc value
  display->fillScreen(getBgColor(readings.iaq));
  display->setTextColor(TFT_WHITE);
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->printf("%d rpm", rpm);
}
