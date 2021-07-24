#include <Arduino.h>
#include <WiFi.h>

#include "Display.h"
#include "Fan.h"
#include "Sensor.h"

// Uncomment this to enable serial debugging
#define DEBUG

Display* display = new Display();
Fan* fan = new Fan();
Sensor* sensor = new Sensor();

void disableRadios() {
  WiFi.mode(WIFI_OFF);
  btStop();
}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
#endif

  disableRadios();

  display->init();

  fan->init();

  if (!sensor->init()) {
#ifdef DEBUG
    Serial.println("Failed to initialize sensor, cannot continue!");
#endif
    return abort();
  }
}

void loop() {
  fan->poll();

  if (!sensor->read()) {
    return;
  }

  const Bsec* rawSensor = sensor->sensor;

#ifdef DEBUG
  Serial.printf("Temperature raw %.2f compensated %.2f\n", rawSensor->rawTemperature, rawSensor->temperature);
  Serial.printf("Humidity raw %.2f compensated %.2f\n", rawSensor->rawHumidity, rawSensor->humidity);
  Serial.printf("Pressure %.2f kPa\n", rawSensor->pressure / 1e3);
  Serial.printf("IAQ %.0f accuracy %d\n", rawSensor->iaq, rawSensor->iaqAccuracy);
  Serial.printf("Static IAQ %.0f accuracy %d\n", rawSensor->staticIaq, rawSensor->staticIaqAccuracy);
  Serial.printf("Gas resistance %.2f kOhm\n", rawSensor->gasResistance / 1e3);
  Serial.printf("VOC %.2f accuracy %d\n", rawSensor->breathVocEquivalent, rawSensor->breathVocAccuracy);
  Serial.printf("CO2 %.2f accuracy %d\n", rawSensor->co2Equivalent, rawSensor->co2Accuracy);
#endif

  display->draw(rawSensor->breathVocEquivalent, fan->getSpeed());
}
