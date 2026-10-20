int ledPin = 9; // Connect LED to PWM pin 9
int brightness = 1; // Set brightness (0-255)

void setup() {
  pinMode(ledPin, OUTPUT); // Set LED pin as output
}

void loop() {
  analogWrite(ledPin, brightness); // Set constant dim brightness
}
