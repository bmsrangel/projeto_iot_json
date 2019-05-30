#include <Arduino.h>
#include "config/config.h"
#include "json/readSendJson.h"

unsigned long readTime;

void setup() {
  Serial.begin(9600);
  readTime = 0;
  delay(10);

  if (connectWiFi()) {
    startMQTT();
    startGPIO();
    initSensorDHT11();
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WIFI, LOW);
    if (!MQTTClient.connected()) {
      connectClientMQTT();
    }

    if (millis() > readTime + 6000) {
      readTime = millis();
      publishTopics();
    }

    MQTTClient.loop();
  } else {
    if (connectWiFi()) {
      startMQTT();
    }
  }
}