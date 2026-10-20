// Define LED pins
const int led1 = 2;  
const int led2 = 3;  
const int led3 = 4;  
const int led4 = 5;  
const int led5 = 6;  
const int led6 = 7;  
const int led7 = 8;  
const int led8 = 9;  
const int led9 = 10;  
const int led10 = 11;  
const int led11 = 12;  
const int led12 = 13;  


// Define LDR pin
const int ldrPin = A0; // LDR connected to analog pin A0

// Threshold value for light level
const int lightThreshold = 500; // Adjust based on your LDR readings

void setup() {
  // Set LED pins as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);
  pinMode(led11, OUTPUT);
  pinMode(led12, OUTPUT);

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
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led9, HIGH);
    digitalWrite(led10, HIGH);
    digitalWrite(led11, HIGH);
    digitalWrite(led12, HIGH);
  } else {
    // Turn OFF all LEDs
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led12, LOW);
  }

  // Small delay to stabilize the readings
  delay(100);
}
