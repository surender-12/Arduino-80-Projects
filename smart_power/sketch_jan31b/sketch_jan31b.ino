#define SENSOR_PIN 2   // IR Sensor Pin
#define LED_PIN 3      // LED Pin
#define MOTOR_PIN1 4   // Motor IN1 Pin
#define MOTOR_PIN2 5   // Motor IN2 Pin

void setup() {
  pinMode(SENSOR_PIN, INPUT);  // IR Sensor as input
  pinMode(LED_PIN, OUTPUT);    // LED as output
  pinMode(MOTOR_PIN1, OUTPUT); // Motor control pin
  pinMode(MOTOR_PIN2, OUTPUT); // Motor control pin
  
  digitalWrite(LED_PIN, LOW);  // Ensure LED is off initially
  digitalWrite(MOTOR_PIN1, LOW); // Ensure motor is off initially
  digitalWrite(MOTOR_PIN2, LOW); // Ensure motor is off initially
}

void loop() {
  int sensorValue = digitalRead(SENSOR_PIN); // Read the IR sensor value
  
  if (sensorValue == LOW) { // If hand is detected (sensor value is LOW)
    digitalWrite(LED_PIN, HIGH);   // Turn LED on
    digitalWrite(MOTOR_PIN1, HIGH); // Rotate motor in one direction
    digitalWrite(MOTOR_PIN2, LOW);  // Ensure proper motor direction
  }
  else { // If no hand is detected (sensor value is HIGH)
    digitalWrite(LED_PIN, LOW);    // Turn LED off
    digitalWrite(MOTOR_PIN1, LOW); // Stop motor
    digitalWrite(MOTOR_PIN2, LOW); // Stop motor
  }
}
