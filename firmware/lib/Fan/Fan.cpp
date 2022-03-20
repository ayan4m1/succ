#include "Fan.h"

Fan::Fan() { setRpm = 0; }

void Fan::init() {
  pinMode(FAN_TACH_PIN, INPUT_PULLUP);

  analogWriteFrequency(FAN_PWM_PIN, FAN_PWM_FREQUENCY_HZ);
  analogWriteResolution(FAN_PWM_PIN, FAN_PWM_RESOLUTION_BITS);
  pinMode(FAN_PWM_PIN, OUTPUT_OPEN_DRAIN);
  analogWrite(FAN_PWM_PIN, FAN_SPEED_MAX, FAN_SPEED_MAX);
}

bool Fan::poll() {
  uint32_t risingPulseUs = pulseIn(FAN_TACH_PIN, HIGH, FAN_TACH_TIMEOUT_US);
  uint32_t fallingPulseUs = pulseIn(FAN_TACH_PIN, HIGH, FAN_TACH_TIMEOUT_US);
  uint16_t totalPulseMs = (risingPulseUs + fallingPulseUs) / 1e3;

  if (totalPulseMs <= 0) {
    rpm = 0;
    return false;
  }

  double speedHz = totalPulseMs > 0 ? (1e3 / totalPulseMs) : 0;
  rpm = speedHz * FAN_TACH_HZ_TO_RPM;

  Serial.printf("Tach pin giving %f Hz, %d RPM\n", speedHz, rpm);

  double speedDelta = abs(rpm - setRpm);

  if (speedDelta > 0) {
    dutyCycle += 1;
  } else {
    dutyCycle -= 1;
  }

  Serial.printf("Setting duty cycle to %d\n", dutyCycle);
  analogWrite(FAN_PWM_PIN, dutyCycle, FAN_SPEED_MAX);

  return true;
}

void Fan::setSpeed(uint16_t rpm) {
  // todo: figure out rpm difference and adjust PWM
  this->setRpm = rpm;
}

uint16_t Fan::getSpeed() {
  // todo: get this live from a pulseIn count
  return this->rpm;
}
