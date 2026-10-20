const int buttonPin = 2;  // Pin connected to the push button
const int buzzerPin = 8;  // Pin connected to the buzzer
int buttonState = 0;      // Variable to store button state

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Enable internal pull-up resistor
  pinMode(buzzerPin, OUTPUT);        // Set buzzer pin as output
}

void loop() {
  // Read the state of the push button
  buttonState = digitalRead(buttonPin);

  // Check if the button is pressed (LOW because of pull-up resistor)
  if (buttonState == LOW) {
    // Turn on the buzzer
    digitalWrite(buzzerPin, HIGH);
  } else {
    // Turn off the buzzer
    digitalWrite(buzzerPin, LOW);
  }
}
