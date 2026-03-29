#include "SPIFFS.h"

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Write
  File file = SPIFFS.open("/data.txt", FILE_WRITE);
  file.println("ESP32 File System");
  file.close();

  // Read
  file = SPIFFS.open("/data.txt");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void loop() {}