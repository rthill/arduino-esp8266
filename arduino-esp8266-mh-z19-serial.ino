#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "settings.h"
#include "WiFiUtils.h"
#include "MHZ19.h"

long previousMillis = 0;
long previousMillisSend = 0;
int errorCount = 0;

WiFiClient client;
PubSubClient mqttClient(client);
MHZ19 co2Sensor(MH_Z19_RX, MH_Z19_TX);

void (*resetFunc)(void) = 0; //declare reset function @ address 0

void setup() {
  Serial.begin(115200); // Init console
  Serial.println("Setup started");

  unsigned long previousMillis = millis();
  co2Sensor.start(); //Init sensor MH-Z19(14)

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
  }
  
  WiFi.mode(WIFI_STA); //be only wifi client, not station
  //Get Current Hostname
  Serial.printf("Default hostname: %s\n", WiFi.hostname());

  //Set new hostname
  WiFi.hostname(WIFI_HOSTNAME);
  Serial.printf("New hostname: %s\n", WiFi.hostname());

  WiFiUtils::printNetworks();
  
  // attempt to connect to Wifi network:
  unsigned int attempt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (WIFI_MAX_ATTEMPTS_INIT != 0 && attempt > WIFI_MAX_ATTEMPTS_INIT)
      break;

    if (attempt >= 65535)
      attempt = 0;

    attempt++;

    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);
    
    // Connect to WPA/WPA2 network:
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(10000);
  }

  WiFiUtils::printWiFiStatus();
  WiFiUtils::printCurrentNet();
  WiFiUtils::printWifiData();

  if (MQTT_ENABLED) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  }
  Serial.println("Waiting for sensors to init");

  while (millis() - previousMillis < 10000)
    delay(1000);
  Serial.println("Setup finished");
  Serial.println("");
}

void loop() {
  unsigned long currentMillis = millis();
  if (CHECK_INTERVAL > (currentMillis - previousMillis)) {
    delay(100);
    return;
  }

  previousMillis = currentMillis;
  Serial.println("loop started");

  if (errorCount > MAX_DATA_ERRORS) {
    Serial.println("Too many errors, resetting");
    delay(2000);
    resetFunc();
  }

  Serial.println("reading data:");
  int ppm = co2Sensor.read();
  bool dataError = false;
  Serial.println("  PPM = " + String(ppm));

  if (ppm < 100 || ppm > 6000) {
    Serial.println("PPM not valid");
    dataError = true;
  }

  int mem = ESP.getFreeHeap();
  Serial.println("  Free RAM: " + String(mem));

  errorCount = 0;

  unsigned long currentMillisSend = millis();
  if (WIFI_SEND_INTERVAL > (currentMillisSend - previousMillisSend)) {
    Serial.println("Not sending to server - too early");
    return;
  }

  previousMillisSend = currentMillisSend;

  WiFiUtils::checkReconnect(WIFI_SSID, WIFI_PASS, WIFI_MAX_ATTEMPTS_SEND);
  WiFiUtils::printCurrentNet();

  bool sentOk = false;

  if (MQTT_ENABLED) {
    bool connected = true;
    if (!mqttClient.connected()) {
      connected = false;
      if (mqttClient.connect("ESP8266Client")) {
        Serial.println("MQTT reconnected");
        connected = true;
      } else {
        connected = false;
        Serial.print("MQTT failed, rc=");
        Serial.print(mqttClient.state());
      }
    }

    if (connected) {
      char dataString[5];       // number of digits + 1 for null terminator
      itoa(ppm, dataString, 10);  // int value, pointer to string, base number
      mqttClient.publish(CO2_TOPIC, dataString, true);
      itoa(mem, dataString, 10);  // int value, pointer to string, base number
      mqttClient.publish(RAM_TOPIC, dataString, true);
      Serial.println("MQTT request sent");
      sentOk = true;
    } else {
      Serial.println("MQTT not connected");
    }
  }
  if (!sentOk)
  Serial.println("WiFi connection error");
  Serial.println("loop finished");
  Serial.println("");
}
