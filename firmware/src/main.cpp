#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#if defined(ESP32)
#include <WiFi.h>
#endif

#include <Button2.h>
#include <FastLED.h>

#include "Display.h"
#include "Fan.h"
// #include "FanRamp.h"
// #include "PWMFan.hpp"
#include "Sensor.h"

// Uncomment this to enable serial debugging
#define DEBUG

xTaskHandle fanTask;
Display* display = new Display();
// PWMFan* fan = new PWMFan(GPIO_NUM_2, GPIO_NUM_15, 400);
Fan* fan = new Fan();
// Sensor *sensor = new Sensor();

Button2 upButton = Button2();
Button2 downButton = Button2();

void upClick(Button2& btn) {
  fan->setTargetRpm(fan->getTargetRpm() + (FAN_RPM_MAX / 10.0));
}

void downClick(Button2& btn) {
  fan->setTargetRpm(fan->getTargetRpm() - (FAN_RPM_MAX / 10.0));
}

void longClick(Button2& btn) {
  vTaskSuspend(fanTask);
  // fan->tune();
  vTaskResume(fanTask);
}

void fanLoop(void* params) {
  for (;;) {
    // fan->update();
    fan->poll();
  }
}

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
#endif

  WiFi.mode(WIFI_OFF);

  display->init();

  fan->init();

  // fan->setPIDs(0.11, 0.01, 0.6);
  // fan->setTargetRpm(1500);
  // fan->begin();

  //   if (!sensor->init()) {
  // #ifdef DEBUG
  //     Serial.println("Failed to initialize sensor, cannot continue!");
  // #endif
  //     return abort();
  //   }

  upButton.setTapHandler(upClick);
  downButton.setTapHandler(downClick);
  upButton.setLongClickTime(5000);
  upButton.setLongClickHandler(longClick);

  upButton.begin(GPIO_NUM_35);
  downButton.begin(GPIO_NUM_0);

  // FanRamp::test();
  xTaskCreatePinnedToCore(fanLoop, "FAN", 20000, nullptr, 1, &fanTask, 1);
}

void loop() {
  // fan->poll();
  // fan->update();

  upButton.loop();
  downButton.loop();

  //   if (!sensor->read()) {
  //     return;
  //   }

  //   const Bsec *rawSensor = sensor->sensor;

  // #ifdef DEBUG
  //   Serial.printf("Temperature raw %.2f compensated %.2f\n",
  //                 rawSensor->rawTemperature, rawSensor->temperature);
  //   Serial.printf("Humidity raw %.2f compensated %.2f\n",
  //   rawSensor->rawHumidity,
  //                 rawSensor->humidity);
  //   Serial.printf("Pressure %.2f kPa\n", rawSensor->pressure / 1e3);
  //   Serial.printf("IAQ %.0f accuracy %d\n", rawSensor->iaq,
  //                 rawSensor->iaqAccuracy);
  //   Serial.printf("Static IAQ %.0f accuracy %d\n", rawSensor->staticIaq,
  //                 rawSensor->staticIaqAccuracy);
  //   Serial.printf("Gas resistance %.2f kOhm\n", rawSensor->gasResistance /
  //   1e3); Serial.printf("VOC %.2f accuracy %d\n",
  //   rawSensor->breathVocEquivalent,
  //                 rawSensor->breathVocAccuracy);
  //   Serial.printf("CO2 %.2f accuracy %d\n", rawSensor->co2Equivalent,
  //                 rawSensor->co2Accuracy);
  // #endif

  // display->draw(SensorReadings(rawSensor), fan->getSpeed());
  // display->draw(SensorReadings(), fan->getCurrentRpm());
  EVERY_N_MILLISECONDS(50) {
    display->draw(fan->getCurrentRpm(), fan->getCurrentRpm() / FAN_RPM_MAX);
  }
}
