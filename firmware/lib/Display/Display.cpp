#include <Display.h>

// declare the bitmaps
constexpr static const size16 gauge_size(GAUGE_WIDTH, GAUGE_HEIGHT);
uint8_t gauge_buf[bmp_type::sizeof_buffer(gauge_size)];
bmp_type gauge(gauge_size, gauge_buf);

void redraw_gauge(const float percent) {
  gauge.clear(gauge.bounds());
  if (percent > 0) {
    draw::filled_rounded_rectangle(
        gauge,
        srect16(0, gauge.bounds().height() * (1 - percent),
                gauge.bounds().width(), gauge.bounds().height()),
        0.2, bmpa_color::green);
  }
  draw::rounded_rectangle(gauge, (srect16)gauge.bounds(), 0.2,
                          bmpa_color::gray);
}

bool Display::init() {
  redraw_gauge(0);

  if (!lcd.initialize()) {
    return false;
  }

  lcd.clear(lcd.bounds());
  return true;
}

void Display::draw(const uint16_t rpm, const float speed) {
  char* rpms = new char[6];
  sprintf(rpms, "%d", rpm);

  const float text_scale = VeraMono_ttf.scale(32);
  auto text_size = VeraMono_ttf.measure_text(ssize16(LCD_WIDTH, LCD_HEIGHT),
                                             spoint16(0, 0), rpms, text_scale);
  auto text_pos = srect16(
      (LCD_WIDTH - text_size.width) / 2.0, LCD_HEIGHT - text_size.height,
      (LCD_WIDTH - text_size.width) / 2.0 + text_size.width, LCD_HEIGHT);

  uint8_t text_buf[bmp_type::sizeof_buffer((size16)text_size)];
  bmp_type text((size16)text_size, text_buf);
  text.clear(text.bounds());
  draw::text(text, text_size.bounds(), spoint16(0, 0), rpms, VeraMono_ttf,
             text_scale, lcd_color::ivory);

  // update the gauge bitmap
  redraw_gauge(speed);

  draw::suspend(lcd);

  // draw the new gauge over the previous one
  draw::bitmap(
      lcd,
      srect16(GAUGE_X_PADDING, GAUGE_Y_PADDING, GAUGE_X_PADDING + GAUGE_WIDTH,
              GAUGE_Y_PADDING + GAUGE_HEIGHT),
      gauge, gauge.bounds());
  // clear the area where the text lives
  lcd.clear(rect16(0, LCD_HEIGHT - text_size.height, LCD_WIDTH, LCD_HEIGHT));
  // draw the rpm text
  draw::bitmap(lcd, text_pos, text, text.bounds());

  draw::resume(lcd);
}
