#pragma once

#include <bsec.h>

const uint8_t iaqConfig[] = {
#include "config/generic_33v_3s_28d/bsec_iaq.txt"
};

extern bsec_virtual_sensor_t sensorFields[];

struct SensorReadings {
  SensorReadings();
  SensorReadings(const Bsec* sensor);

  float iaq, pressure, gasResistance, temperature, humidity, staticIaq,
      co2Equivalent, breathVocEquivalent, compGasValue, gasPercentage;
};

struct Sensor {
  Bsec* sensor;

  Sensor();
  ~Sensor();

  bool init();
  bool check();
  bool read();
};
