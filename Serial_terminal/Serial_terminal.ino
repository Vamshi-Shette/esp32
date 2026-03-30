int LED=2;
void setup() 
{
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  Serial.begin(115200);

}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    String cmd=Serial.readString();
    cmd.trim();
    if(cmd=="1")
    {
      digitalWrite(LED,HIGH);
      Serial.println("LED ON");
    }
    else
    {
      digitalWrite(LED,LOW);
      Serial.println("LED OFF");
    }
  }
}