//ESP32 as client code

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// Replace with SERVER ESP32 IP
String serverName = "http://192.168.1.100/temp";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(serverName);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Temperature: " + payload);
    } else {
      Serial.println("Error fetching data");
    }

    http.end();
  }

  delay(3000);
}