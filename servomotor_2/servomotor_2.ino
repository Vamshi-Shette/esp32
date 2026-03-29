//interfacing servo using ledc
onst int servoPin = 23;
const int channel = 0;

void setup() {
  ledcSetup(channel, 50, 16);  // 50 Hz, 16-bit resolution
  ledcAttachPin(servoPin, channel);
}

void loop() {
  // 0 degrees
  ledcWrite(channel, 1638);  
  delay(2000);

  // 90 degrees
  ledcWrite(channel, 4915);  
  delay(2000);

  // 180 degrees
  ledcWrite(channel, 8192);  
  delay(2000);
}

/*
#include <ESP32Servo.h>

Servo myServo;

void setup() {
  myServo.attach(23);
}

void loop() {
  myServo.write(0);
  delay(2000);

  myServo.write(90);
  delay(2000);

  myServo.write(180);
  delay(2000);
}
*/