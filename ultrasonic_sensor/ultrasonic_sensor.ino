
#include <NewPing.h>
#define TRIGGER_PIN  5
#define ECHO_PIN     18
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
void setup() 
{ 
  Serial.begin(115200); 
}
void loop() 
{
  
  Serial.print("Distance: ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
  delay(500);
}
