#include "Fan.h"

Fan::Fan() {
  setRpm = 0;
}

void Fan::init() {
  pinMode(FAN_TACH_PIN, INPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);

  analogWriteFrequency(FAN_PWM_PIN, FAN_PWM_FREQUENCY_HZ);
  analogWriteResolution(FAN_PWM_RESOLUTION_BITS);
}

void Fan::poll() {
  uint32_t fallingPulseUs = pulseIn(FAN_TACH_PIN, HIGH, FAN_TACH_TIMEOUT_US);
  uint32_t risingPulseUs = pulseIn(FAN_TACH_PIN, LOW, FAN_TACH_TIMEOUT_US);
  uint16_t totalPulseLengthMs = (fallingPulseUs + risingPulseUs) / 1e3;

  double frameRateHz = (1000 / totalPulseLengthMs);
  double frameRateRpm = frameRateHz * FAN_TACH_HZ_TO_RPM;
  Serial.printf("Tach pin giving %f Hz, %f RPM\n", frameRateHz, frameRateRpm);
}

void Fan::setSpeed(uint16_t rpm) {
  // todo: figure out rpm difference and adjust PWM
  this->setRpm = rpm;
}

uint16_t Fan::getSpeed() {
  // todo: get this live from a pulseIn count
  return this->setRpm;
}
