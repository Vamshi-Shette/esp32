#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

// -------- WIFI --------
const char* ssid = "vamshi";
const char* password = "11111111";

// -------- THINGSPEAK --------
String apiKey = "YOUR_API_KEY";   // replace
String server = "http://api.thingspeak.com/update";

// -------- ULTRASONIC --------
#define TRIG 5
#define ECHO 18

bool personInside = false;
int personCount = 0;

long duration;
float distance;

// -------- NTP --------
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;

// ================= TIME =================
String getTimeString() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) return "Error";

  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

  return String(buffer);
}

// ================= DISTANCE =================
float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;
}

// ================= SEND TO CLOUD =================
void sendToCloud(String person, String entry, String exitTime) {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = server + "?api_key=" + apiKey +
                 "&field1=" + person +
                 "&field2=" + entry +
                 "&field3=" + exitTime;

    http.begin(url);
    int httpCode = http.GET();

    Serial.print("HTTP Response: ");
    Serial.println(httpCode);

    http.end();
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  // NTP
  configTime(gmtOffset_sec, 0, ntpServer);
  delay(2000);
}

// ================= LOOP =================
void loop() {

  float distance = getDistance();

  static String entryTime = "";

  // ENTRY
  if (distance < 5 && !personInside) {
    personInside = true;
    entryTime = getTimeString();

    Serial.println("Entry: " + entryTime);
  }

  // EXIT
  if (distance > 10 && personInside) {
    personInside = false;

    String exitTime = getTimeString();
    personCount++;

    Serial.println("Exit: " + exitTime);

    // SEND TO CLOUD
    sendToCloud(
      "Person" + String(personCount),
      entryTime,
      exitTime
    );
  }

  delay(200);
}