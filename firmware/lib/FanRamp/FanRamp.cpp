#include "FanRamp.h"

uint16_t getCurrentRpm() {
  uint32_t totalPulseMs = (pulseIn(FAN_TACH_PIN, HIGH, FAN_TACH_TIMEOUT_US) +
                           pulseIn(FAN_TACH_PIN, HIGH, FAN_TACH_TIMEOUT_US)) /
                          1e3;

  if (totalPulseMs == 0) {
    return 0;
  }

  return (1e3 / totalPulseMs) * FAN_TACH_HZ_TO_RPM;
}

void FanRamp::test() {
  pinMode(FAN_TACH_PIN, INPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);

  ledcSetup(FAN_LEDC_CHANNEL, FAN_PWM_FREQUENCY_HZ, FAN_PWM_RESOLUTION_BITS);
  ledcAttachPin(FAN_PWM_PIN, FAN_LEDC_CHANNEL);

  Serial.println(F("Duty,RPM"));
  for (uint8_t i = 0; i < FAN_SPEED_MAX; i++) {
    ledcWrite(FAN_LEDC_CHANNEL, i);
    delay(250);
    Smoothed<uint16_t> averageRpm;
    averageRpm.begin(SMOOTHED_AVERAGE, 10);
    for (uint8_t j = 0; j < 10; j++) {
      delay(100);
      averageRpm.add(getCurrentRpm());
    }
    Serial.printf("%d,%d\n", i, averageRpm.get());
  }

  ledcDetachPin(FAN_PWM_PIN);
  delay(5000);
}
