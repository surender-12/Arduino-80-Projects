#include <Servo.h>

// Define pins for rain sensor and servo motor
const int rainSensorPin = A0; // Analog pin for the rain sensor
const int threshold = 500;    // Threshold for rain detection (adjust as needed)
Servo wiperServo;             // Servo object for controlling the wiper motor

void setup() {
  Serial.begin(9600);

  wiperServo.attach(9);

  // Initial position
  wiperServo.write(0);

  Serial.println("Auto Wiper Control System Initialized!");
}

void loop() {
  int rainValue = analogRead(rainSensorPin);

  Serial.print("Rain Sensor Value: ");
  Serial.println(rainValue);

  // 🔁 REVERSED LOGIC HERE
  if (rainValue < threshold) {
    Serial.println("No rain detected! Activating wipers.");

    // Move servo from 0 to 90
    for (int pos = 0; pos <= 90; pos += 2) {
      wiperServo.write(pos);
      delay(10);
    }

    // Move back to 0
    for (int pos = 90; pos >= 0; pos -= 1) {
      wiperServo.write(pos);
      delay(10);
    }
  } else {
    Serial.println("Rain detected. Wipers off.");
  }

  delay(500);
}