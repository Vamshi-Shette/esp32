#include <WiFi.h>
#include "ThingSpeak.h"
#include "time.h"

const char* ssid     = "vamshi";
const char* password = "11111111";

unsigned long myChannelNumber = 3316385;   
const char * myWriteAPIKey = "OK6GHM4WOW1W1HWB"; 

#define TRIG 5
#define ECHO 21

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800; // IST
const int   daylightOffset_sec = 0;

WiFiClient client;
int totalPersons = 0;
bool personInside = false;
String entryTimeStr = "";
String exitTimeStr = "";
String personLabel = "";
unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 16000; 
bool dataPending = false;

String getTimeString() 
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) 
    return "Time Error";
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
  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) 
    return 400;
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(500); 
    Serial.print("."); 
  }
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) 
  { 
    delay(500); 
    Serial.print("Syncing..."); 
  }
  Serial.println("\nWiFi Connected & Time Synced!");
  ThingSpeak.begin(client);
}

void loop() 
{
  float distance = getDistance();

  // ENTRY DETECTION
  if (distance > 1 && distance < 5 && !personInside) 
  {
    personInside = true;
    totalPersons++;
    personLabel = "Person " + String(totalPersons);
    entryTimeStr = getTimeString();
    Serial.println("\n>>> " + personLabel + " Entered at: " + entryTimeStr);
  }

  if (distance > 10 && personInside) 
  {
    personInside = false;
    exitTimeStr = getTimeString();
    Serial.println(">>> " + personLabel + " Exited at: " + exitTimeStr);
    dataPending = true; // Queue the full log
  }

  if (dataPending && (millis() - lastUploadTime >= uploadInterval)) 
  {
    ThingSpeak.setField(1, personLabel);
    ThingSpeak.setField(2, entryTimeStr);
    ThingSpeak.setField(3, exitTimeStr);

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (x == 200) {
      Serial.println(">>> ThingSpeak Update Successful.");
      dataPending = false;
      lastUploadTime = millis();
    } 
    else 
    {
      Serial.println(">>> Update Error: " + String(x));
      lastUploadTime = millis() - 11000; 
    }
  }

  delay(100);
}
