// Define LED pins
const int redLED = 8;    // Red LED connected to pin 8
const int greenLED = 9;  // Green LED connected to pin 9
const int orangeLED = 10; // Orange LED connected to pin 10

// Define delay times in milliseconds
const int redDelay = 2000;    // 2 seconds for red light
const int greenDelay = 2000;  // 2 seconds for green light
const int orangeDelay = 2000; // 2 seconds for orange light

void setup() {
  // Set LED pins as output
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
}

void loop() {
  // Red light ON, others OFF
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  digitalWrite(orangeLED, LOW);
  delay(redDelay);

  // Green light ON, others OFF
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  digitalWrite(orangeLED, LOW);
  delay(greenDelay);

  // Orange light ON, others OFF
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(orangeLED, HIGH);
  delay(orangeDelay);
}
