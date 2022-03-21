#include "Fan.h"

void Fan::init() {
  pinMode(FAN_TACH_PIN, INPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);

  ledcSetup(FAN_LEDC_CHANNEL, FAN_PWM_FREQUENCY_HZ, FAN_PWM_RESOLUTION_BITS);
  ledcAttachPin(FAN_PWM_PIN, FAN_LEDC_CHANNEL);

  currentRpm.begin(SMOOTHED_AVERAGE, 8);
  setTargetRpm(targetRpm);
}

uint16_t Fan::getTargetRpm() { return targetRpm; }

void Fan::setTargetRpm(const uint16_t targetRpm) {
  this->targetRpm = min(max((int)targetRpm, FAN_RPM_MIN), FAN_RPM_MAX);
  ledcWrite(FAN_LEDC_CHANNEL, FAN_RPM_TO_DUTY_CYCLE(this->targetRpm));
}

void Fan::poll() {
  uint32_t risingPulseUs = pulseIn(FAN_TACH_PIN, LOW, FAN_TACH_TIMEOUT_US);
  uint32_t fallingPulseUs = pulseIn(FAN_TACH_PIN, HIGH, FAN_TACH_TIMEOUT_US);
  uint16_t totalPulseMs = (risingPulseUs + fallingPulseUs) / 1e3;

  if (totalPulseMs <= 0) {
    currentRpm.clear();
    currentRpm.add(0);
  } else {
    double speedHz = totalPulseMs > 0 ? (1e3 / totalPulseMs) : 0;
    currentRpm.add(speedHz * FAN_TACH_HZ_TO_RPM);
  }
}

uint16_t Fan::getCurrentRpm() { return currentRpm.get(); }
