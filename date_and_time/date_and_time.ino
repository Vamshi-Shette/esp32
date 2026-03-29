//Real time data of date and time

#include <WiFi.h>
#include "time.h"

const char* ssid     = "vamshi";
const char* password = "11111111";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;   // IST = +5:30 → 5*3600 + 1800
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  // Init time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  Serial.print("Date & Time: ");
  Serial.println(&timeinfo, "%d-%m-%Y %H:%M:%S");

  delay(1000); // update every second
}