//ESP32 as server code

#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

void handleTemp() {
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    server.send(200, "text/plain", "Sensor Error");
    return;
  }

  server.send(200, "text/plain", String(temp));
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nServer IP: ");
  Serial.println(WiFi.localIP());

  server.on("/temp", handleTemp);
  server.begin();
}

void loop() {
  server.handleClient();
}
