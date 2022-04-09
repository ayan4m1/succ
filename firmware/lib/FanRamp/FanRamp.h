#include <Arduino.h>
#include <Smoothed.h>

#define FAN_LEDC_CHANNEL 0
#define FAN_TACH_PIN GPIO_NUM_2
#define FAN_PWM_PIN GPIO_NUM_15
#define FAN_PWM_RESOLUTION_BITS 6
#define FAN_PWM_FREQUENCY_HZ 2.5e4  // 25kHz is ideal
#define FAN_TACH_TIMEOUT_US 67000   // Rated lowest speed is 450rpm
#define FAN_TACH_HZ_TO_RPM 30
#define FAN_SPEED_MAX 0b111111
#define FAN_SPEED_MIN 0b000000
#define FAN_RPM_MAX 1770
#define FAN_RPM_MIN 430

struct FanRamp {
  static void test();
};
