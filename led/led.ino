#define led_pin 9

void setup() {
  pinMode(led_pin, OUTPUT);   // Set pin 8 as an output
  digitalWrite(led_pin, HIGH); // Turn the LED on
}

void loop() {
  // No code needed here, LED stays on
}
