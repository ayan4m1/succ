#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <Smoothed.h>

#define FAN_LEDC_CHANNEL 0
#define FAN_TACH_PIN GPIO_NUM_2
#define FAN_PWM_PIN GPIO_NUM_15
#define FAN_PWM_RESOLUTION_BITS 4
#define FAN_PWM_FREQUENCY_HZ 2.5e4  // 25kHz is ideal
#define FAN_TACH_TIMEOUT_US 67000   // Rated lowest speed is 450rpm
#define FAN_TACH_HZ_TO_RPM 30
#define FAN_SPEED_MAX 0b1111
#define FAN_SPEED_MIN 0b0000
#define FAN_RPM_MAX 2500
#define FAN_RPM_MIN 450

// derived experimentally from my Noctua fan
#define FAN_RPM_TO_DUTY_CYCLE(rpm) (rpm + 7.20199) / 149.34926

#define FAN_SPEED_PCT(x) x > 1 : FAN_SPEED_MAX : floor((x * FAN_SPEED_MAX))

class Fan {
  Smoothed<uint16_t> currentRpm;
  double targetRpm = (FAN_RPM_MAX - FAN_RPM_MIN) / 2.0;

 public:
  void init();
  void poll();
  uint16_t getCurrentRpm();
  uint16_t getTargetRpm();
  void setTargetRpm(const uint16_t targetRpm);
};
