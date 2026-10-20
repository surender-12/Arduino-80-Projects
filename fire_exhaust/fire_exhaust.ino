// Define pin connections
const int flameSensorPin = A0;  // Flame sensor connected to analog pin A0
const int relayPin = 7;         // Relay connected to digital pin 7
const int flameThreshold = 500; // Threshold value to detect fire (adjust based on your sensor readings)

void setup() {
  // Initialize the relay pin as output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure the water pump is OFF initially
  
  // Initialize Serial Monitor for debugging (optional)
  Serial.begin(9600);
}

void loop() {
  // Read the value from the flame sensor
  int flameValue = analogRead(flameSensorPin);
  
  // Print the flame sensor value to the Serial Monitor for testing
  Serial.println(flameValue);
  
  // Check if the flame sensor detects fire
  if (flameValue > flameThreshold) {
    // If fire is detected, activate the relay (turn ON the water pump)
    digitalWrite(relayPin, HIGH);
    Serial.println("Fire detected! Water pump activated.");
  } else {
    // If no fire is detected, deactivate the relay (turn OFF the water pump)
    digitalWrite(relayPin, LOW);
    Serial.println("No fire detected. Water pump deactivated.");
  }

  // Small delay for stability
  delay(100);
}
