// Define pins for the LEDs
const int led1 = 2;  // LED 1 connected to pin 2
const int led2 = 3;  // LED 2 connected to pin 3
const int led3 = 4;  // LED 3 connected to pin 4
const int led4 = 5;  // LED 4 connected to pin 5

void setup() {
  // Set the LED pins as OUTPUT
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Ensure all LEDs are off initially
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
}

void loop() {
  // Sequentially turn on LEDs with a 2-second delay
  digitalWrite(led1, HIGH); // Turn on LED 1
  delay(2000);              // Wait for 2 seconds

  digitalWrite(led2, HIGH); // Turn on LED 2
  delay(2000);              // Wait for 2 seconds

  digitalWrite(led3, HIGH); // Turn on LED 3
  delay(2000);              // Wait for 2 seconds

  digitalWrite(led4, HIGH); // Turn on LED 4
  delay(2000);              // Wait for 2 seconds

  // All LEDs are now on; turn them off for 2 seconds
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(2000);              // Wait for 2 seconds

  // Turn all LEDs back on for 2 seconds
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  delay(2000);              // Wait for 2 seconds

  // Repeat the sequence
}
