#include <Display.h>

// declare the bitmaps
constexpr static const size16 gauge_size(GAUGE_WIDTH, GAUGE_HEIGHT);
uint8_t gauge_buf[bmp_type::sizeof_buffer(gauge_size)];
bmp_type gauge(gauge_size, gauge_buf);

void redraw_gauge(const float percent) {
  gauge.clear(gauge.bounds());
  draw::filled_rounded_rectangle(
      gauge,
      srect16(0, gauge.bounds().height() * (1 - percent),
              gauge.bounds().width(), gauge.bounds().height()),
      0.2, bmpa_color::green);
  draw::rounded_rectangle(gauge, (srect16)gauge.bounds(), 0.2,
                          bmpa_color::gray);
}

bool Display::init() {
  redraw_gauge(0);
  return lcd.initialize();
}

void Display::draw(const SensorReadings readings, const uint16_t rpm,
                   const float speed) {
  char* rpms = new char[6];
  sprintf(rpms, "%d", rpm);
  auto text_size =
      Bm437_ACM_VGA_9x14_FON.measure_text(ssize16(LCD_WIDTH, LCD_HEIGHT), rpms);
  auto text_pos = srect16((LCD_WIDTH - text_size.width) / 2.0, LCD_HEIGHT - 14,
                          LCD_WIDTH, LCD_HEIGHT);

  redraw_gauge(speed);

  draw::suspend(lcd);

  // clear the whole screen
  lcd.clear(lcd.bounds());
  // draw the gauge
  draw::bitmap(
      lcd,
      srect16(GAUGE_X_PADDING, GAUGE_Y_PADDING, GAUGE_X_PADDING + GAUGE_WIDTH,
              GAUGE_Y_PADDING + GAUGE_HEIGHT),
      gauge, gauge.bounds());
  // draw the rpm text
  draw::text(lcd, text_pos, rpms, Bm437_ACM_VGA_9x14_FON, lcd_color::ivory,
             lcd_color::black, false);

  draw::resume(lcd);
}

/*Display::Display() { display = new TFT_eSPI(SCREEN_WIDTH, SCREEN_HEIGHT); }

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
}*/
