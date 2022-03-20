#pragma once

#include <Arduino.h>
#include <analogWrite.h>

#define FAN_TACH_PIN GPIO_NUM_2
#define FAN_PWM_PIN GPIO_NUM_15
#define FAN_PWM_RESOLUTION_BITS 4
#define FAN_PWM_FREQUENCY_HZ 2.5e4
#define FAN_TACH_TIMEOUT_US 67000
#define FAN_TACH_HZ_TO_RPM 30
#define FAN_SPEED_MAX 0b1111
#define FAN_SPEED_MIN 0b0000

struct Fan {
  uint16_t setRpm = 0;
  uint16_t rpm = 0;
  uint8_t dutyCycle = 127;

  Fan();

  void init();
  bool poll();
  void setSpeed(uint16_t rpm);
  uint16_t getSpeed();
};
