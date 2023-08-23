#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>


const char* SSID = "ssid";
const char* PASSWORD = "password";

const int REED_SWITCH_PIN = 2;
const unsigned long WIFI_CHECK_INTERVAL = 5000;

ESP8266WebServer server(80);
unsigned long lastWiFiCheckTime = 0;


void handleRoot();

void connect_to_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    }
  }
}

void setup() {
  pinMode(REED_SWITCH_PIN, INPUT);

  WiFi.begin(SSID, PASSWORD);
  connect_to_wifi();

  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  unsigned long currentTime = millis();
  if (currentTime - lastWiFiCheckTime >= WIFI_CHECK_INTERVAL) {
    lastWiFiCheckTime = currentTime;
    connect_to_wifi();
  }
}

void handleRoot() {
  bool reedSwitchValue = digitalRead(REED_SWITCH_PIN) == LOW;

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["pinState"] = reedSwitchValue;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  server.send(200, "application/json", jsonString);
}
