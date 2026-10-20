// Define LED pins
const int led1 = 8; // LED 1
const int led2 = 9; // LED 2
const int led3 = 10; // LED 3

// Define LDR pin
const int ldrPin = A0; // LDR connected to analog pin A0

// Threshold value for light level
const int lightThreshold = 500; // Adjust based on your LDR readings

void setup() {
  // Set LED pins as output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Initialize Serial Monitor (optional for debugging)
  Serial.begin(9600);
}

void loop() {
  // Read the value from the LDR
  int ldrValue = analogRead(ldrPin);

  // Print LDR value to the Serial Monitor (optional for testing)
  Serial.println(ldrValue);

  // Check if the light level is below the threshold (dark condition)
  if (ldrValue > lightThreshold) {
    // Turn ON all LEDs (simulate street lights)
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  } else {
    // Turn OFF all LEDs
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }

  // Small delay to stabilize the readings
  delay(100);
}
