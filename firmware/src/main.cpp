#include <Arduino.h>
#include "Fan.h"
#include "Sensor.h"

Fan* fan = new Fan(GPIO_NUM_2, GPIO_NUM_15);
Sensor* sensor = new Sensor();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  if (!sensor->init()) {
    Serial.println("Failed to initialize sensor, cannot continue!");
    return abort();
  }
}

void loop() {
  fan->poll();

  if (!sensor->read()) {
    return;
  }

  const Bsec* rawSensor = sensor->sensor;

  Serial.printf("Temperature raw %.2f compensated %.2f\n", rawSensor->rawTemperature, rawSensor->temperature);
  Serial.printf("Humidity raw %.2f compensated %.2f\n", rawSensor->rawHumidity, rawSensor->humidity);
  Serial.printf("Pressure %.2f kPa\n", rawSensor->pressure / 1e3);
  Serial.printf("IAQ %.0f accuracy %d\n", rawSensor->iaq, rawSensor->iaqAccuracy);
  Serial.printf("Static IAQ %.0f accuracy %d\n", rawSensor->staticIaq, rawSensor->staticIaqAccuracy);
  Serial.printf("Gas resistance %.2f kOhm\n", rawSensor->gasResistance / 1e3);
  Serial.printf("VOC %.2f accuracy %d\n", rawSensor->breathVocEquivalent, rawSensor->breathVocAccuracy);
  Serial.printf("CO2 %.2f accuracy %d\n", rawSensor->co2Equivalent, rawSensor->co2Accuracy);
}
