#include "Sensor.h"

bsec_virtual_sensor_t sensorFields[] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
};

SensorReadings::SensorReadings() {}

SensorReadings::SensorReadings(const Bsec* sensor) {
  this->breathVocEquivalent = sensor->breathVocEquivalent;
  this->co2Equivalent = sensor->co2Equivalent;
  this->compGasValue = sensor->compGasValue;
  this->gasPercentage = sensor->gasPercentage;
  this->gasResistance = sensor->gasResistance;
  this->humidity = sensor->humidity;
  this->iaq = sensor->iaq;
  this->pressure = sensor->pressure;
  this->staticIaq = sensor->staticIaq;
  this->temperature = sensor->temperature;
}

Sensor::Sensor() { sensor = new Bsec(); };

Sensor::~Sensor() { delete sensor; };

bool Sensor::init() {
  sensor = new Bsec();
  Wire.begin();
  sensor->begin(BME680_I2C_ADDR_SECONDARY, Wire);

  if (!check()) {
    Serial.println("Error connecting to BME680!");
    return false;
  }

  sensor->setConfig(iaqConfig);

  if (!check()) {
    Serial.println("Failed to set up IAQ config for BME680!");
    return false;
  }

  sensor->updateSubscription(sensorFields,
                             sizeof(sensorFields) / sizeof(sensorFields[0]),
                             BSEC_SAMPLE_RATE_LP);

  if (!check()) {
    Serial.println("Failed to set up subscription for BME680!");
    return false;
  }

  return true;
}

bool Sensor::check() {
  if (sensor->status < BSEC_OK) {
    Serial.println(sensor->status);
    return false;
  } else if (sensor->status > BSEC_OK) {
    Serial.printf("Status warning %d\n", sensor->status);
  }

  if (sensor->bme680Status < BME680_OK) {
    Serial.println(sensor->bme680Status);
    return false;
  } else if (sensor->bme680Status > BME680_OK) {
    Serial.printf("Sensor warning %d\n", sensor->bme680Status);
  }

  return true;
}

bool Sensor::read() { return sensor->run(); }
