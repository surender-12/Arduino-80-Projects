const int ledPin = 9;     // PWM pin for LED
const int buzzerPin = 8;  // Digital pin for Buzzer
const int brightness = 2; // Adjust this value (0 to 255) for dimming level
const int buzzerFrequency = 500; // Frequency in Hz for buzzer tone

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  analogWrite(ledPin, brightness);   // Set LED to a dimmed state
  tone(buzzerPin, buzzerFrequency);  // Set constant buzzer tone
  delay(100);                        // Small delay for stability
}
