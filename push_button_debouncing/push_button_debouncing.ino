//debouncing avoided using millis()
const int buttonPin = 18;

int buttonState;           // current reading
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50 ms

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(buttonPin);

  // check if button state changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // reset timer
  }

  // check if stable for debounce time
  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if stable, update actual button state
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        Serial.println("Button Pressed");
      }
    }
  }

  lastButtonState = reading;
}

/*
debouncing using delay()
const int buttonPin = 18;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {   // button pressed
    delay(50);                           // wait for bouncing to settle

    if (digitalRead(buttonPin) == LOW) { // check again
      Serial.println("Button Pressed");

      // wait until button is released (to avoid multiple prints)
      while (digitalRead(buttonPin) == LOW);
      delay(50); // small delay after release
    }
  }
}
*/
