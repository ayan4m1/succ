#ifndef DISPLAY_H
#define DISPLAY_H

#pragma once

#include <Arduino.h>
#include <Sensor.h>
#include <VeraMono.h>

#include <gfx_cpp14.hpp>
#include <st7789.hpp>
#include <tft_io.hpp>

#define LCD_ROTATION 0
#define LCD_WIDTH 135
#define LCD_HEIGHT 240
#define LCD_HOST VSPI
#define PIN_NUM_MISO -1
#define PIN_NUM_MOSI 19
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5
#define PIN_NUM_DC 16
#define PIN_NUM_RST 23
#define PIN_NUM_BCKL 4

#define GAUGE_X_PADDING 8
#define GAUGE_Y_PADDING 32
#define GAUGE_WIDTH LCD_WIDTH - (GAUGE_X_PADDING * 2)
#define GAUGE_HEIGHT LCD_HEIGHT - (GAUGE_Y_PADDING * 2)

using namespace arduino;
using namespace gfx;

using bus_type = tft_spi_ex<LCD_HOST, PIN_NUM_CS, PIN_NUM_MOSI, PIN_NUM_MISO,
                            PIN_NUM_CLK, SPI_MODE0>;
using lcd_type = st7789<LCD_WIDTH, LCD_HEIGHT, PIN_NUM_DC, PIN_NUM_RST,
                        PIN_NUM_BCKL, bus_type, LCD_ROTATION, true, 400, 200>;

using lcd_color = color<typename lcd_type::pixel_type>;
using bmp_type = bitmap<typename lcd_type::pixel_type>;
using bmp_color = color<typename bmp_type::pixel_type>;
using bmpa_pixel_type = rgba_pixel<HTCW_MAX_WORD>;
using bmpa_color = color<bmpa_pixel_type>;

struct Display {
  bool init();
  void draw(const uint16_t rpm, const float speed);

 private:
  lcd_type lcd;
};

#endif
