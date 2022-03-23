#include "PWMFan.hpp"

PWMFan::PWMFan(const uint8_t tach_pin, const uint8_t pwm_pin,
               const uint16_t min_rpm, const uint8_t hz_to_rpm) {
  this->tach_pin = tach_pin;
  this->pwm_pin = pwm_pin;
  this->min_rpm = min_rpm;
  this->hz_to_rpm = hz_to_rpm;
  this->tach_timeout = max(10000.0, (1 / (double)(min_rpm / hz_to_rpm)) * 1e6);
}

void PWMFan::begin() {
  pinMode(tach_pin, INPUT);
  pinMode(pwm_pin, OUTPUT);

  ledcSetup(FAN_LEDC_CHANNEL, FAN_PWM_FREQUENCY_HZ, FAN_PWM_RESOLUTION_BITS);
  ledcAttachPin(pwm_pin, FAN_LEDC_CHANNEL);
}

void PWMFan::end() { ledcDetachPin(pwm_pin); }

void PWMFan::update() {
  uint32_t rising_pulse_us = pulseIn(tach_pin, LOW, tach_timeout);
  uint32_t falling_pulse_us = pulseIn(tach_pin, HIGH, tach_timeout);
  uint16_t pulse_ms = (rising_pulse_us + falling_pulse_us) / 1e3;

  Serial.printf("Total pulse was %dms\n", pulse_ms);

  if (pulse_ms > 0) {
    current_rpm = (1e3 / pulse_ms) * hz_to_rpm;
  } else {
    current_rpm = 0;
  }

  Serial.printf("Current RPM %d\n", current_rpm);

  if (pids.Kp == 0) {
    return;
  }

  epid_pid_calc(&pid, target_rpm, current_rpm);
  epid_pid_sum(&pid, FAN_SPEED_MIN, FAN_SPEED_MAX);

  Serial.printf("PID Output %d\n", lroundf(pid.y_out));

  ledcWrite(FAN_LEDC_CHANNEL, (uint32_t)lroundf(pid.y_out));
}

void PWMFan::setTargetRpm(const uint16_t target_rpm) {
  this->target_rpm = min(max(target_rpm, (uint16_t)0), (uint16_t)3000);
}

bool PWMFan::setPIDs(const float p, const float i, const float d) {
  this->pids.Kp = p;
  this->pids.Ki = i;
  this->pids.Kd = d;

  auto epid_err = epid_init(&pid, current_rpm, current_rpm, FAN_SPEED_MIN,
                            pids.Kp, pids.Ki, pids.Kd);

  return epid_err == EPID_ERR_NONE;
}

uint16_t PWMFan::getCurrentRpm() { return current_rpm; }

uint16_t PWMFan::getTargetRpm() { return target_rpm; }
