#include "Fan.h"

Fan::Fan(uint8_t tachPin, uint8_t pwmPin) {
  this->tachPin = tachPin;
  this->pwmPin = pwmPin;

  pinMode(this->tachPin, INPUT);
  pinMode(this->pwmPin, OUTPUT);

  analogWriteFrequency(this->pwmPin, FAN_PWM_FREQUENCY_HZ);
  analogWriteResolution(FAN_PWM_RESOLUTION_BITS);
}

void Fan::poll() {
  uint32_t fallingPulseUs = pulseIn(this->tachPin, HIGH, FAN_TACH_TIMEOUT_US);
  uint32_t risingPulseUs = pulseIn(this->tachPin, LOW, FAN_TACH_TIMEOUT_US);
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
