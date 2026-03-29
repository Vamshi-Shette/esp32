#include <ESP32Servo.h>
const int servoPin = 14; // GPIO pin connected to the servo signal
Servo servo1;
void setup() {
   Serial.begin(115200);
   delay(1000);
   // Allocate a PWM timer for the servo
   ESP32PWM::allocateTimer(0);
   // Set the PWM frequency to 50Hz (standard for servos)
   servo1.setPeriodHertz(50);
   // Attach the servo to the specified pin with min/max pulse widths
   servo1.attach(servoPin, 1000, 2000); // Min: 1000µs, Max: 2000µs
   // Set initial position to 0 degrees
   servo1.write(0);
}
void loop() {
   // Move the servo to 0 degrees
   servo1.writeMicroseconds(1000);
   delay(3000);
   // Move the servo to 180 degrees
   servo1.writeMicroseconds(2000);
   delay(3000);
}