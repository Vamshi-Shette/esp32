//this one
#include <BluetoothSerial.h>
#include <WiFi.h>
#include "time.h"

BluetoothSerial SerialBT;

#define TRIG 5
#define ECHO 18

const char* ssid = "vamshi";
const char* password = "11111111";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;

struct tm timeinfo;

bool personInside = false;
int personCount = 0;
long duration;

String getTimeString() 
{
  if (!getLocalTime(&timeinfo, 2000)) 
    return "NoTime";
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

float getDistance() 
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) 
    return 999;
  return duration * 0.034 / 2;
}

void setup() 
{
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  SerialBT.begin("ESP32_Logger");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }

  configTime(gmtOffset_sec, 0, ntpServer);
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 20) 
  {
    delay(500);
    retry++;
  }
}

void loop() 
{
  float distance = getDistance();
  static String entryTime = "";
  static unsigned long lastTrigger = 0;
  if (distance == 999) 
    return;
  if (SerialBT.hasClient()) 
  {

    // ENTRY (distance < 5 cm)
    if (distance < 5 && !personInside && millis() - lastTrigger > 2000) 
    {
      personInside = true;
      lastTrigger = millis();
      entryTime = getTimeString();
    }

    // EXIT (distance > 10 cm)
    if (distance > 10 && personInside && millis() - lastTrigger > 2000) 
    {
      personInside = false;
      lastTrigger = millis();
      String exitTime = getTimeString();
      personCount++;
      String log = "Person" + String(personCount) + "|" + entryTime + "|" + exitTime;
      // Send to Serial + Mobile
      Serial.println(log);
      SerialBT.println(log);
    }
  }
  delay(200);
}
