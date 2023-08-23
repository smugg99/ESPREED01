#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Funbox6-420D";
const char* password = "Wysowianka1";

ESP8266WebServer server(80);

const int reedSwitchPin = 2;

unsigned long lastWiFiCheckTime = 0;
const unsigned long WiFiCheckInterval = 5000;

void handleRoot();

void connect_to_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    }
  }
}

void setup() {
  pinMode(reedSwitchPin, INPUT);

  WiFi.begin(ssid, password);
  connect_to_wifi();

  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  unsigned long currentTime = millis();
  if (currentTime - lastWiFiCheckTime >= WiFiCheckInterval) {
    lastWiFiCheckTime = currentTime;
    connect_to_wifi();
  }
}

void handleRoot() {
  bool reedSwitchValue = digitalRead(reedSwitchPin) == LOW;

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["pinState"] = reedSwitchValue;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  server.send(200, "application/json", jsonString);
}
