#include <Arduino.h>
#include <analogWrite.h>

#define FAN_PWM_RESOLUTION_BITS 4
#define FAN_PWM_FREQUENCY_HZ 2.5e4
#define FAN_TACH_TIMEOUT_US 2000
#define FAN_TACH_HZ_TO_RPM 30
#define FAN_SPEED_MAX 0b1111
#define FAN_SPEED_MIN 0b0000

struct Fan {
  uint8_t tachPin;
  uint8_t pwmPin;
  uint16_t setRpm;

  Fan(uint8_t tachPin, uint8_t pwmPin);

  void poll();
  void setSpeed(uint16_t rpm);
  uint16_t getSpeed();
};
