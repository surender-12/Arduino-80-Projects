// LED on pin 13
// Buzzer on pin 12

int ledPin = 13;
int buzzerPin = 11;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Turn ON LED and Buzzer together
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(1000); // wait 1 second

  // Turn OFF LED and Buzzer together
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(1000); // wait 1 second
}
